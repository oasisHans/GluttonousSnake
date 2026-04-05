#include "EnemySnake.h"
#include <queue>
#include <map>

void EnemySnake::AI_handle(const MapManager &map, const Snake &player, const std::vector<Snake *> &allSnakes, int iqValue)
{
    static RandomGenerator localRng;

    if (localRng.chance(100 - iqValue))
    {
        int dx[] = {0, 0, -1, 1};
        int dy[] = {-1, 1, 0, 0};

        int order[] = {0, 1, 2, 3};
        for (int i = 3; i > 0; i--)
            std::swap(order[i], order[localRng.generate(0, i)]);

        for (int i = 0; i < 4; i++)
        {
            Point nextPos = {this->getHeadPos().x + dx[order[i]],
                             this->getHeadPos().y + dy[order[i]]};

            Direction tryDir;
            if (dx[order[i]] == 1)
                tryDir = Direction::RIGHT;
            else if (dx[order[i]] == -1)
                tryDir = Direction::LEFT;
            else if (dy[order[i]] == 1)
                tryDir = Direction::DOWN;
            else
                tryDir = Direction::UP;

            if (isCellSafe(nextPos, map, allSnakes, player))
            {
                this->setDirection(tryDir);
                return;
            }
        }
        return;
    }

    // BFS寻路
    Point Target;
    if (this->body.size() < (((player.getBody().size()) * 2) / 3))
    {
        Target = map.getNearestFoodPos(this->getHeadPos());
        if (Target.x == -1)
            return;
    }
    else
    {
        Target = player.getHeadPos();
    }

    if (Target.x < 0 || Target.x >= GRID_W || Target.y < 0 || Target.y >= GRID_H)
        return;

    Direction nextDir = findPath(Target, map, allSnakes, player);
    this->setDirection(nextDir);
}

Direction EnemySnake::findPath(Point Target, const MapManager &map, const std::vector<Snake *> &allSnakes, const Snake &player)
{
    Point StartPos = this->getHeadPos();
    if (StartPos == Target || (abs(StartPos.x - Target.x) + abs(StartPos.y - Target.y) <= 1))
    {
        return this->dir;
    }

    Point cameFrom[GRID_W][GRID_H];
    for (int i = 0; i < GRID_W; i++)
        for (int j = 0; j < GRID_H; j++)
            cameFrom[i][j] = Point(-1, -1);

    std::queue<Point> q;
    q.push(StartPos);
    cameFrom[StartPos.x][StartPos.y] = Point(-2, -2);

    bool found = false;
    int dx[] = {0, 0, -1, 1};
    int dy[] = {-1, 1, 0, 0};

    while (!q.empty())
    {
        Point curr = q.front();
        q.pop();

        if (curr == Target)
        {
            found = true;
            break;
        }

        for (int i = 0; i < 4; i++)
        {
            Point nextPos = {curr.x + dx[i], curr.y + dy[i]};
            if (nextPos.x >= 0 && nextPos.x < GRID_W &&
                nextPos.y >= 0 && nextPos.y < GRID_H)
            {
                if (cameFrom[nextPos.x][nextPos.y].x == -1)
                {
                    if (nextPos == Target || isCellSafe(nextPos, map, allSnakes, player))
                    {
                        cameFrom[nextPos.x][nextPos.y] = curr;
                        q.push(nextPos);

                        Item *item = map.getItemAt(nextPos);
                        if (item != nullptr && item->getType() == ItemType::PORTAL)
                        {
                            Portal *portal = static_cast<Portal *>(item);
                            Point dest = portal->getDestination();

                            if (dest.x >= 0 && dest.x < GRID_W &&
                                dest.y >= 0 && dest.y < GRID_H &&
                                cameFrom[dest.x][dest.y].x == -1)
                            {
                                // 传送门目的地的父节点记为传送门入口
                                cameFrom[dest.x][dest.y] = nextPos;
                                q.push(dest);
                            }
                        }
                    }
                }
            }
        }
    }

    if (found)
    {
        Point Step = Target;
        int safety = 0;
        while (safety < GRID_W * GRID_H)
        {
            Point parent = cameFrom[Step.x][Step.y];
            if (parent.x == -2 || parent == StartPos)
                break;
            if (parent.x == -1)
                break;
            Step = parent;
            safety++;
        }

        if (Step.x > StartPos.x)
            return Direction::RIGHT;
        if (Step.x < StartPos.x)
            return Direction::LEFT;
        if (Step.y > StartPos.y)
            return Direction::DOWN;
        if (Step.y < StartPos.y)
            return Direction::UP;
    }

    // fallback：找一个安全方向，优先保持原方向
    for (int i = 0; i < 4; i++)
    {
        Point nextPos = {StartPos.x + dx[i], StartPos.y + dy[i]};
        if (isCellSafe(nextPos, map, allSnakes, player))
        {
            Direction tryDir;
            if (dx[i] == 1)
                tryDir = Direction::RIGHT;
            else if (dx[i] == -1)
                tryDir = Direction::LEFT;
            else if (dy[i] == 1)
                tryDir = Direction::DOWN;
            else
                tryDir = Direction::UP;

            // 优先原方向
            if (tryDir == this->dir)
                return tryDir;
        }
    }
    // 原方向不行就找第一个安全方向
    for (int i = 0; i < 4; i++)
    {
        Point nextPos = {StartPos.x + dx[i], StartPos.y + dy[i]};
        if (isCellSafe(nextPos, map, allSnakes, player))
        {
            if (dx[i] == 1)
                return Direction::RIGHT;
            if (dx[i] == -1)
                return Direction::LEFT;
            if (dy[i] == 1)
                return Direction::DOWN;
            if (dy[i] == -1)
                return Direction::UP;
        }
    }

    return this->dir;
}

bool EnemySnake::isCellSafe(Point p, const MapManager &map, const std::vector<Snake *> &allSnakes, const Snake &player)
{
    if (p.x < 0 || p.x >= GRID_W || p.y < 0 || p.y >= GRID_H)
    {
        return false;
    }
    // 检查是否撞到障碍物
    Item *item = map.getItemAt(p);
    if (item != nullptr)
    {
        if (item->getType() == ItemType::OBSTACLE)
        {
            return false;
        }
    }

    // 检查是否撞到蛇身
    for (const auto *s : allSnakes)
    {
        if (s != this)
            continue;
        const auto &sBody = s->getBody();
        size_t startIdx = 1;
        for (size_t i = startIdx; i < sBody.size(); ++i)
        {
            if (p == sBody[i])
                return false;
        }
    }
    return true;
}