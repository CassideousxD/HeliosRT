#ifndef INTEGRATOR_H
#define INTEGRATOR_H
#include "math/Ray.h"
#include "math/Vector3.h"
#include "scene/Scene.h"

class Integrator
{
public:
    Integrator();
    Vector3 trace(const Ray& ray, const Scene& scene, int depth) const;

private:

};

#endif