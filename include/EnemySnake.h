#pragma once
#include "Snake.h"
#include "MapManager.h"
#include <vector>

class EnemySnake : public Snake
{
public:
    EnemySnake(Point startPos, Direction startDir, COLORREF col)
        : Snake(startPos, startDir, col) {}

    void AI_handle(const MapManager &map, const Snake &player, const std::vector<Snake *> &allSnakes, int iqValue);

    Direction findPath(Point Target, const MapManager &map, const std::vector<Snake *> &allSnakes, const Snake &player);

    bool isCellSafe(Point p, const MapManager &map, const std::vector<Snake *> &allSnakes, const Snake &player);
};
