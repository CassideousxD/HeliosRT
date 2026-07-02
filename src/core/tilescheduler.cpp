#include "core/TileScheduler.h"
#include <algorithm>

TileScheduler::TileScheduler(int imageWidth, int imageHeight, int tileSize)
    : nextIndex(0)
{
    for (int y = 0; y < imageHeight; y += tileSize)
    {
        for (int x = 0; x < imageWidth; x += tileSize)
        {
            Tile tile;
            tile.x      = x;
            tile.y      = y;
            tile.width  = std::min(tileSize, imageWidth  - x);
            tile.height = std::min(tileSize, imageHeight - y);
            tiles.push_back(tile);
        }
    }
    convergedFlags.assign(tiles.size(), false);
}

std::optional<ScheduledTile> TileScheduler::getNextTile()
{
    while (true)
    {
        int index = nextIndex.fetch_add(1);
        if (index >= static_cast<int>(tiles.size()))
            return std::nullopt;

        if (!convergedFlags[index])
            return ScheduledTile{ tiles[index], index };
    }
}

void TileScheduler::markConverged(int tileIndex)
{
    convergedFlags[tileIndex] = true;
}

void TileScheduler::reset()
{
    nextIndex = 0;
}

int TileScheduler::totalTiles() const
{
    return static_cast<int>(tiles.size());
}