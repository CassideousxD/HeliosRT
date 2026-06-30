#include "rendering/Presenter.h"
#include "rendering/FrameBuffer.h"
#include <vector>
#include <string>

#include <vector>

Presenter::Presenter()
    : window(nullptr),
      renderer(nullptr),
      texture(nullptr)
{
}

Presenter::~Presenter()
{
    shutdown();
}

bool Presenter::initialize(
    const RenderSettings& settings,
    const FrameBuffer& frameBuffer)
{
    Uint32 flags = 0;

    if (settings.fullscreen)
    {
        flags |= SDL_WINDOW_FULLSCREEN;
    }

    window = SDL_CreateWindow(
        settings.title.c_str(),
        frameBuffer.getWidth(),
        frameBuffer.getHeight(),
        flags);

    if (!window)
    {
        return false;
    }

    renderer = SDL_CreateRenderer(window, nullptr);

    if (!renderer)
    {
        return false;
    }

    texture = SDL_CreateTexture(
        renderer,
        SDL_PIXELFORMAT_RGBA8888,
        SDL_TEXTUREACCESS_STREAMING,
        frameBuffer.getWidth(),
        frameBuffer.getHeight());

    return texture != nullptr;
}

void Presenter::present(const FrameBuffer& frameBuffer)
{
    static std::vector<uint32_t> pixels(
        frameBuffer.getWidth() * frameBuffer.getHeight());

    for (int y = 0; y < frameBuffer.getHeight(); y++)
    {
        for (int x = 0; x < frameBuffer.getWidth(); x++)
        {
            const Vector3& pixel = frameBuffer.getPixel(x, y);

            uint8_t r = static_cast<uint8_t>(pixel.x * 255.999f);
            uint8_t g = static_cast<uint8_t>(pixel.y * 255.999f);
            uint8_t b = static_cast<uint8_t>(pixel.z * 255.999f);
            uint8_t a = 255;

            pixels[y * frameBuffer.getWidth() + x] =
                (static_cast<uint32_t>(r) << 24) |
                (static_cast<uint32_t>(g) << 16) |
                (static_cast<uint32_t>(b) << 8) |
                static_cast<uint32_t>(a);
        }
    }

    SDL_UpdateTexture(
        texture,
        nullptr,
        pixels.data(),
        frameBuffer.getWidth() * sizeof(uint32_t));

    SDL_RenderTexture(
        renderer,
        texture,
        nullptr,
        nullptr);

    SDL_RenderPresent(renderer);
}

void Presenter::updateTitle(const RenderStats& stats, const RenderSettings& settings)
{
    std::string title =
        stats.isComplete
        ? settings.title + " | Done | " + std::to_string(stats.totalPasses) + " passes"
        : settings.title
            + " | Pass " + std::to_string(stats.currentPass)
            + "/"        + std::to_string(stats.totalPasses)
            + " | "      + std::to_string(settings.threadCount) + " threads";

    SDL_SetWindowTitle(window, title.c_str());
}

void Presenter::shutdown()
{
    if (texture)
    {
        SDL_DestroyTexture(texture);
        texture = nullptr;
    }

    if (renderer)
    {
        SDL_DestroyRenderer(renderer);
        renderer = nullptr;
    }

    if (window)
    {
        SDL_DestroyWindow(window);
        window = nullptr;
    }
}