#include "GameManager.h"

GameManager::GameManager()
    : snake(nullptr),
      map(nullptr),
      score(0),
      gamestate(GameState::Start) {}

GameManager::~GameManager()
{
    if (snake != nullptr)
    {
        delete snake;
        snake = nullptr;
    }
    if (map != nullptr)
    {
        delete map;
        map = nullptr;
    }
}

void GameManager::initGame()
{
    // 清除旧游戏数据
    if (snake != nullptr)
    {
        delete snake;
        snake = nullptr;
    }
    if (map != nullptr)
    {
        map->clearAll();
        delete map;
        map = nullptr;
    }

    // 初始化分数
    this->score = 0;
    // 创建地图
    this->map = new MapManager();
    // 创建Snake
    this->snake = new PlayerSnake(Point(GRID_W / 2, GRID_H / 2), Direction::RIGHT, GREEN);

    this->map->GenerateFood(*(this->snake));
    this->map->GenerateObstacle(*(this->snake), this->gamesettings.getObstacleLevel());
}

void GameManager::updatePlaying()
{
    if (input.isKeyPressed(VK_SPACE))
    {
        gamestate = GameState::Paused;
        return;
    }

    snake->HandleInput(input);

    Point nextHead = snake->getNextHead();

    CollisionResult collide_result = CollisionManager::check(nextHead, *snake, *map);

    switch (collide_result)
    {
    // 正常移动
    case CollisionResult::NONE:
        snake->move(false);
        break;
    // 吃到食物增长
    case CollisionResult::FOOD:
    {
        snake->move(true);
        this->score++;
        map->removeTypeAll(ItemType::FOOD);
        map->removeTypeAll(ItemType::OBSTACLE);
        map->GenerateFood(*snake);
        map->GenerateObstacle(*snake, gamesettings.getObstacleLevel());
        break;
    }
    // 死亡
    case CollisionResult::WALL:
    case CollisionResult::SELF:
    case CollisionResult::OBSTACLE:
        snake->die();
        renderer.render(gamestate, snake, map, score, gamesettings);
        FlushBatchDraw();
        Sleep(1000);
        this->gamestate = GameState::GameOver;
    }
}

// 游戏运行总逻辑
void GameManager::run()
{
    while (true)
    {
        input.updateKey();

        switch (gamestate)
        {
        case GameState::Start:
            if (input.isKeyPressed(VK_SPACE))
            {
                gamestate = GameState::Set;
                gamesettings.reset();
            }
            break;
        case GameState::Set:
        {
            int choice = input.getNumberInput();
            if (choice != -1)
            {
                gamesettings.setGame(choice);

                if (gamesettings.isSetDone())
                {
                    initGame();
                    gamestate = GameState::Playing;
                }
            }
            break;
        }
        case GameState::Playing:
            updatePlaying();
            break;
        case GameState::Paused:
            if (input.isKeyPressed(VK_SPACE))
                gamestate = GameState::Playing;
            break;
        case GameState::GameOver:
        {
            int choice = input.getNumberInput();
            if (choice == 1)
            {
                gamestate = GameState::Start;
            }
            else if (choice == 0)
            {
                return;
            }
            break;
        }
        }

        renderer.render(gamestate, snake, map, score, gamesettings);

        if (gamestate == GameState::Playing)
        {
            Sleep(gamesettings.PlayDelay()); // 游戏时，按蛇的速度走
        }
        else
        {
            Sleep(20);
        }
    }
}