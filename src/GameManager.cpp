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

    for (auto e : enemies)
        delete e;
    enemies.clear();

    // 初始化分数
    this->score = 0;
    // 初始化最高分
    this->lastGoatBeforeGame = this->record.getLastGoat();
    // 创建地图
    this->map = new MapManager();
    // 创建Snake
    this->snake = new PlayerSnake(Point(GRID_W / 2, GRID_H / 2), Direction::RIGHT, GREEN);

    std::vector<Snake *> tempNames;
    tempNames.push_back(this->snake);
    this->map->GenerateFood(tempNames);
    this->map->GenerateObstacle(tempNames, this->gamesettings.getObstacleLevel());
}

void GameManager::updatePlaying()
{

    std::vector<Snake *> allSnakes;
    if (snake)
    {
        allSnakes.push_back(snake);
    }

    for (auto e : enemies)
    {
        allSnakes.push_back(e);
    }

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
        CollisionResult res = CollisionManager::check(finalPos, *snake, *map, allSnakes);

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

    CollisionResult finalRes = CollisionManager::check(finalPos, *snake, *map, allSnakes);

    switch (finalRes)
    {
    case CollisionResult::NONE:
        snake->moveDirect(finalPos, false, false);
        break;

    case CollisionResult::FOOD:
    {
        snake->moveDirect(finalPos, true, false);
        this->score++;

        // 清空旧地图
        Item *targetFood = map->getItemAt(finalPos);
        map->removeItem(targetFood);

        map->removeTypeAll(ItemType::PORTAL);
        map->removeTypeAll(ItemType::HALVEPOTION);

        if (score >= 5 && (enemies.size() < 1))
        {
            enemies.push_back(new EnemySnake(Point(1, 1), Direction::RIGHT, YELLOW));
        }
        if (score >= 15 && (enemies.size() < 2))
        {
            enemies.push_back(new EnemySnake(Point(1, 1), Direction::RIGHT, YELLOW));
        }
        allSnakes.clear();
        allSnakes.push_back(this->snake);
        for (auto e : enemies)
            allSnakes.push_back(e);

        // 生成下一轮地图
        if (map->getItemCount(ItemType::FOOD) < 2)
        {
            map->GenerateFood(allSnakes);
        }

        if (this->rng.chance(PortalChance))
        {
            map->GeneratePortalPair(allSnakes);
        }
        if (this->rng.chance(HalveChance))
        {
            map->GenerateHalvePotion(allSnakes);
        }
        break;
    }
    case CollisionResult::HALVEPOTION:
        snake->moveDirect(finalPos, false, true);
        map->removeTypeAll(ItemType::HALVEPOTION);
        break;

    case CollisionResult::SELF:
    case CollisionResult::OBSTACLE:
    case CollisionResult::OTHERSNAKE:
        record.last_Goat = record.getLastGoat();
        snake->die();
        renderer.render(gamestate, snake, map, score, gamesettings, record, enemies);
        FlushBatchDraw();
        Sleep(1000);
        this->gamestate = GameState::GameOver;
        break;
    }
    allSnakes.clear();
    allSnakes.push_back(snake);
    for (auto e : enemies)
        allSnakes.push_back(e);

    // enemy蛇逻辑
    for (auto it = enemies.begin(); it != enemies.end();)
    {
        EnemySnake *e = *it;
        e->AI_handle(*map, *snake, allSnakes, gamesettings.getIQValue());
        Point ePos = e->getNextHead();
        bool eResolved = false;

        while (!eResolved)
        {
            CollisionResult res = CollisionManager::check(ePos, *e, *map, allSnakes);

            switch (res)
            {
            case CollisionResult::WALL:
                ePos = e->getWrappedPos(ePos);
                break;
            case CollisionResult::PORTAL:
            {
                Portal *p = static_cast<Portal *>(map->getItemAt(ePos));
                ePos = p->getDestination();
                map->removeTypeAll(ItemType::PORTAL);
                break;
            }
            default:
                eResolved = true;
                break;
            }
        }

        CollisionResult eFinal = CollisionManager::check(ePos, *e, *map, allSnakes);
        switch (eFinal)
        {
        case CollisionResult::NONE:
            e->moveDirect(ePos, false, false);
            ++it;
            break;
        case CollisionResult::FOOD:
        {
            e->moveDirect(ePos, true, false);
            Item *targetFood = map->getItemAt(ePos);
            map->removeItem(targetFood);
            if (map->getItemCount(ItemType::FOOD) < 2)
            {
                map->GenerateFood(allSnakes);
            }
            ++it;
            break;
        }
        case CollisionResult::HALVEPOTION:
            e->moveDirect(ePos, false, true);
            map->removeTypeAll(ItemType::HALVEPOTION);
            ++it;
            break;
        case CollisionResult::OBSTACLE:
        case CollisionResult::OTHERSNAKE:
        case CollisionResult::SELF:
            for (const auto &bodyPart : e->getBody())
            {
                if (map->getItemAt(bodyPart) == nullptr)
                {
                    map->addFood(bodyPart);
                }
            }
            e->die();
            it = enemies.erase(it);
            break;
        }
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
                record.saveRecord(gamesettings, this->score);
                initGame();
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

        renderer.render(gamestate, snake, map, score, gamesettings, record, enemies);

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