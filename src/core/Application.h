#ifndef APPLICATION_H
#define APPLICATION_H
#include "core/Renderer.h"
#include "core/RenderSettings.h"
#include "rendering/Presenter.h"
#include <SDL3/SDL.h>

class Application
{
public:
    Application(const RenderSettings& settings);
    ~Application();
    void run();

private:
    bool initialize();
    void processEvents();
    void update();
    void render();
    void shutdown();

    RenderSettings settings; 
    Renderer renderer;
    Presenter presenter;
    bool isRunning;
    int lastDisplayedPass = -1;
};

#endif