#pragma once
#include "Item.h"

class Obstacle : public Item
{
public:
    Obstacle(Point p) : Item(p, BLUE, ItemType::OBSTACLE) {}

    // 实现绘图函数
    virtual void draw() override
    {
        setfillcolor(color);
        fillrectangle(pos.x * GRID_SIZE, pos.y * GRID_SIZE,
                      (pos.x + 1) * GRID_SIZE, (pos.y + 1) * GRID_SIZE);
    }
};