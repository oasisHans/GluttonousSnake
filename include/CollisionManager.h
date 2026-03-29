#pragma once
#include "Snake.h"
#include "MapManager.h"

// 定义碰撞结果枚举
enum class CollisionResult
{
    NONE,
    WALL,
    SELF,
    FOOD,
    OBSTACLE,
    PORTAL,
    HALVEPOTION
    // 后面可以继续添加
};

class CollisionManager
{
public:
    static CollisionResult check(Point nextPos, const Snake &snake, const MapManager &map);
};