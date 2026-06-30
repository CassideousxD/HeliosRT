#include "core/Application.h"
#include "core/RenderSettings.h"

int main()
{
    RenderSettings settings;

    settings.width = 800;
    settings.height = 600;
    settings.title = "Ray Tracer";
    settings.aperture = 0.0f;
    settings.focusDistance = 1.0f;

    Application application(settings);

    application.run();

    return 0;
}