#ifndef SCENE_H
#define SCENE_H
#include "scene/Hittable.h"
#include "scene/HitRecord.h"
#include "math/Ray.h"
#include "scene/Environment.h"
#include "scene/Light.h"
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
    void addLight(std::shared_ptr<Light> light);
    const std::vector<std::shared_ptr<Light>>& getLights() const;

private:
    std::vector<std::shared_ptr<Hittable>> objects;
    std::vector<std::shared_ptr<Light>> lights;
    Environment environment;
};

#endif