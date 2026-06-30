#include "acceleration/BVHNode.h"
#include <algorithm>
#include <stdexcept>
#include <array>
#include <limits>

// compare two hittables along a given axis by box center
static bool boxCompare(
    const std::shared_ptr<Hittable>& a,
    const std::shared_ptr<Hittable>& b,
    int axis)
{
    AABB boxA = a->boundingBox();
    AABB boxB = b->boundingBox();

    float centerA = (axis == 0)
        ? (boxA.minimum.x + boxA.maximum.x)
        : (axis == 1)
        ? (boxA.minimum.y + boxA.maximum.y)
        : (boxA.minimum.z + boxA.maximum.z);

    float centerB = (axis == 0)
        ? (boxB.minimum.x + boxB.maximum.x)
        : (axis == 1)
        ? (boxB.minimum.y + boxB.maximum.y)
        : (boxB.minimum.z + boxB.maximum.z);

    return centerA < centerB;
}

static float surfaceArea(const AABB& box)
{
    Vector3 e(
        box.maximum.x - box.minimum.x,
        box.maximum.y - box.minimum.y,
        box.maximum.z - box.minimum.z
    );
    return 2.0f * (e.x * e.y + e.y * e.z + e.z * e.x);
}

BVHNode::BVHNode(std::vector<std::shared_ptr<Hittable>>& objects,
                 size_t start, size_t end)
{
    // build combined box for all objects in range
    box = objects[start]->boundingBox();
    for (size_t i = start + 1; i < end; i++)
        box = box.expand(objects[i]->boundingBox());

    size_t count = end - start;

    if (count == 1)
    {
        left = right = objects[start];
    }
    else if (count == 2)
    {
        left  = objects[start];
        right = objects[start + 1];
    }
    else if (count <= 4)
    {
        // small count — naive midpoint split on longest axis
        Vector3 extent(
            box.maximum.x - box.minimum.x,
            box.maximum.y - box.minimum.y,
            box.maximum.z - box.minimum.z
        );
        int axis = 0;
        if (extent.y > extent.x) axis = 1;
        if (extent.z > ((axis == 0) ? extent.x : extent.y)) axis = 2;

        std::sort(objects.begin() + start, objects.begin() + end,
            [axis](const std::shared_ptr<Hittable>& a,
                   const std::shared_ptr<Hittable>& b)
            { return boxCompare(a, b, axis); });

        size_t mid = start + count / 2;
        left  = std::make_shared<BVHNode>(objects, start, mid);
        right = std::make_shared<BVHNode>(objects, mid,   end);
    }
    else
    {
        // SAH bucketing
        constexpr int numBuckets = 12;
        constexpr float T_box = 1.0f;
        constexpr float T_prim = 2.0f;

        float bestCost = std::numeric_limits<float>::infinity();
        int   bestAxis = 0;
        int   bestBucket = 0;

        float parentArea = surfaceArea(box);

        for (int axis = 0; axis < 3; axis++)
        {
            float axisMin = (axis == 0) ? box.minimum.x
                          : (axis == 1) ? box.minimum.y : box.minimum.z;
            float axisMax = (axis == 0) ? box.maximum.x
                          : (axis == 1) ? box.maximum.y : box.maximum.z;

            if (axisMax - axisMin < 1e-6f) continue;

            // assign objects to buckets
            struct Bucket { int count = 0; AABB box; bool hasBox = false; };
            std::array<Bucket, numBuckets> buckets;

            for (size_t i = start; i < end; i++)
            {
                AABB objBox = objects[i]->boundingBox();
                float centroid = (axis == 0)
                    ? (objBox.minimum.x + objBox.maximum.x) * 0.5f
                    : (axis == 1)
                    ? (objBox.minimum.y + objBox.maximum.y) * 0.5f
                    : (objBox.minimum.z + objBox.maximum.z) * 0.5f;

                int b = static_cast<int>(
                    numBuckets * (centroid - axisMin) / (axisMax - axisMin));
                b = std::clamp(b, 0, numBuckets - 1);

                buckets[b].count++;
                buckets[b].box = buckets[b].hasBox
                    ? buckets[b].box.expand(objBox)
                    : objBox;
                buckets[b].hasBox = true;
            }

            // evaluate numBuckets-1 split positions
            for (int split = 1; split < numBuckets; split++)
            {
                AABB leftBox;  bool leftHas  = false;
                AABB rightBox; bool rightHas = false;
                int  leftCount = 0, rightCount = 0;

                for (int b = 0; b < split; b++)
                {
                    if (!buckets[b].hasBox) continue;
                    leftCount += buckets[b].count;
                    leftBox = leftHas
                        ? leftBox.expand(buckets[b].box)
                        : buckets[b].box;
                    leftHas = true;
                }
                for (int b = split; b < numBuckets; b++)
                {
                    if (!buckets[b].hasBox) continue;
                    rightCount += buckets[b].count;
                    rightBox = rightHas
                        ? rightBox.expand(buckets[b].box)
                        : buckets[b].box;
                    rightHas = true;
                }

                if (!leftHas || !rightHas) continue;

                float cost = 2.0f * T_box
                    + (surfaceArea(leftBox)  / parentArea) * leftCount  * T_prim
                    + (surfaceArea(rightBox) / parentArea) * rightCount * T_prim;

                if (cost < bestCost)
                {
                    bestCost   = cost;
                    bestAxis   = axis;
                    bestBucket = split;
                }
            }
        }

        // partition objects by best split
        float axisMin = (bestAxis == 0) ? box.minimum.x
                      : (bestAxis == 1) ? box.minimum.y : box.minimum.z;
        float axisMax = (bestAxis == 0) ? box.maximum.x
                      : (bestAxis == 1) ? box.maximum.y : box.maximum.z;

        auto mid = std::partition(objects.begin() + start, objects.begin() + end,
            [&](const std::shared_ptr<Hittable>& obj)
            {
                AABB objBox = obj->boundingBox();
                float centroid = (bestAxis == 0)
                    ? (objBox.minimum.x + objBox.maximum.x) * 0.5f
                    : (bestAxis == 1)
                    ? (objBox.minimum.y + objBox.maximum.y) * 0.5f
                    : (objBox.minimum.z + objBox.maximum.z) * 0.5f;

                int b = static_cast<int>(
                    numBuckets * (centroid - axisMin) / (axisMax - axisMin));
                b = std::clamp(b, 0, numBuckets - 1);
                return b < bestBucket;
            });

        size_t midIndex = mid - objects.begin();

        // fallback if partition degenerated
        if (midIndex == start || midIndex == end)
            midIndex = start + count / 2;

        left  = std::make_shared<BVHNode>(objects, start,    midIndex);
        right = std::make_shared<BVHNode>(objects, midIndex, end);
    }
}

bool BVHNode::hit(const Ray& ray, float tMin, float tMax, HitRecord& record) const
{
    // miss the bounding box → skip everything inside
    if (!box.hit(ray, tMin, tMax))
        return false;

    bool hitLeft  = left->hit(ray,  tMin, tMax, record);
    bool hitRight = right->hit(ray, tMin, hitLeft ? record.t : tMax, record);

    return hitLeft || hitRight;
}

AABB BVHNode::boundingBox() const
{
    return box;
}