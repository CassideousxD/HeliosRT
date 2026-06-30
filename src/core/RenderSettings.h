#ifndef RENDERSETTINGS_H
#define RENDERSETTINGS_H
#include <string>
#include <thread>

struct RenderSettings
{
    // Window
    int width = 800;
    int height = 600;
    std::string title = "Ray Tracer";
    bool fullscreen = false;
    bool vsync = true;
    // Renderer
    int samplesPerPixel = 100;
    int maxRayDepth = 10;
    // Camera
    float verticalFov = 90.0f;
    float aperture = 0.005f;//DepthOfField
    float focusDistance = 1.0f;
    float shutterOpen  = 0.0f;
    float shutterClose = 1.0f;
    //Multi-Threading
    int threadCount = std::thread::hardware_concurrency();
    //Denoiser
    bool enableDenoiser = true;
};

#endif