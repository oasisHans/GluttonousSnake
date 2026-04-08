#include "CollisionManager.h"

CollisionResult CollisionManager::check(Point nextPos,
                                        const Snake &currentSnake,
                                        const MapManager &map,
                                        const std::vector<Snake *> &allSnakes)
{
    // 1.撞墙情况
    if (nextPos.x < 0 || nextPos.y < 0 || nextPos.x >= GRID_W || nextPos.y >= GRID_H)
    {
        return CollisionResult::WALL;
    }

    // 2.撞蛇
    for (const auto *s : allSnakes)
    {
        const auto &body = s->getBody();
        size_t startIdx = (s == &currentSnake) ? 1 : 0;
        for (size_t i = startIdx; i < body.size(); ++i)
        {
            if (nextPos == body[i])
            {
                if (s == &currentSnake)
                    return CollisionResult::SELF;

                // 碰到其他蛇：区分头部(i==0)和身体
                if (i == 0)
                    return CollisionResult::OTHERSNAKE_HEAD;
                else
                    return CollisionResult::OTHERSNAKE;
            }
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
        case ItemType::HALVEPOTION:
            return CollisionResult::HALVEPOTION;
        }
    }

    return CollisionResult::NONE;
}