#pragma once
#include "Snake.h"
#include "InputHandler.h"

class PlayerSnake : public Snake
{
public:
    PlayerSnake(Point startPos, Direction startDir, COLORREF col)
        : Snake(startPos, startDir, col) {}

    void HandleInput(const InputHandler& input)
    {
        Direction newDir = input.getDirection(this->dir);
        
        this->setDirection(newDir);
    }
};