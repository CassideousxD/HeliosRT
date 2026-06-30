#include "rendering/Denoiser.h"
#include "rendering/AccumulationBuffer.h"
#include "rendering/FrameBuffer.h"

#include <OpenImageDenoise/oidn.hpp>

#include <algorithm>
#include <cmath>
#include <iostream>

namespace
{
    float sanitize(float value)
    {
        if (!std::isfinite(value))
            return 0.0f;

        return std::max(value, 0.0f);
    }
}

bool Denoiser::denoise(
    const AccumulationBuffer& input,
    FrameBuffer& output
) const
{
    int width = input.getWidth();
    int height = input.getHeight();

    if (width != output.getWidth() ||
        height != output.getHeight())
    {
        std::cerr
            << "Denoiser: buffer dimensions do not match"
            << std::endl;

        return false;
    }

    size_t pixelCount =
        static_cast<size_t>(width) *
        static_cast<size_t>(height);

    size_t bufferSize =
        pixelCount * 3 * sizeof(float);

    oidn::DeviceRef device = oidn::newDevice();
    device.commit();

    oidn::BufferRef colorBuffer =
        device.newBuffer(bufferSize);

    oidn::BufferRef outputBuffer =
        device.newBuffer(bufferSize);

    float* colorData =
        static_cast<float*>(colorBuffer.getData());

    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            Vector3 color = input.getAverage(x, y);

            size_t index =
                (static_cast<size_t>(y) *
                 static_cast<size_t>(width) +
                 static_cast<size_t>(x)) * 3;

            colorData[index]     = sanitize(color.x);
            colorData[index + 1] = sanitize(color.y);
            colorData[index + 2] = sanitize(color.z);
        }
    }

    oidn::FilterRef filter =
        device.newFilter("RT");

    filter.setImage(
        "color",
        colorBuffer,
        oidn::Format::Float3,
        width,
        height
    );

    filter.setImage(
        "output",
        outputBuffer,
        oidn::Format::Float3,
        width,
        height
    );

    filter.set("hdr", true);
    filter.commit();
    filter.execute();

    const char* errorMessage = nullptr;

    if (device.getError(errorMessage) != oidn::Error::None)
    {
        std::cerr
            << "Denoiser error: "
            << (errorMessage ? errorMessage : "unknown error")
            << std::endl;

        return false;
    }

    const float* denoisedData =
        static_cast<const float*>(outputBuffer.getData());

    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            size_t index =
                (static_cast<size_t>(y) *
                 static_cast<size_t>(width) +
                 static_cast<size_t>(x)) * 3;

            output.setPixel(
                x,
                y,
                Vector3(
                    denoisedData[index],
                    denoisedData[index + 1],
                    denoisedData[index + 2]
                )
            );
        }
    }

    return true;
}