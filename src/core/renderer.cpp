#include "core/Renderer.h"
#include "primitives/Sphere.h"
#include "loaders/OBJLoader.h"
#include "material/ImageTexture.h"
#include "scene/TransformNode.h"
#include "scene/SphereLight.h"
#include "material/Lambertian.h"
#include "material/Metal.h"
#include "material/Glass.h"
#include "material/Emissive.h"
#include "material/Checkerboard.h"
#include "material/SolidColor.h"
#include "math/Random.h"
#include "material/RoughConductor.h"
#include "material/RoughDielectric.h"
#include "primitives/ConstantMedium.h"
#include <iostream>
#include <memory>

Renderer::Renderer(const RenderSettings& settings)
    : settings(settings),
      frameBuffer(settings.width, settings.height),
      accumulationBuffer(settings.width, settings.height),
      camera(settings.width,settings.height,settings.verticalFov,settings.aperture,settings.focusDistance,settings.shutterOpen,settings.shutterClose),
      integrator(),
      scene(),
      tileScheduler(settings.width, settings.height),
      isRendering(false),
      currentPass(0)
{
    /*auto checker = std::make_shared<Checkerboard>(
        std::make_shared<SolidColor>(Vector3(0.9f, 0.9f, 0.9f)),
        std::make_shared<SolidColor>(Vector3(0.2f, 0.5f, 0.1f))
    );
    auto groundMaterial = std::make_shared<Lambertian>(checker);
    auto centerMaterial =
        std::make_shared<Lambertian>(Vector3(0.7f, 0.3f, 0.3f));
    auto leftMetal =
        std::make_shared<Metal>(Vector3(0.8f, 0.8f, 0.8f), 0.1f);
    auto rightMetal =
        std::make_shared<Metal>(Vector3(0.8f, 0.6f, 0.2f), 0.4f);
    auto glassMaterial =
        std::make_shared<Glass>(1.5f);
    auto lightMaterial =
        std::make_shared<Emissive>(Vector3(1.0f, 0.9f, 0.7f), 4.0f);

    scene.add(std::make_shared<Sphere>(
        Vector3(0.0f, -100.5f, -1.0f), 100.0f, groundMaterial));
    scene.add(std::make_shared<Sphere>(
        Vector3(0.0f, 0.0f, -1.2f), 0.5f, centerMaterial));
    scene.add(std::make_shared<Sphere>(
        Vector3(-1.0f, 0.0f, -1.0f), 0.5f, glassMaterial));
    scene.add(std::make_shared<Sphere>(
        Vector3(-1.0f, 0.0f, -1.0f), -0.4f, glassMaterial));
    scene.add(std::make_shared<Sphere>(
        Vector3(1.0f, 0.0f, -1.0f), 0.5f, rightMetal));
    scene.add(std::make_shared<Sphere>(
       Vector3(0.0f, 2.0f, -1.0f), 0.8f, lightMaterial));*/

    /*auto bunnyMaterial = std::make_shared<Lambertian>(Vector3(0.8f, 0.5f, 0.3f));
    auto bunny = OBJLoader::load("../objs/stanford-bunny.obj", bunnyMaterial);

    if (bunny)
    {
        constexpr float bunnyLocalMinY = 0.032887f;
        constexpr float bunnyScale     = 15.0f;
        constexpr float groundY        = -0.5f;

        float translateY = groundY - bunnyScale * bunnyLocalMinY;

        auto bunnyStartTransform = Transform(
            Vector3(0.0f, translateY, -3.0f),
            Vector3(0.0f, 180.0f, 0.0f),
            Vector3(bunnyScale, bunnyScale, bunnyScale));

        auto bunnyEndTransform = Transform(
            Vector3(1.5f, translateY, -3.0f),   // moves 1.5 units along X
            Vector3(0.0f, 180.0f, 0.0f),
            Vector3(bunnyScale, bunnyScale, bunnyScale));

        auto bunnyInstance =
            std::make_shared<TransformNode>(
                bunny,
                bunnyStartTransform,
                bunnyEndTransform,
                0.0f, 1.0f);

        scene.add(bunnyInstance);
    }

    auto imageTexture =
        std::make_shared<ImageTexture>("../textures/Screenshot 2026-06-30 at 10.18.16 PM.png");

    if (!imageTexture->isLoaded())
    {
        std::cerr << "Test image failed to load!" << std::endl;
    }

    auto imageMaterial =
        std::make_shared<Lambertian>(imageTexture);

    scene.add(std::make_shared<Sphere>(
            Vector3(0.0f, 0.0f, -1.2f),
            0.5f,
            imageMaterial
        ));

    auto movingSphere = std::make_shared<Sphere>(
    Vector3(0.0f, 0.0f, -1.0f),   // start position
    Vector3(0.3f, 0.0f, -1.0f),   // end position
    0.0f, 1.0f,                    // shutter open/close
    0.5f,
    centerMaterial);

    scene.add(movingSphere);

    auto lightCenter = Vector3(0.0f, 2.0f, -1.0f);
    float lightRadius = 0.8f;

    scene.add(std::make_shared<Sphere>(lightCenter, lightRadius, lightMaterial));
    scene.addLight(std::make_shared<SphereLight>(
        lightCenter, lightRadius, Vector3(1.0f, 0.9f, 0.7f) * 4.0f));

    auto roughGold = std::make_shared<RoughConductor>(Vector3(1.0f, 0.86f, 0.57f), 0.3f);
    auto roughPlastic = std::make_shared<RoughDielectric>(Vector3(0.8f, 0.1f, 0.1f), 0.4f);

    scene.add(std::make_shared<Sphere>(Vector3(-1.2f, 0.0f, -1.0f), 0.5f, roughGold));
    scene.add(std::make_shared<Sphere>(Vector3( 1.2f, 0.0f, -1.0f), 0.5f, roughPlastic));*/

    auto fogBoundary = std::make_shared<Sphere>(
        Vector3(0.0f, 0.0f, -1.0f), 0.7f,
        std::make_shared<Lambertian>(Vector3(1,1,1))); 

    auto fog = std::make_shared<ConstantMedium>(
        fogBoundary, 1.0f, Vector3(0.9f, 0.9f, 1.0f));

    scene.add(fog);

    if (scene.getEnvironment().loadHDRI("../hdris/rogland_clear_night_4k.hdr"))
    {
        scene.getEnvironment().setIntensity(1.0f);
    }

    scene.buildBVH();
}

Renderer::~Renderer()
{
    stop();
}

void Renderer::render()
{
    isRendering = true;
    renderThread = std::thread(&Renderer::renderLoop, this);
}

void Renderer::stop()
{
    isRendering = false;
    if (renderThread.joinable())
        renderThread.join();
}

void Renderer::renderLoop()
{
    for (
        int pass = 1;
        pass <= settings.samplesPerPixel && isRendering;
        pass++
    )
    {
        currentPass = pass;
        tileScheduler.reset();
        renderPass();

        std::lock_guard<std::mutex> lock(frameMutex);

        for (int y = 0; y < frameBuffer.getHeight(); y++)
        {
            for (int x = 0; x < frameBuffer.getWidth(); x++)
            {
                frameBuffer.setPixel(
                    x,
                    y,
                    accumulationBuffer.getAverage(x, y)
                );
            }
        }
    }

    bool completed =
        isRendering &&
        currentPass.load() == settings.samplesPerPixel;

    if (completed && settings.enableDenoiser)
    {
        std::cout << "Denoising final image..." << std::endl;

        std::lock_guard<std::mutex> lock(frameMutex);

        if (denoiser.denoise(
                accumulationBuffer,
                frameBuffer))
        {
            std::cout << "Denoising complete" << std::endl;
        }
    }

    isRendering = false;
}

void Renderer::renderPass()
{
    int numThreads = settings.threadCount;
    std::vector<std::thread> threads;
    threads.reserve(numThreads);

    for (int i = 0; i < numThreads; i++)
    {
        threads.emplace_back([this]()
        {
            while (isRendering)
            {
                auto scheduled = tileScheduler.getNextTile();
                if (!scheduled) break;
                renderTile(*scheduled);
            }
        });
    }

    for (auto& t : threads)
        t.join();
}

void Renderer::renderTile(const ScheduledTile& scheduledTile)
{
    const Tile& tile = scheduledTile.tile;
    bool allConverged = settings.enableAdaptiveSampling;

    for (int y = tile.y; y < tile.y + tile.height; y++)
    {
        for (int x = tile.x; x < tile.x + tile.width; x++)
        {
            float u = (x + sampleOffset()) / (frameBuffer.getWidth()  - 1);
            float v = (y + sampleOffset()) / (frameBuffer.getHeight() - 1);

            Ray ray = camera.getRay(u, v);
            Vector3 sample = integrator.trace(ray, scene, settings.maxRayDepth);

            accumulationBuffer.addSample(x, y, sample);

            if (settings.enableAdaptiveSampling &&
                !accumulationBuffer.isConverged(x, y, settings.adaptiveThreshold))
            {
                allConverged = false;
            }
        }
    }

    if (allConverged)
    {
        tileScheduler.markConverged(scheduledTile.index);
    }
}

float Renderer::sampleOffset() const
{
    return Random::randomFloat();
}

const FrameBuffer& Renderer::getFrameBuffer() const
{
    return frameBuffer;
}

RenderStats Renderer::getStats() const
{
    RenderStats stats;
    stats.currentPass  = currentPass.load();
    stats.totalPasses  = settings.samplesPerPixel;
    stats.isComplete   = !isRendering.load();
    return stats;
}