#include "camera/Camera.h"
#include "math/Random.h"
#include <cmath>

Camera::Camera(
    int imageWidth,
    int imageHeight,
    float verticalFov,
    float aperture,
    float focusDistance,
    float shutterOpen,
    float shutterClose
)
    : position(0.0f, 0.0f, 0.0f),
      imageWidth(imageWidth),
      imageHeight(imageHeight),
      verticalFov(verticalFov),
      lensRadius(aperture > 0.0f ? aperture * 0.5f : 0.0f),
      focusDistance(focusDistance > 0.0f ? focusDistance : 0.001f),
      shutterOpen(shutterOpen),
      shutterClose(shutterClose)
{
    initialize();
}

Camera::Camera(
    const Vector3& position,
    int imageWidth,
    int imageHeight,
    float verticalFov,
    float aperture,
    float focusDistance,
    float shutterOpen,
    float shutterClose
)
    : position(position),
      imageWidth(imageWidth),
      imageHeight(imageHeight),
      verticalFov(verticalFov),
      lensRadius(aperture > 0.0f ? aperture * 0.5f : 0.0f),
      focusDistance(focusDistance > 0.0f ? focusDistance : 0.001f),
      shutterOpen(shutterOpen),
      shutterClose(shutterClose)
{
    initialize();
}

void Camera::initialize()
{
    constexpr float PI = 3.14159265358979323846f;

    aspectRatio =
        static_cast<float>(imageWidth) /
        static_cast<float>(imageHeight);

    float theta = verticalFov * PI / 180.0f;

    float viewportHeight =
        2.0f * std::tan(theta * 0.5f) * focusDistance;

    float viewportWidth =
        viewportHeight * aspectRatio;

    horizontal = Vector3(
        viewportWidth,
        0.0f,
        0.0f
    );

    vertical = Vector3(
        0.0f,
        -viewportHeight,
        0.0f
    );

    lowerLeftCorner =
        position
        - horizontal * 0.5f
        - vertical * 0.5f
        - Vector3(0.0f, 0.0f, focusDistance);
}

const Vector3& Camera::getPosition() const
{
    return position;
}

Ray Camera::getRay(float u, float v) const
{
    Vector3 offset;

    if (lensRadius > 0.0f)
    {
        Vector3 diskSample =
            Random::randomInUnitDisk() * lensRadius;

        offset = Vector3(
            diskSample.x,
            diskSample.y,
            0.0f
        );
    }

    Vector3 rayOrigin = position + offset;

    Vector3 focusPoint =
        lowerLeftCorner
        + horizontal * u
        + vertical * v;

    Vector3 direction =
        (focusPoint - rayOrigin).normalized();

    float rayTime = shutterOpen +
        Random::randomFloat() * (shutterClose - shutterOpen);

    return Ray(rayOrigin, direction, rayTime);
}