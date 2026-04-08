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
    iqLevel = IQLevel::NORMAL;
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
    case SetType::Obastacle:
        switch (choice)
        {
        case 0:
            obsLevel = ObstacleLevel::NONE;
            setStep = SetType::IQ;
            break;
        case 1:
            obsLevel = ObstacleLevel::LOW;
            setStep = SetType::IQ;
            break;
        case 2:
            obsLevel = ObstacleLevel::NORMAL;
            setStep = SetType::IQ;
            break;
        case 3:
            obsLevel = ObstacleLevel::HIGH;
            setStep = SetType::IQ;
            break;

        default:
            break;
        }
        break;
    case SetType::IQ:
        switch (choice)
        {
        case 0:
            ifgenerateEnemy = false;
            setStep = SetType::Final;
            break;
        case 1:
            ifgenerateEnemy = true;
            iqLevel = IQLevel::LOW;
            setStep = SetType::Final;
            break;
        case 2:
            ifgenerateEnemy = true;
            iqLevel = IQLevel::NORMAL;
            setStep = SetType::Final;
            break;
        case 3:
            ifgenerateEnemy = true;
            iqLevel = IQLevel::HIGH;
            setStep = SetType::Final;
            break;
        }
        break;
    default:
        break;
    }
}

int GameSettings::getIQValue() const
{
    switch (iqLevel)
    {
    case IQLevel::LOW:
        return IQValue_LOW;
    case IQLevel::NORMAL:
        return IQValue_NORMAL;
    case IQLevel::HIGH:
        return IQValue_HIGH;
    default:
        return IQValue_NORMAL;
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
