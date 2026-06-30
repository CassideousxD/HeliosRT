#pragma once

class AccumulationBuffer;
class FrameBuffer;

class Denoiser
{
public:
    bool denoise(
        const AccumulationBuffer& input,
        FrameBuffer& output
    ) const;
};