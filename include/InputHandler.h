#pragma once
#include "Common.h"

class InputHandler
{
private:
    int lastKey;
    bool hasNewKey;

public:
    InputHandler() : lastKey(0), hasNewKey(false) {}

    void updateKey();

    int getNumberInput() const;

    // 将按键转换为 Direction 枚举
    Direction getDirection(Direction currentDir) const;

    // 通用按键（如空格暂停
    bool isKeyPressed(int vkCode) const
    {
        return (hasNewKey && lastKey == vkCode);
    }
};