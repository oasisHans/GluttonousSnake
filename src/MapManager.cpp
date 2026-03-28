#include "MapManager.h"

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
        num = rng.generate(5, 8);
        break;
    case ObstacleLevel::NORMAL:
        num = rng.generate(14, 18);
        break;
    case ObstacleLevel::HIGH:
        num = rng.generate(25, 30);
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

void MapManager::GeneratePortalPair(const Snake &snake)
{
    std::vector<Point> portalSite;
    Point snakeHead = snake.getHeadPos();
    for (int i = 0; i < 2; i++)
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
            if (!portalSite.empty())
            {
                const auto it = portalSite.begin();
                if ((abs(p.x - (*it).x) < 6) && (abs(p.y - (*it).y) < 6))
                {
                    overlap = true;
                }
            }
            if (!overlap)
            {
                break;
            }
        }
        portalSite.push_back(p);
    }
    Point p1 = portalSite[0];
    Point p2 = portalSite[1];

    Portal *portalA = new Portal(p1, p2);
    Portal *portalB = new Portal(p2, p1);

    warehouses[ItemType::PORTAL].push_back(portalA);
    warehouses[ItemType::PORTAL].push_back(portalB);
    portalSite.clear();
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
