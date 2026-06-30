#ifndef SPHERE_H
#define SPHERE_H
#include "scene/Hittable.h"
#include "math/Vector3.h"
#include <memory>
#include "material/Material.h"

class Sphere : public Hittable
{
public:
    Sphere(const Vector3& center, float radius,std::shared_ptr<Material> material);
    bool hit(const Ray& ray, float tMin, float tMax, HitRecord& record) const override;
    AABB boundingBox() const override;
    Sphere(const Vector3& centerStart,const Vector3& centerEnd,float timeStart,float timeEnd,float radius,std::shared_ptr<Material> material);

private:
    Vector3 center;
    float radius;
    std::shared_ptr<Material> material;
    static void getUV(const Vector3& point, float& u, float& v);
    Vector3 centerEnd;
    float timeStart;
    float timeEnd;
    bool isMoving;
    Vector3 centerAt(float time) const;
};

#endif