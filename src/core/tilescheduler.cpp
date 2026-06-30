#include "core/TileScheduler.h"

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
}

std::optional<Tile> TileScheduler::getNextTile()
{
    int index = nextIndex.fetch_add(1);
    if (index >= static_cast<int>(tiles.size()))
    {
        return std::nullopt;
    }
    return tiles[index];
}

void TileScheduler::reset()
{
    nextIndex = 0;
}

int TileScheduler::totalTiles() const
{
    return static_cast<int>(tiles.size());
}