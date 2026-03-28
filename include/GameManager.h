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

class GameManager
{
private:
    PlayerSnake *snake;
    MapManager *map;
    InputHandler input;
    GameState gamestate;
    Render renderer;
    GameSettings gamesettings;

    int score;

public:
    GameManager();
    ~GameManager();

    void run();

    // 内部逻辑分块
    void initGame();      // 初始化/重置游戏
    void updatePlaying(); // 处理游戏运行逻辑
};