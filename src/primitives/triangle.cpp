#include "primitives/Triangle.h"
#include "scene/HitRecord.h"
#include <cmath>
#include <algorithm>
#include <iostream>

Triangle::Triangle(
    const TriangleVertex& v0,
    const TriangleVertex& v1,
    const TriangleVertex& v2,
    std::shared_ptr<Material> material)
    : v0(v0), v1(v1), v2(v2),
      material(material)
{
    Vector3 edge1 = v1.position - v0.position;
    Vector3 edge2 = v2.position - v0.position;
    faceNormal = edge1.cross(edge2).normalized();
}

bool Triangle::hit(const Ray& ray, float tMin, float tMax, HitRecord& record) const
{
    Vector3 edge1 = v1.position - v0.position;
    Vector3 edge2 = v2.position - v0.position;

    Vector3 h = ray.getDirection().cross(edge2);
    float det = edge1.dot(h);

    // ray parallel to triangle
    if (std::fabs(det) < 1e-8f) return false;

    float f = 1.0f / det;
    Vector3 s = ray.getOrigin() - v0.position;

    float u = f * s.dot(h);
    if (u < 0.0f || u > 1.0f) return false;

    Vector3 q = s.cross(edge1);
    float v = f * ray.getDirection().dot(q);
    if (v < 0.0f || u + v > 1.0f) return false;

    float t = f * edge2.dot(q);
    if (t < tMin || t > tMax) return false;

    // interpolate normal using barycentric coords
    float w = 1.0f - u - v;
    Vector3 interpNormal = (v0.normal * w + v1.normal * u + v2.normal * v).normalized();

    // fall back to face normal if vertex normals are zero
    if (interpNormal.nearZero()) interpNormal = faceNormal;

    // interpolate UVs
    float interpU = v0.u * w + v1.u * u + v2.u * v;
    float interpV = v0.v * w + v1.v * u + v2.v * v;

    record.t        = t;
    record.point    = ray.at(t);
    record.u        = interpU;
    record.v        = interpV;
    record.material = material;
    record.setFaceNormal(ray, interpNormal);

    return true;
}

AABB Triangle::boundingBox() const
{
    constexpr float pad = 1e-4f; // pad thin triangles so AABB is never flat

    Vector3 minP(
        std::min({v0.position.x, v1.position.x, v2.position.x}) - pad,
        std::min({v0.position.y, v1.position.y, v2.position.y}) - pad,
        std::min({v0.position.z, v1.position.z, v2.position.z}) - pad
    );
    Vector3 maxP(
        std::max({v0.position.x, v1.position.x, v2.position.x}) + pad,
        std::max({v0.position.y, v1.position.y, v2.position.y}) + pad,
        std::max({v0.position.z, v1.position.z, v2.position.z}) + pad
    );
    return AABB(minP, maxP);
}