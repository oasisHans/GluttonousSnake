#include "MapManager.h"
#include "Food.h"

MapManager::MapManager() {}

void MapManager::clearAll()
{
    for (auto &pair : warehouses)
    {
        for (Item *item : pair.second)
        {
            delete item; // 释放内存
        }
        pair.second.clear(); // 清空 vector
    }
    warehouses.clear(); // 清空 map
}

MapManager::~MapManager()
{
    clearAll();
}

void MapManager::GenerateFood(const Snake &snake)
{
    Point p;
    Point snakeHead = snake.getHeadPos();

    while (true)
    {
        bool overlap = false;
        p.x = rng.generate(0, GRID_W - 1);
        p.y = rng.generate(0, GRID_H - 1);

        // 判断是否和蛇身重叠
        for (const auto &bodyPart : snake.getBody())
        {
            if (p == bodyPart)
            {
                overlap = true;
                break;
            }
        }
        if (abs(p.x - snakeHead.x) < DIS_MIN && abs(p.y - snakeHead.y) < DIS_MIN)
        {
            overlap = true;
        }
        if (!overlap)
        {
            break;
        }
    }
    warehouses[ItemType::FOOD].push_back(new Food(p));
}

void MapManager::GenerateObstacle(const Snake &snake, const ObstacleLevel &obsLevel)
{
    int num = 0;
    switch (obsLevel)
    {
    case ObstacleLevel::LOW:
        num = rng.generate(3, 5);
        break;
    case ObstacleLevel::NORMAL:
        num = rng.generate(9, 12);
        break;
    case ObstacleLevel::HIGH:
        num = rng.generate(20, 26);
        break;
    }
    Point snakeHead = snake.getHeadPos();
    for (int i = 0; i < num; i++)
    {
        Point p;
        while (true)
        {
            bool overlap = false;
            p.x = rng.generate(0, GRID_W - 1);
            p.y = rng.generate(0, GRID_H - 1);

            // 判断是否和蛇身重叠
            for (const auto &bodyPart : snake.getBody())
            {
                if (p == bodyPart)
                {
                    overlap = true;
                    break;
                }
            }
            if (abs(p.x - snakeHead.x) < DIS_MIN && abs(p.y - snakeHead.y) < DIS_MIN)
            {
                overlap = true;
            }
            for (const auto &pair : warehouses)
            {
                for (Item *item : pair.second)
                {
                    if (p == item->getPos())
                    {
                        overlap = true;
                        break;
                    }
                    if (overlap)
                        break;
                }
            }
            if (!overlap)
            {
                break;
            }
        }
        warehouses[ItemType::OBSTACLE].push_back(new Obstacle(p));
    }
}

void MapManager::drawAll() const
{
    for (const auto &pair : warehouses)
    {
        for (Item *item : pair.second)
        {
            item->draw();
        }
    }
}

Item *MapManager::getItemAt(Point p) const
{
    for (const auto &pair : warehouses)
    {
        for (Item *item : pair.second)
        {
            if (p == item->getPos())
            {
                return item;
            }
        }
    }
    return nullptr;
}

void MapManager::removeItem(Item *item)
{
    if (item == nullptr)
        return;
    auto &drawer = warehouses[item->getType()];
    for (auto it = drawer.begin(); it != drawer.end(); ++it)
    {
        if (*it == item)
        {
            delete *it;
            drawer.erase(it);
            return;
        }
    }
}

void MapManager::removeTypeAll(ItemType itemtype)
{
    auto &drawer = warehouses[itemtype];
    for (Item *item : drawer)
    {
        delete item;
    }
    drawer.clear();
}
