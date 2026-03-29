#pragma once
#include <iostream>
#include <easyx.h>
#include <vector>
#include <deque>
#include <random>
#include <math.h>
#include <fstream>
#include <algorithm>

// 游戏窗口尺寸
constexpr int WIDTH = 640;
constexpr int HEIGHT = 480;

// 网格设置
constexpr int GRID_SIZE = 20;
constexpr int GRID_W = (WIDTH / GRID_SIZE);
constexpr int GRID_H = (HEIGHT / GRID_SIZE);

// 帧率设置（速度设置）
constexpr int DELAY_SLOW = 180;
constexpr int DELAY_NORMAL = 90;
constexpr int DELAY_FAST = 40;

constexpr int DIS_MIN = 3;

// 传送门生成概率（%）
constexpr int PortalChance = 100;

// 减半药水生成概率（%）
constexpr int HalveChance = 50;

// Snake方向枚举
enum class Direction
{
    UP,
    DOWN,
    LEFT,
    RIGHT
};

// Item类型枚举
enum class ItemType
{
    FOOD,
    OBSTACLE,
    PORTAL,
    HALVEPOTION
    // 未完待续后续版本添加
};

// 游戏进行状态
enum class GameState
{
    Start,
    Set,
    History,
    Playing,
    Paused,
    GameOver
};

// 设置内容枚举
enum class SetType
{
    Speed,
    Obastacle,
    Final
};

// 设置速度枚举
enum class Speed
{
    SLOW,
    NORMAL,
    FAST
};

// 设置障碍物数量枚举
enum class ObstacleLevel
{
    LOW,
    NORMAL,
    HIGH
};

// 基础坐标结构体
struct Point
{
    int x;
    int y;
    // Point构造函数
    Point(int _x = 0, int _y = 0) : x(_x), y(_y) {}

    // 重载"=="判断坐标是否重合
    bool operator==(const Point &other) const
    {
        return x == other.x && y == other.y;
    }
};

// 随机数工具
class RandomGenerator
{
private:
    std::mt19937 gen;

public:
    RandomGenerator() : gen(std::random_device{}()) {}
    // 随机数生成
    int generate(int min, int max)
    {
        std::uniform_int_distribution<> distrib(min, max);
        return distrib(gen);
    }

    bool chance(int odds)
    {
        int num = generate(0, 99);
        if (num < odds)
        {
            return true;
        }
        return false;
    }
};