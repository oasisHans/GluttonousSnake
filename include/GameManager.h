#pragma once
#include "Common.h"
#include "MapManager.h"
#include "Item.h"
#include "Food.h"
#include "Snake.h"
#include "PlayerSnake.h"
#include "CollisionManager.h"
#include "InputHandler.h"
#include "Render.h"
#include "GameSettings.h"
#include "RecordManager.h"
#include "EnemySnake.h"

class GameManager
{
private:
    PlayerSnake *snake;
    MapManager *map;
    std::vector<EnemySnake *> enemies;

    InputHandler input;
    GameState gamestate;
    Render renderer;
    GameSettings gamesettings;
    RecordManager record;

    int score;
    RandomGenerator rng;
    int lastGoatBeforeGame;

public:
    GameManager();
    ~GameManager();

    void run();

    // 内部逻辑分块
    void initGame();      // 初始化/重置游戏
    void updatePlaying(); // 处理游戏运行逻辑
};