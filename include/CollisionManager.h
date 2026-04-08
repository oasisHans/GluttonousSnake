#pragma once
#include "Snake.h"
#include "MapManager.h"
#include "EnemySnake.h"

// 定义碰撞结果枚举
enum class CollisionResult
{
    NONE,
    WALL,
    SELF,
    OTHERSNAKE,
    OTHERSNAKE_HEAD,
    FOOD,
    OBSTACLE,
    PORTAL,
    HALVEPOTION
    // 后面可以继续添加
};

class CollisionManager
{

public:
    static CollisionResult check(
        Point nextPos,
        const Snake &currentSnake,
        const MapManager &map,
        const std::vector<Snake *> &allSnakes);
};