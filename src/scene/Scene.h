#ifndef SCENE_H
#define SCENE_H
#include "scene/Hittable.h"
#include "scene/HitRecord.h"
#include "math/Ray.h"
#include "scene/Environment.h"
#include <vector>
#include <memory>

class Scene
{
public:
    void add(std::shared_ptr<Hittable> object);
    bool hit(const Ray& ray, float tMin, float tMax, HitRecord& record) const;
    const Environment& getEnvironment() const;
    Environment& getEnvironment();
    void buildBVH();
    std::shared_ptr<Hittable> bvhRoot;

private:
    std::vector<std::shared_ptr<Hittable>> objects;
    Environment environment;
};

#endif