#include "primitives/Mesh.h"

Mesh::Mesh(std::vector<std::shared_ptr<Hittable>> triangles)
    : triangles(triangles)
{
    if (!triangles.empty())
    {
        bvh = std::make_shared<BVHNode>(this->triangles, 0, this->triangles.size());
        box = bvh->boundingBox();
    }
}

bool Mesh::hit(const Ray& ray, float tMin, float tMax, HitRecord& record) const
{
    if (!bvh) return false;
    return bvh->hit(ray, tMin, tMax, record);
}

AABB Mesh::boundingBox() const
{
    return box;
}