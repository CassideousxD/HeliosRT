#pragma once

#include "core/RenderSettings.h"
#include "core/RenderStats.h"
#include "core/TileScheduler.h"
#include "rendering/FrameBuffer.h"
#include "rendering/AccumulationBuffer.h"
#include "camera/Camera.h"
#include "integrator/Integrator.h"
#include "scene/Scene.h"
#include "rendering/Denoiser.h"

#include <thread>
#include <mutex>
#include <atomic>

class Renderer
{
public:
    Renderer(const RenderSettings& settings);
    ~Renderer();

    void render();
    void stop();

    const FrameBuffer& getFrameBuffer() const;
    RenderStats getStats() const;

    mutable std::mutex frameMutex;

private:
    void renderLoop();
    void renderPass();
    void renderTile(const Tile& tile);
    float sampleOffset() const;

    RenderSettings settings;
    FrameBuffer frameBuffer;
    AccumulationBuffer accumulationBuffer;
    Camera camera;
    Integrator integrator;
    Scene scene;
    TileScheduler tileScheduler;

    std::thread renderThread;
    std::atomic<bool> isRendering;
    std::atomic<int> currentPass;

    Denoiser denoiser;
};