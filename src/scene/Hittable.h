#ifndef HITTABLE_H
#define HITTABLE_H
#include "math/Ray.h"
#include "scene/HitRecord.h"
#include "acceleration/AABB.h"

class Hittable
{
public:
    virtual ~Hittable() = default;
    virtual bool hit(const Ray& ray, float tMin, float tMax, HitRecord& record) const = 0;
    virtual AABB boundingBox() const = 0;

};

#endif