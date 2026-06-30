#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H
#include "math/Vector3.h"
#include <vector>

class FrameBuffer
{
public:
    FrameBuffer(int width, int height);

    int getWidth() const;
    int getHeight() const;
    void setPixel(int x, int y, const Vector3& color);
    const Vector3& getPixel(int x, int y) const;

private:
    int width;
    int height;
    std::vector<Vector3> pixels;
    Vector3 clampColor(const Vector3& color) const;
    Vector3 gammaCorrect(const Vector3& color) const;
};

#endif