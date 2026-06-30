#pragma once
#include "core/Tile.h"
#include <vector>
#include <optional>
#include <atomic>

class TileScheduler
{
public:
    TileScheduler(int imageWidth, int imageHeight, int tileSize = 32);

    std::optional<Tile> getNextTile();
    void reset();
    int totalTiles() const;

private:
    std::vector<Tile> tiles;
    std::atomic<int> nextIndex;
};