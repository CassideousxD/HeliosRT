#include "math/Random.h"
#include <cmath>
#include <cstdlib>
#include <random>
namespace
{
    std::mt19937 generator(std::random_device{}());
    std::uniform_real_distribution<float> distribution(0.0f, 1.0f);
}

float Random::randomFloat()
{
    return distribution(generator);
}

float Random::randomFloat(float min, float max)
{
    return min + (max - min) * randomFloat();
}

Vector3 Random::randomVector()
{
    return Vector3(
        randomFloat(),
        randomFloat(),
        randomFloat()
    );
}

Vector3 Random::randomVector(float min, float max)
{
    return Vector3(
        randomFloat(min, max),
        randomFloat(min, max),
        randomFloat(min, max)
    );
}

Vector3 Random::randomInUnitSphere()
{
    while (true)
    {
        Vector3 point = randomVector(-1.0f, 1.0f);
        if (point.lengthSquared() >= 1.0f)
        {
            continue;
        }
        return point;
    }
}

Vector3 Random::randomUnitVector()
{
    return randomInUnitSphere().normalized();
}

Vector3 Random::randomInHemisphere(const Vector3& normal)
{
    Vector3 point = randomInUnitSphere();
    if (point.dot(normal) > 0.0f)
    {
        return point;
    }
    return -point;
}

Vector3 Random::randomInUnitDisk()
{
    while (true)
    {
        Vector3 point(
            randomFloat(-1.0f, 1.0f),
            randomFloat(-1.0f, 1.0f),
            0.0f
        );
        if (point.lengthSquared() >= 1.0f)
        {
            continue;
        }
        return point;
    }
}

