#pragma once
#include "math/Vector3.h"
#include <vector>

class AccumulationBuffer
{
public:
    AccumulationBuffer(int width, int height);

    void addSample(int x, int y, const Vector3& sample);
    Vector3 getAverage(int x, int y) const;
    void reset();

    int getWidth() const;
    int getHeight() const;

private:
    int width;
    int height;
    std::vector<Vector3> accumulator;
    std::vector<int> sampleCounts;
};