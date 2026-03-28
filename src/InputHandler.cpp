#include "InputHandler.h"

void InputHandler::updateKey()
{
    ExMessage msg;
    hasNewKey = false; // 每帧开始重置

    while (peekmessage(&msg, EX_KEY))
    {
        if (msg.message == WM_KEYDOWN)
        {
            lastKey = msg.vkcode;
            hasNewKey = true;
        }
    }
}

// 处理数字输入
int InputHandler::getNumberInput() const
{
    if (!hasNewKey)
        return -1;
    if (lastKey >= '0' && lastKey <= '9')
        return lastKey - '0';
    if (lastKey >= VK_NUMPAD1 && lastKey <= VK_NUMPAD9)
        return lastKey - VK_NUMPAD1 + 1;
    return -1;
}

// 将按键转换为 Direction 枚举
Direction InputHandler::getDirection(Direction currentDir) const
{
    if (!hasNewKey)
        return currentDir;

    switch (lastKey)
    {
    case 'W':
    case 'w':
    case VK_UP:
        return Direction::UP;
    case 'S':
    case 's':
    case VK_DOWN:
        return Direction::DOWN;
    case 'A':
    case 'a':
    case VK_LEFT:
        return Direction::LEFT;
    case 'D':
    case 'd':
    case VK_RIGHT:
        return Direction::RIGHT;
    }
    return currentDir;
}