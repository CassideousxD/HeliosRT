#include "scene/Environment.h"
#include "stb_image.h"
#include <algorithm>
#include <cmath>
#include <iostream>

Vector3 Environment::sample(const Ray& ray) const
{
    Vector3 direction = ray.getDirection().normalized();

    if (!isHDRILoaded())
    {
        float blend = 0.5f * (direction.y + 1.0f);

        return Vector3(1.0f, 1.0f, 1.0f) * (1.0f - blend)
             + Vector3(0.5f, 0.7f, 1.0f) * blend;
    }

    constexpr float PI = 3.14159265358979323846f;

    float phi = std::atan2(-direction.z, direction.x) + PI;
    float theta = std::acos(
        std::clamp(direction.y, -1.0f, 1.0f)
    );

    float u = phi / (2.0f * PI);
    float v = theta / PI;

    int x = std::min(
        static_cast<int>(u * width),
        width - 1
    );

    int y = std::min(
        static_cast<int>(v * height),
        height - 1
    );

    size_t index =
        (static_cast<size_t>(y) * static_cast<size_t>(width)
         + static_cast<size_t>(x)) * 3;

    return Vector3(
        pixels[index],
        pixels[index + 1],
        pixels[index + 2]
    ) * intensity;
}

bool Environment::loadHDRI(const std::string& filepath)
{
    int loadedWidth = 0;
    int loadedHeight = 0;
    int sourceChannels = 0;

    float* data = stbi_loadf(
        filepath.c_str(),
        &loadedWidth,
        &loadedHeight,
        &sourceChannels,
        STBI_rgb
    );

    if (!data)
    {
        std::cerr
            << "Environment: could not load HDRI "
            << filepath
            << ": "
            << stbi_failure_reason()
            << std::endl;

        return false;
    }

    width = loadedWidth;
    height = loadedHeight;

    size_t valueCount =
        static_cast<size_t>(width) *
        static_cast<size_t>(height) * 3;

    pixels.assign(data, data + valueCount);

    stbi_image_free(data);

    std::cout
        << "Environment: loaded HDRI "
        << width << "x" << height
        << std::endl;

    return true;
}

bool Environment::isHDRILoaded() const
{
    return width > 0 &&
           height > 0 &&
           !pixels.empty();
}

void Environment::setIntensity(float intensity)
{
    this->intensity = std::max(0.0f, intensity);
}