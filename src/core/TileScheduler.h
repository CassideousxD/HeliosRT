#pragma once
#include "core/Tile.h"
#include <vector>
#include <optional>
#include <atomic>

class TileScheduler
{
public:
    TileScheduler(int imageWidth, int imageHeight, int tileSize = 32);

    std::optional<ScheduledTile> getNextTile();
    void markConverged(int tileIndex);
    void reset();
    int totalTiles() const;

private:
    std::vector<Tile> tiles;
    std::vector<bool> convergedFlags;
    std::atomic<int> nextIndex;
};