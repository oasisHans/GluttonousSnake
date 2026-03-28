#include "CollisionManager.h"

CollisionResult CollisionManager::check(Point nextPos, const Snake &snake, const MapManager &map)
{
    // 1.撞墙情况
    if (nextPos.x < 0 || nextPos.y < 0 || nextPos.x >= GRID_W || nextPos.y >= GRID_H)
    {
        return CollisionResult::WALL;
    }

    // 2.撞自己
    const auto &body = snake.getBody();
    for (size_t i = 1; i < body.size(); ++i)
    {
        if (nextPos == body[i])
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
        case ItemType::PORTAL:
            return CollisionResult::PORTAL;
        }
    }

    return CollisionResult::NONE;
}