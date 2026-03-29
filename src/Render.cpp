#include "Render.h"

void Render::render(GameState gamestate, const Snake *snake, const MapManager *map, int score, const GameSettings &settings, const RecordManager &record)
{
    BeginBatchDraw();
    cleardevice();
    // 根据游戏状态绘图
    switch (gamestate)
    {
    case GameState::Start:
        this->drawStartUI();
        break;
    case GameState::Set:
        this->drawSetUI(settings);
        break;
    case GameState::History:
        this->drawHistoryUI(record);
        break;
    case GameState::Playing:
        this->drawPlayingUI(snake, map, score);
        break;
    case GameState::Paused:
        this->drawPauseUI();
        break;
    case GameState::GameOver:
        this->drawGameOverUI(score, record);
        break;
    }
    EndBatchDraw();
}

void Render::drawStartUI()
{
    settextcolor(GREEN);
    settextstyle(60, 0, _T("Consolas"));
    outtextxy(WIDTH / 8 + 10, HEIGHT / 3, _T("Gluttonous Snake"));

    settextcolor(WHITE);
    settextstyle(25, 0, _T("Consolas"));
    outtextxy(WIDTH / 4, HEIGHT / 2 + 40, _T("Press SPACE to Start"));

    settextcolor(WHITE);
    settextstyle(25, 0, _T("Consolas"));
    outtextxy(WIDTH / 4, HEIGHT / 2 + 80, _T("Press H to check HISTORY GOAT"));
}

void Render::drawSetUI(const GameSettings &settings)
{
    switch (settings.getSetStep())
    {
    // 速度设置板块渲染
    case SetType::Speed:
        settextcolor(WHITE);
        settextstyle(25, 0, _T("Consolas"));
        outtextxy(WIDTH / 8, HEIGHT / 6, _T("Select Speed:"));

        settextcolor(YELLOW);
        outtextxy(WIDTH / 4, HEIGHT / 2 - 40, _T("1. SLOW"));
        outtextxy(WIDTH / 4, HEIGHT / 2, _T("2. NORMAL"));
        outtextxy(WIDTH / 4, HEIGHT / 2 + 40, _T("3. FAST"));

        settextcolor(LIGHTGRAY);
        outtextxy(WIDTH / 4, HEIGHT / 2 + 100, _T("Press 1, 2 or 3 to Set Speed"));
        break;
    case SetType::Obastacle:
        settextcolor(WHITE);
        settextstyle(25, 0, _T("Consolas"));
        outtextxy(WIDTH / 8, HEIGHT / 6, _T("Select num of Obstacles:"));

        settextcolor(YELLOW);
        outtextxy(WIDTH / 4, HEIGHT / 2 - 40, _T("1. LOW"));
        outtextxy(WIDTH / 4, HEIGHT / 2, _T("2. NORMAL"));
        outtextxy(WIDTH / 4, HEIGHT / 2 + 40, _T("3. HIGHT"));

        settextcolor(LIGHTGRAY);
        outtextxy(WIDTH / 4, HEIGHT / 2 + 100, _T("Press 1, 2 or 3 to Set Obastale"));
        break;
    }
}

void Render::drawHistoryUI(const RecordManager &record)
{
    record.draw();
}

void Render::drawPlayingUI(const Snake *snake, const MapManager *map, int score)
{
    map->drawAll();
    snake->draw();

    // 绘制实时分数
    settextcolor(YELLOW);
    settextstyle(20, 0, _T("Consalas"));
    TCHAR strScore[16];
    _stprintf_s(strScore, _T("Score:%d"), score);
    outtextxy(10, 10, strScore);
}

void Render::drawPauseUI()
{
    settextcolor(WHITE);
    settextstyle(25, 0, _T("Consolas"));
    outtextxy(WIDTH / 4, HEIGHT / 2, _T("Press SPACE to continue"));
}
void Render::drawGameOverUI(int score, const RecordManager &record)
{
    settextcolor(RED);
    settextstyle(80, 0, _T("Consolas"));
    outtextxy(WIDTH / 6, HEIGHT / 4, _T("GAME OVER!!!"));

    settextcolor(WHITE);
    settextstyle(40, 0, _T("Consolas"));
    TCHAR scoreStr[32];
    _stprintf_s(scoreStr, _T("Final Score: %d"), score);
    outtextxy(WIDTH / 4, HEIGHT / 3 + 40, scoreStr);

    if (score > record.last_Goat)
    {
        settextcolor(YELLOW);
        settextstyle(30, 0, _T("Consolas"));
        outtextxy(WIDTH / 4, HEIGHT / 3 + 80, _T("New Goat!"));
    }

    settextcolor(YELLOW);
    settextstyle(25, 0, _T("Consolas"));

    outtextxy(WIDTH / 4, HEIGHT / 3 + 120, _T("Press 1 to Play Again"));
    outtextxy(WIDTH / 4, HEIGHT / 3 + 160, _T("Press 2 to StartMenu"));
    outtextxy(WIDTH / 4, HEIGHT / 3 + 200, _T("Press 0 to Exit Game"));
}