#pragma once
#include "Common.h"

class GameSettings
{
private:
    Speed speedLevel;
    ObstacleLevel obsLevel;
    SetType setStep;

public:
    GameSettings();
    ~GameSettings();

    void reset();
    void setGame(int choice);
    bool isSetDone();
    int PlayDelay() const;
    SetType getSetStep() const { return setStep; }
    ObstacleLevel getObstacleLevel() const { return obsLevel; }
    Speed getSpeedLevel() const { return speedLevel; }
};