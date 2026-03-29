#pragma once
#include "Common.h"
#include "Snake.h"
#include "MapManager.h"
#include "GameSettings.h"
#include "RecordManager.h"

class Render
{
public:
    void render(GameState gamestate, const Snake *snake, const MapManager *map, int score, const GameSettings &settings, const RecordManager &record);

private:
    void drawStartUI();
    void drawSetUI(const GameSettings &settings);
    void drawHistoryUI(const RecordManager &record);
    void drawPlayingUI(const Snake *snake, const MapManager *map, int score);
    void drawPauseUI();
    void drawGameOverUI(int score, const RecordManager &record);
};