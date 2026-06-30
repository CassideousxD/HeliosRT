#include "primitives/Sphere.h"
#include <cmath>

Sphere::Sphere(const Vector3& center, float radius,std::shared_ptr<Material> material)
    : center(center),
      radius(radius),
      material(std::move(material))
{
}

bool Sphere::hit(const Ray& ray,float tMin,float tMax,HitRecord& record) const
{
    Vector3 oc = ray.getOrigin() - center;
    float a = ray.getDirection().dot(ray.getDirection());
    float h = oc.dot(ray.getDirection());
    float c = oc.dot(oc) - radius * radius;
    float discriminant = h * h - a * c;

    if (discriminant < 0.0f)
    {
        return false;
    }

    float sqrtD = std::sqrt(discriminant);
    float t = (-h - sqrtD) / a;
    if (t < tMin || t > tMax)
    {
        t = (-h + sqrtD) / a;

        if (t < tMin || t > tMax)
        {
            return false;
        }
    }

    record.t = t;
    record.point = ray.at(t);

    Vector3 outwardNormal = (record.point - center) / radius;
    record.setFaceNormal(ray, outwardNormal);

    record.material = material;
    Vector3 unitPoint = (record.point - center) / radius;
    getUV(unitPoint, record.u, record.v);

    return true;
}

AABB Sphere::boundingBox() const
{
    float absoluteRadius = std::fabs(radius);
    Vector3 radiusVec(
        absoluteRadius,
        absoluteRadius,
        absoluteRadius
    );

    return AABB(center - radiusVec, center + radiusVec);
}

void Sphere::getUV(const Vector3& point, float& u, float& v)
{
    // point is assumed to be on unit sphere centered at origin
    float theta = std::acos(-point.y);
    float phi   = std::atan2(-point.z, point.x) + M_PI;

    u = phi   / (2.0f * M_PI);
    v = theta / M_PI;
}