#include "material/ImageTexture.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <algorithm>
#include <cmath>
#include <iostream>

namespace
{
    float srgbToLinear(unsigned char value)
    {
        float channel = static_cast<float>(value) / 255.0f;

        if (channel <= 0.04045f)
            return channel / 12.92f;

        return std::pow((channel + 0.055f) / 1.055f, 2.4f);
    }
}

ImageTexture::ImageTexture(const std::string& filepath)
    : width(0),
      height(0)
{
    int sourceChannels = 0;
    unsigned char* data = stbi_load(
        filepath.c_str(),
        &width,
        &height,
        &sourceChannels,
        STBI_rgb);

    if (!data)
    {
        width = 0;
        height = 0;
        std::cerr << "ImageTexture: could not load " << filepath
                  << ": " << stbi_failure_reason() << std::endl;
        return;
    }

    const size_t byteCount =
        static_cast<size_t>(width) * static_cast<size_t>(height) * 3;
    pixels.assign(data, data + byteCount);
    stbi_image_free(data);
}

Vector3 ImageTexture::sample(float u, float v, const Vector3& point) const
{
    if (!isLoaded())
        return Vector3(1.0f, 0.0f, 1.0f);

    u = std::clamp(u, 0.0f, 1.0f);
    v = 1.0f - std::clamp(v, 0.0f, 1.0f);

    int x = std::min(static_cast<int>(u * width), width - 1);
    int y = std::min(static_cast<int>(v * height), height - 1);

    size_t index =
        (static_cast<size_t>(y) * static_cast<size_t>(width)
         + static_cast<size_t>(x)) * 3;

    return Vector3(
        srgbToLinear(pixels[index]),
        srgbToLinear(pixels[index + 1]),
        srgbToLinear(pixels[index + 2]));
}

bool ImageTexture::isLoaded() const
{
    return width > 0 && height > 0 && !pixels.empty();
}
