#include "scene/Scene.h"
#include "acceleration/BVHNode.h"

void Scene::add(std::shared_ptr<Hittable> object)
{
    objects.push_back(object);
}

bool Scene::hit(const Ray& ray, float tMin, float tMax, HitRecord& record) const
{
    if (bvhRoot)
        return bvhRoot->hit(ray, tMin, tMax, record);

    // fallback to linear if BVH not built
    HitRecord tempRecord;
    bool hitAnything = false;
    float closestSoFar = tMax;

    for (const auto& object : objects)
    {
        if (object->hit(ray, tMin, closestSoFar, tempRecord))
        {
            hitAnything = true;
            closestSoFar = tempRecord.t;
            record = tempRecord;
        }
    }
    return hitAnything;
}

const Environment& Scene::getEnvironment() const
{
    return environment;
}

Environment& Scene::getEnvironment()
{
    return environment;
}

void Scene::buildBVH()
{
    if (objects.empty()) return;
    bvhRoot = std::make_shared<BVHNode>(objects, 0, objects.size());
}

void Scene::addLight(std::shared_ptr<Light> light)
{
    lights.push_back(light);
}

const std::vector<std::shared_ptr<Light>>& Scene::getLights() const
{
    return lights;
}