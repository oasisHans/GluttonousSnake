#pragma once
#include "Common.h"

class Entity
{
protected:
    Point pos;
    COLORREF color;
    ItemType type;

public:
    // 构造函数
    Entity(Point p, COLORREF c, ItemType t)
        : pos(p), color(c), type(t) {}
    // 析构函数
    virtual ~Entity() {}

    virtual void draw() = 0;

    // 返回Entity坐标
    Point getPos() const { return pos; }
    // 返回Entity类型
    ItemType getType() const { return type; }
    // 设置Entity坐标
    void setPos(const Point &p)
    {
        pos = p;
    }
};