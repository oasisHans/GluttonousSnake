#pragma once
#include "Entity.h"
#include "Item.h"
#include "Snake.h"
#include "PlayerSnake.h"
#include "Food.h"
#include "Portal.h"
#include "Obstacle.h"
#include "HalvePotion.h"
#include "GameSettings.h"
#include <vector>
#include <map>

class MapManager
{
private:
    std::map<ItemType, std::vector<Item *>> warehouses;
    RandomGenerator rng;

public:
    MapManager();
    ~MapManager();

    void GenerateFood(const Snake &snake);
    void GenerateObstacle(const Snake &snake, const ObstacleLevel &num_obs);
    void GeneratePortalPair(const Snake &snake);
    void GenerateHalvePotion(const Snake &snake);

    void drawAll() const;

    Item *getItemAt(Point p) const;
    void removeItem(Item *item);
    void removeTypeAll(ItemType itemtype);
    void clearAll();
};
