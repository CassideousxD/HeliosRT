#pragma once

struct Tile
{
    int x;
    int y;
    int width;
    int height;
};

struct ScheduledTile
{
    Tile tile;
    int index;
};