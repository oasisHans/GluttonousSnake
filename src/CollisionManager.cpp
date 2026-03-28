#include "CollisionManager.h"

CollisionResult CollisionManager::check(Point nextPos, const Snake &snake, const MapManager &map)
{
    // 1.撞墙情况
    if (nextPos.x < 0 || nextPos.y < 0 || nextPos.x >= GRID_W || nextPos.y >= GRID_H)
    {
        return CollisionResult::WALL;
    }

    // 2.撞自己
    for (const auto &p : snake.getBody())
    {
        if (nextPos == p)
        {
            return CollisionResult::SELF;
        }
    }

    // 3.撞Entity
    Item *item = map.getItemAt(nextPos);
    if (item != nullptr)
    {
        switch (item->getType())
        {
        case ItemType::FOOD:
            return CollisionResult::FOOD;
        case ItemType::OBSTACLE:
            return CollisionResult::OBSTACLE;
        }
    }

    return CollisionResult::NONE;
}