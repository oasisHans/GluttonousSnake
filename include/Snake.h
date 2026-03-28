#pragma once
#include "Common.h"
#include <deque>

class Snake
{
protected:
    std::deque<Point> body;
    COLORREF color;
    Direction dir;
    bool isAlive;

public:
    // 构造函数
    Snake(Point startPos, Direction startDir, COLORREF col);
    // 析构函数
    virtual ~Snake() {}

    Point getNextHead() const;

    virtual void move(bool Grow);

    virtual void draw() const;

    void setDirection(Direction newDir);

    // 返回参数
    Point getHeadPos() const { return body.front(); }
    // 返回整个蛇身
    const std::deque<Point> &getBody() const { return body; }
    // 检查是否存活
    bool checkAlive() const { return isAlive; }
    void die();
};