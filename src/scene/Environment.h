#pragma once
#include "math/Vector3.h"
#include "math/Ray.h"
#include <string>
#include <vector>

class Environment
{
public:
    Environment() = default;
    ~Environment() = default;
    Vector3 sample(const Ray& ray) const;
    bool loadHDRI(const std::string& filepath);
    bool isHDRILoaded() const;
    void setIntensity(float intensity);

private:
    int width = 0;
    int height = 0;
    float intensity = 1.0f;
    std::vector<float> pixels;
};