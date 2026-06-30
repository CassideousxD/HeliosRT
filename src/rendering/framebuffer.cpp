#include "rendering/FrameBuffer.h"
#include <algorithm>
#include <cmath>

FrameBuffer::FrameBuffer(int width, int height)
    : width(width),
      height(height),
      pixels(width * height)
{
}

int FrameBuffer::getWidth() const
{
    return width;
}

int FrameBuffer::getHeight() const
{
    return height;
}

void FrameBuffer::setPixel(int x, int y, const Vector3& color)
{
    Vector3 finalColor = clampColor(color);
    finalColor = gammaCorrect(finalColor);
    pixels[y * width + x] = finalColor;
}

const Vector3& FrameBuffer::getPixel(int x, int y) const
{
    return pixels[y * width + x];
}

Vector3 FrameBuffer::clampColor(const Vector3& color) const
{
    return Vector3(
        std::clamp(color.x, 0.0f, 1.0f),
        std::clamp(color.y, 0.0f, 1.0f),
        std::clamp(color.z, 0.0f, 1.0f)
    );
}

Vector3 FrameBuffer::gammaCorrect(const Vector3& color) const
{
    return Vector3(
        std::sqrt(color.x),
        std::sqrt(color.y),
        std::sqrt(color.z)
    );
}