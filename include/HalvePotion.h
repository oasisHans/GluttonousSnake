#pragma once
#include "Item.h"
#include <cmath>

class HalvePotion : public Item
{
private:
    double timer;

public:
    HalvePotion(Point p) : Item(p, WHITE, ItemType::HALVEPOTION)
    {
        this->timer = 0;
    }

    // 实现绘图函数
    virtual void draw() override
    {
        timer += 0.1;

        int offsetY = (int)(3 * sin(timer));

        int cx = pos.x * GRID_SIZE + GRID_SIZE / 2;
        int cy = pos.y * GRID_SIZE + GRID_SIZE / 2 + offsetY;

        setfillcolor(RGB(0, 50, 50));
        solidcircle(cx, cy + 2, GRID_SIZE / 2);

        setfillcolor(RGB(0, 200, 255));

        int alpha = 150 + (int)(50 * sin(timer * 2));
        setfillcolor(RGB(0, alpha, alpha + 50));
        solidcircle(cx, cy + 2, GRID_SIZE / 3);

        setfillcolor(WHITE);
        solidrectangle(cx - 3, cy - GRID_SIZE / 2 + 2, cx + 3, cy);

        setfillcolor(RGB(150, 75, 0));
        solidrectangle(cx - 4, cy - GRID_SIZE / 2, cx + 4, cy - GRID_SIZE / 2 + 3);
        for (int i = 0; i < 2; i++)
        {
            double bubbleTimer = timer + (i * 1.5);
            int bx = cx + (int)(5 * cos(bubbleTimer * 2 + i));
            int by = cy + 5 - ((int)(bubbleTimer * 10) % 15);

            setfillcolor(WHITE);
            solidcircle(bx, by, 1);
        }
        setfillcolor(RGB(255, 255, 255));
        solidcircle(cx - 3, cy - 2, 2);
    }
};