#include "GameManager.h"
#include "Common.h"
#include <easyx.h>

extern "C"
{
    void *__imp___iob_func = nullptr;
}

int main()
{
    initgraph(WIDTH, HEIGHT);

    BeginBatchDraw();

    GameManager game;

    game.run();

    EndBatchDraw();
    closegraph();

    return 0;
}