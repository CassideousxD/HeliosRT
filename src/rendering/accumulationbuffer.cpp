#include "rendering/AccumulationBuffer.h"
#include <cmath>
#include <algorithm>
#include <limits>

AccumulationBuffer::AccumulationBuffer(int width, int height)
    : width(width),
      height(height),
      accumulator(width * height),
      sampleCounts(width * height, 0),
      m2(width * height)
{
}

void AccumulationBuffer::addSample(int x, int y, const Vector3& sample)
{
    int index = y * width + x;
    sampleCounts[index]++;
    int count = sampleCounts[index];

    Vector3& mean = accumulator[index];
    Vector3 delta = sample - mean;
    mean = mean + delta * (1.0f / count);
    Vector3 delta2 = sample - mean;

    // Welford's running M2 (per channel)
    m2[index] = Vector3(
        m2[index].x + delta.x * delta2.x,
        m2[index].y + delta.y * delta2.y,
        m2[index].z + delta.z * delta2.z
    );
}

Vector3 AccumulationBuffer::getAverage(int x, int y) const
{
    return accumulator[y * width + x];
}

float AccumulationBuffer::getStandardError(int x, int y) const
{
    int index = y * width + x;
    int n = sampleCounts[index];
    if (n < 2) return std::numeric_limits<float>::infinity(); // not enough samples yet

    Vector3 variance = m2[index] / static_cast<float>(n - 1);

    // luminance-weighted variance (Rec. 709 weights)
    float lumVariance =
        0.2126f * variance.x +
        0.7152f * variance.y +
        0.0722f * variance.z;

    return std::sqrt(std::max(lumVariance, 0.0f) / static_cast<float>(n));
}

bool AccumulationBuffer::isConverged(int x, int y, float threshold) const
{
    return getStandardError(x, y) < threshold;
}

void AccumulationBuffer::reset()
{
    std::fill(accumulator.begin(), accumulator.end(), Vector3());
    std::fill(sampleCounts.begin(), sampleCounts.end(), 0);
    std::fill(m2.begin(), m2.end(), Vector3());
}

int AccumulationBuffer::getWidth() const { return width; }
int AccumulationBuffer::getHeight() const { return height; }