#include "core/Application.h"
#include "core/RenderStats.h"
#include <string>
#include <iostream>
#include <SDL3/SDL.h>

Application::Application(const RenderSettings& settings)
    : settings(settings),
      renderer(this->settings),
      presenter(),
      isRunning(false)
{
}

Application::~Application()
{
    shutdown();
}

bool Application::initialize()
{
    if (!SDL_Init(SDL_INIT_VIDEO))
    {
        std::cerr << "SDL_Init failed: " << SDL_GetError() << std::endl;
        return false;
    }

    if (!presenter.initialize(settings, renderer.getFrameBuffer()))
    {
        std::cerr << "Presenter init failed: " << SDL_GetError() << std::endl;
        return false;
    }
    renderer.render();
    return true;
}

void Application::run()
{
    if (!initialize())
        return;

    isRunning = true;

    while (isRunning)
    {
        processEvents();
        update();
        render();
    }
}

void Application::processEvents()
{
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
            case SDL_EVENT_QUIT:
                isRunning = false;
                break;
            default:
                break;
        }
    }
}

void Application::update()
{
    RenderStats stats = renderer.getStats();

    if (stats.currentPass != lastDisplayedPass)
    {
        lastDisplayedPass = stats.currentPass;
        presenter.updateTitle(stats, settings);
    }
}

void Application::render()
{
    std::lock_guard<std::mutex> lock(renderer.frameMutex);
    presenter.present(renderer.getFrameBuffer());
}

void Application::shutdown()
{
    renderer.stop();
    presenter.shutdown();
    SDL_Quit();
}