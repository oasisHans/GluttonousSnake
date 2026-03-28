#include "GameSettings.h"

GameSettings::GameSettings()
{
    reset();
}

GameSettings::~GameSettings() {}

void GameSettings::reset()
{
    speedLevel = Speed::NORMAL;
    setStep = SetType::Speed;
    obsLevel = ObstacleLevel::NORMAL;
}

void GameSettings::setGame(int choice)
{
    switch (setStep)
    {
    case SetType::Speed:
        switch (choice)
        {
        case 1:
            speedLevel = Speed::SLOW;
            setStep = SetType::Obastacle;
            break;
        case 2:
            speedLevel = Speed::NORMAL;
            setStep = SetType::Obastacle;
            break;
        case 3:
            speedLevel = Speed::FAST;
            setStep = SetType::Obastacle;
            break;
        default:
            break;
        }
        break;
    // 后续添加其他菜单
    case SetType::Obastacle:
        switch (choice)
        {
        case 1:
            obsLevel = ObstacleLevel::LOW;
            setStep = SetType::Final;
            break;
        case 2:
            obsLevel = ObstacleLevel::NORMAL;
            setStep = SetType::Final;
            break;
        case 3:
            obsLevel = ObstacleLevel::HIGH;
            setStep = SetType::Final;
            break;
        default:
            break;
        }
        break;
    default:
        break;
    }
}

bool GameSettings::isSetDone()
{
    if (this->setStep == SetType::Final)
    {
        return true;
    }
    return false;
}

int GameSettings::PlayDelay() const
{
    switch (this->speedLevel)
    {
    case Speed::SLOW:
        return DELAY_SLOW;
    case Speed::NORMAL:
        return DELAY_NORMAL;
    case Speed::FAST:
        return DELAY_FAST;
    default:
        return DELAY_NORMAL;
    }
}
