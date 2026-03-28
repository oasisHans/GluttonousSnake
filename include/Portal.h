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
        timer += 0.2; // 动画驱动

        int cx = pos.x * GRID_SIZE + GRID_SIZE / 2;
        int cy = pos.y * GRID_SIZE + GRID_SIZE / 2;

        // --- 第一层：深空背景光晕 ---
        for (int i = 5; i > 0; i--)
        {
            // 逐渐变淡的紫色光圈
            setfillcolor(RGB(20 * i, 0, 30 * i));
            solidcircle(cx, cy, (GRID_SIZE / 2) + i);
        }

        // --- 第二层：吸积盘漩涡粒子 (25个粒子) ---
        for (int i = 0; i < 25; i++)
        {
            // 每个粒子有自己的基础轨道
            double orbitRadius = (i % 3 + 1) * (GRID_SIZE / 8.0) + 2;

            // 内圈(i小)转得快，外圈(i大)转得慢
            double speed = 5.0 - (i * 0.15);
            double angle = timer * speed + (i * 6.28 / 20.0);

            // 计算粒子位置
            int px = cx + (int)(cos(angle) * orbitRadius);
            int py = cy + (int)(sin(angle) * orbitRadius);

            // 粒子颜色：紫色、青色、白色交替，营造星尘感
            if (i % 3 == 0)
                setfillcolor(RGB(255, 0, 255)); // 霓虹紫
            else if (i % 3 == 1)
                setfillcolor(RGB(0, 255, 255)); // 极光青
            else
                setfillcolor(WHITE); // 核心白

            // 粒子大小随呼吸起伏
            int pSize = (i % 2 == 0) ? 2 : 1;
            solidcircle(px, py, pSize);
        }

        // --- 第三层：事件视界 (Event Horizon) ---
        // 带有暗红边缘的纯黑核心
        setfillcolor(RGB(60, 0, 0));
        solidcircle(cx, cy, GRID_SIZE / 4 + 1);
        setfillcolor(BLACK);
        solidcircle(cx, cy, GRID_SIZE / 4);

        // --- 第四层：核心闪烁亮点 ---
        if ((int)(timer * 10) % 5 == 0)
        {
            setfillcolor(WHITE);
            solidcircle(cx, cy, 1);
        }
    }

    Point getDestination() const { return destination; }
};