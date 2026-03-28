#include "Snake.h"
#include "Common.h"

Snake::Snake(Point startPos, Direction startDir, COLORREF col)
    : dir(startDir), color(col), isAlive(true)
{
    // 设置初始Snake
    body.push_front(startPos);
    body.push_back(Point(startPos.x - 1, startPos.y));
    body.push_back(Point(startPos.x - 2, startPos.y));
}

Point Snake::getNextHead() const
{
    Point NextHead = this->body.front();
    switch (this->dir)
    {
    case Direction::UP:
        NextHead.y--;
        break;
    case Direction::DOWN:
        NextHead.y++;
        break;
    case Direction::LEFT:
        NextHead.x--;
        break;
    case Direction::RIGHT:
        NextHead.x++;
        break;
    }
    return NextHead;
}

void Snake::throughWall()
{
    Point newHead = this->getHeadPos();
    switch (this->dir)
    {
    case Direction::UP:
        newHead.y = GRID_H - 1;
        break;
    case Direction::DOWN:
        newHead.y = 0;
        break;
    case Direction::LEFT:
        newHead.x = GRID_W - 1;
        break;
    case Direction::RIGHT:
        newHead.x = 0;
        break;
    }
    this->body.push_front(newHead);
    this->body.pop_back();
}

void Snake::moveDirect(Point destination, bool Grow)
{
    this->body.push_front(destination);
    if (!Grow)
    {
        this->body.pop_back();
    }
}

Point Snake::getWrappedPos(Point p) const
{
    if (p.x < 0)
        p.x = GRID_W - 1;
    else if (p.x >= GRID_W)
        p.x = 0;

    if (p.y < 0)
        p.y = GRID_H - 1;
    else if (p.y >= GRID_H)
        p.y = 0;

    return p;
}

void Snake::draw() const
{
    setfillcolor(WHITE);
    for (const auto &p : body)
    {
        solidrectangle(p.x * GRID_SIZE - 1,
                       p.y * GRID_SIZE - 1,
                       (p.x + 1) * GRID_SIZE + 1,
                       (p.y + 1) * GRID_SIZE + 1);
    }

    setfillcolor(this->color);
    for (const auto &p : body)
    {
        solidrectangle(p.x * GRID_SIZE,
                       p.y * GRID_SIZE,
                       (p.x + 1) * GRID_SIZE,
                       (p.y + 1) * GRID_SIZE);
    }
}

void Snake::setDirection(Direction newDir)
{
    if ((dir == Direction::UP && newDir == Direction::DOWN) ||
        (dir == Direction::DOWN && newDir == Direction::UP) ||
        (dir == Direction::LEFT && newDir == Direction::RIGHT) ||
        (dir == Direction::RIGHT && newDir == Direction::LEFT))
    {
        return;
    }
    this->dir = newDir;
}

void Snake::die()
{
    isAlive = false;

    setfillcolor(WHITE);
    for (const auto &p : body)
    {
        solidrectangle(p.x * GRID_SIZE - 1,
                       p.y * GRID_SIZE - 1,
                       (p.x + 1) * GRID_SIZE + 1,
                       (p.y + 1) * GRID_SIZE + 1);
    }
    this->color = LIGHTGRAY;
}