#pragma once
#include "Item.h"
#include <cmath>

class Portal : public Item
{
private:
    Point destination;
    double timer;

public:
    Portal(Point p, Point destination) : Item(p, MAGENTA, ItemType::PORTAL)
    {
        this->destination = destination;
        this->timer = 0;
    }

    // 实现绘图函数
    virtual void draw() override
    {
        timer += -0.30;

        int cx = pos.x * GRID_SIZE + GRID_SIZE / 2;
        int cy = pos.y * GRID_SIZE + GRID_SIZE / 2;

        setfillcolor(RGB(15, 0, 30));
        solidcircle(cx, cy, GRID_SIZE * 0.8);

        for (int arm = 0; arm < 2; arm++)
        {
            double armOffset = arm * 3.14159;

            for (int i = 0; i < 80; i++)
            {
                double r = (i / 80.0) * (GRID_SIZE * 0.95);

                double spiralShape = r * 0.5;
                double rotation = timer;

                double wave = 0.1 * sin(r * 0.8 - timer * 3);
                double angle = rotation + spiralShape + armOffset + wave;

                int px = cx + (int)(cos(angle) * r);
                int py = cy + (int)(sin(angle) * r);

                if (i < 15)
                {
                    setfillcolor(WHITE);
                    solidrectangle(px, py, px + 2, py + 2);
                }
                else
                {
                    int purple = 255 - i * 2;
                    int blue = i * 3;
                    setfillcolor(RGB(purple > 0 ? purple : 0, 50, blue < 255 ? blue : 255));
                    solidrectangle(px, py, px + 1, py + 1);
                }
            }
        }

        for (int k = 0; k < 12; k++)
        {

            double r = GRID_SIZE * (0.8 + (rand() % 30) / 100.0);

            double fastAngle = timer * 2.5 + (k * 6.28 / 12.0) + (rand() % 10 / 10.0);

            int fx = cx + (int)(cos(fastAngle) * r);
            int fy = cy + (int)(sin(fastAngle) * r);

            if (rand() % 5 > 1)
            {
                setfillcolor(WHITE);
                solidrectangle(fx, fy, fx + 1, fy + 1);
            }
        }

        setfillcolor(WHITE);
        solidcircle(cx, cy, GRID_SIZE / 12 + 1);
        setfillcolor(BLACK);
        solidcircle(cx, cy, GRID_SIZE / 12);

        setfillcolor(RGB(200, 200, 255));
        for (int j = 0; j < 3; j++)
        {
            int lx = cx - 5 + rand() % 10;
            int ly = cy + (int)(2 * cos(timer * 15 + j));
            solidrectangle(lx, ly, lx + 1, ly + 1);
        }
    }

    Point getDestination() const { return destination; }
};