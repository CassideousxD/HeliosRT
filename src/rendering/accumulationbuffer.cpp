#include "rendering/AccumulationBuffer.h"

AccumulationBuffer::AccumulationBuffer(int width, int height)
    : width(width),
      height(height),
      accumulator(width * height),
      sampleCounts(width * height, 0)
{
}

void AccumulationBuffer::addSample(int x, int y, const Vector3& sample)
{
    int index = y * width + x;
    sampleCounts[index]++;
    int count = sampleCounts[index];

    // Welford running average
    Vector3& current = accumulator[index];
    current = current + (sample - current) * (1.0f / count);
}

Vector3 AccumulationBuffer::getAverage(int x, int y) const
{
    return accumulator[y * width + x];
}

void AccumulationBuffer::reset()
{
    std::fill(accumulator.begin(), accumulator.end(), Vector3());
    std::fill(sampleCounts.begin(), sampleCounts.end(), 0);
}

int AccumulationBuffer::getWidth() const { return width; }
int AccumulationBuffer::getHeight() const { return height; }