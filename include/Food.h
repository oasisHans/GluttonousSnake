#pragma once
#include "Item.h"

class Food : public Item
{
public:
    Food(Point p) : Item(p, RED, ItemType::FOOD) {}

    // 实现绘图函数
    virtual void draw() override
    {
        setfillcolor(color);
        fillrectangle(pos.x * GRID_SIZE, pos.y * GRID_SIZE,
                      (pos.x + 1) * GRID_SIZE, (pos.y + 1) * GRID_SIZE);
    }
};