#pragma once

struct RenderStats
{
    int currentPass = 0;
    int totalPasses = 0;
    bool isComplete = false;
};