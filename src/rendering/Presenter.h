#ifndef PRESENTER_H
#define PRESENTER_H
#include "core/RenderSettings.h"
#include "core/RenderStats.h"
#include <SDL3/SDL.h>

class FrameBuffer;

class Presenter
{
public:
    Presenter();
    ~Presenter();
    bool initialize( const RenderSettings& settings,const FrameBuffer& frameBuffer);
    void present(const FrameBuffer& frameBuffer);
    void updateTitle(const RenderStats& stats, const RenderSettings& settings);
    void shutdown();

private:
    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Texture* texture;
};

#endif