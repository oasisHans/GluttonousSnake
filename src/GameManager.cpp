#include "GameManager.h"

GameManager::GameManager()
    : snake(nullptr),
      map(nullptr),
      score(0),
      gamestate(GameState::Start)
{
    this->record.loadRecord();
}

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

    Point finalPos = snake->getNextHead();
    bool resolved = false;

    while (!resolved)
    {
        CollisionResult res = CollisionManager::check(finalPos, *snake, *map);

        switch (res)
        {
        case CollisionResult::WALL:
            finalPos = snake->getWrappedPos(finalPos);
            break;
        case CollisionResult::PORTAL:
        {
            Portal *p = static_cast<Portal *>(map->getItemAt(finalPos));
            finalPos = p->getDestination();
            map->removeTypeAll(ItemType::PORTAL);
            break;
        }
        default:
            resolved = true;
        }
    }

    CollisionResult finalRes = CollisionManager::check(finalPos, *snake, *map);

    switch (finalRes)
    {
    case CollisionResult::NONE:
        snake->moveDirect(finalPos, false);
        break;

    case CollisionResult::FOOD:
        snake->moveDirect(finalPos, true);
        this->score++;

        // 清空旧地图
        map->removeTypeAll(ItemType::FOOD);
        map->removeTypeAll(ItemType::OBSTACLE);
        map->removeTypeAll(ItemType::PORTAL);

        // 生成下一轮地图
        map->GenerateFood(*snake);
        map->GenerateObstacle(*snake, gamesettings.getObstacleLevel());
        if (this->rng.chance(PortalChance))
        {
            map->GeneratePortalPair(*snake);
        }
        break;

    case CollisionResult::SELF:
    case CollisionResult::OBSTACLE:
        snake->die();
        renderer.render(gamestate, snake, map, score, gamesettings, record);
        FlushBatchDraw();
        Sleep(1000);
        this->gamestate = GameState::GameOver;
        break;
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
            else if (input.isKeyPressed(0x48))
            {
                gamestate = GameState::History;
            }
            break;
        case GameState::History:
            if (input.isKeyPressed(VK_SPACE))
            {
                gamestate = GameState::Set;
                gamesettings.reset();
            }
            else if (input.isKeyPressed(0x30) || input.isKeyPressed(0x60))
            {
                gamestate = GameState::Start;
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
                initGame();
                record.saveRecord(gamesettings, this->score);
                record.loadRecord();
                gamestate = GameState::Playing;
            }
            else if (choice == 2)
            {
                record.saveRecord(gamesettings, this->score);
                record.loadRecord();
                gamestate = GameState::Start;
            }
            else if (choice == 0)
            {
                record.saveRecord(gamesettings, this->score);
                return;
            }
            break;
        }
        }

        renderer.render(gamestate, snake, map, score, gamesettings, record);

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