#ifndef CAMERA_H
#define CAMERA_H
#include "math/Ray.h"
#include "math/Vector3.h"

class Camera
{
public:
    Camera(
        int imageWidth,
        int imageHeight,
        float verticalFov,
        float aperture,
        float focusDistance
    );

    Camera(
        const Vector3& position,
        int imageWidth,
        int imageHeight,
        float verticalFov,
        float aperture,
        float focusDistance
    );
    const Vector3& getPosition() const;
    Ray getRay(float u, float v) const;

private:
    void initialize();
    Vector3 position;

    int imageWidth;
    int imageHeight;

    float aspectRatio;
    float verticalFov;
    float lensRadius;
    float focusDistance;

    Vector3 horizontal;
    Vector3 vertical;
    Vector3 lowerLeftCorner;
};

#endif