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

void Snake::move(bool Grow)
{
    Point newHead = this->getNextHead();
    this->body.push_front(newHead);

    // 若普通移动则删除尾部
    if (!Grow)
    {
        this->body.pop_back();
    }
}

void Snake::draw() const
{
    setfillcolor(this->color);
    for (const auto &p : body)
    {
        fillrectangle(p.x * GRID_SIZE, p.y * GRID_SIZE,
                      (p.x + 1) * GRID_SIZE, (p.y + 1) * GRID_SIZE);
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
    this->color = LIGHTGRAY;
}