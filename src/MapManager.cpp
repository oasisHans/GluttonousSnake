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

void MapManager::GenerateFood(const std::vector<Snake *> &allSnakes)
{
    Point p;

    while (true)
    {
        bool overlap = false;
        p.x = rng.generate(0, GRID_W - 1);
        p.y = rng.generate(0, GRID_H - 1);

        for (const auto *s : allSnakes)
        {
            for (const auto &sBody : s->getBody())
            {
                if (p == sBody)
                {
                    overlap = true;
                    break;
                }
            }
            if (overlap)
                break;
        }

        for (const auto *s : allSnakes)
        {
            if (abs(p.x - s->getHeadPos().x) < DIS_MIN && abs(p.y - s->getHeadPos().y) < DIS_MIN)
            {
                overlap = true;
            }
        }
        if (!overlap)
        {
            break;
        }
    }
    warehouses[ItemType::FOOD].push_back(new Food(p));
}

void MapManager::GenerateObstacle(const std::vector<Snake *> &allSnakes, const ObstacleLevel &obsLevel)
{
    int num = 0;
    switch (obsLevel)
    {
    case ObstacleLevel::LOW:
        num = rng.generate(4, 6);
        break;
    case ObstacleLevel::NORMAL:
        num = rng.generate(10, 12);
        break;
    case ObstacleLevel::HIGH:
        num = rng.generate(20, 25);
        break;
    }
    for (int i = 0; i < num; i++)
    {
        Point p;
        int attempts = 0;
        while (attempts < 100)
        {
            attempts++;
            bool overlap = false;
            p.x = rng.generate(0, GRID_W - 1);
            p.y = rng.generate(0, GRID_H - 1);

            // 判断是否和蛇身重叠
            for (const auto *s : allSnakes)
            {
                for (const auto &sBody : s->getBody())
                {
                    if (p == sBody)
                    {
                        overlap = true;
                        break;
                    }
                }
                if (overlap)
                    break;
            }

            for (const auto *s : allSnakes)
            {
                if (abs(p.x - s->getHeadPos().x) < DIS_MIN && abs(p.y - s->getHeadPos().y) < DIS_MIN)
                {
                    overlap = true;
                }
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
        if (attempts < 100)
        {
            warehouses[ItemType::OBSTACLE].push_back(new Obstacle(p));
        }
    }
}

void MapManager::GeneratePortalPair(const std::vector<Snake *> &allSnakes)
{
    std::vector<Point> portalSite;
    for (int i = 0; i < 2; i++)
    {
        Point p;
        while (true)
        {

            bool overlap = false;
            p.x = rng.generate(0, GRID_W - 1);
            p.y = rng.generate(0, GRID_H - 1);

            // 判断是否和蛇身重叠
            for (const auto *s : allSnakes)
            {
                for (const auto &sBody : s->getBody())
                {
                    if (p == sBody)
                    {
                        overlap = true;
                        break;
                    }
                }
                if (overlap)
                    break;
            }

            for (const auto *s : allSnakes)
            {
                if (abs(p.x - s->getHeadPos().x) < DIS_MIN && abs(p.y - s->getHeadPos().y) < DIS_MIN)
                {
                    overlap = true;
                }
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

void MapManager::GenerateHalvePotion(const std::vector<Snake *> &allSnakes)
{
    Point p;
    int attempts = 0;
    while (attempts < 100)
    {
        attempts++;
        bool overlap = false;
        p.x = rng.generate(0, GRID_W - 1);
        p.y = rng.generate(0, GRID_H - 1);

        // 判断是否和蛇身重叠
        for (const auto *s : allSnakes)
        {
            for (const auto &sBody : s->getBody())
            {
                if (p == sBody)
                {
                    overlap = true;
                    break;
                }
            }
            if (overlap)
                break;
        }

        for (const auto *s : allSnakes)
        {
            if (abs(p.x - s->getHeadPos().x) < DIS_MIN && abs(p.y - s->getHeadPos().y) < DIS_MIN)
            {
                overlap = true;
            }
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
    warehouses[ItemType::HALVEPOTION].push_back(new HalvePotion(p));
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

Point MapManager::getFoodPos() const
{
    auto it = warehouses.find(ItemType::FOOD);
    if (it != warehouses.end() && !it->second.empty())
    {
        return it->second[0]->getPos();
    }
    return Point(-1, -1);
}

void MapManager::addFood(Point p)
{
    warehouses[ItemType::FOOD].push_back(new Food(p));
}

int MapManager::getItemCount(ItemType type) const
{
    auto it = warehouses.find(type);
    if (it != warehouses.end())
    {
        return static_cast<int>(it->second.size());
    }
    return 0;
}

Point MapManager::getNearestFoodPos(Point from) const
{
    auto it = warehouses.find(ItemType::FOOD);
    if (it == warehouses.end() || it->second.empty())
        return Point(-1, -1);

    Point nearest(-1, -1);
    int minDist = INT_MAX;

    for (const auto *item : it->second)
    {
        Point p = item->getPos();
        int dist = abs(p.x - from.x) + abs(p.y - from.y);
        if (dist < minDist)
        {
            minDist = dist;
            nearest = p;
        }
    }
    return nearest;
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
