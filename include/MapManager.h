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

    void GenerateFood(const std::vector<Snake *> &allSnakes);
    void GenerateObstacle(const std::vector<Snake *> &allSnakes, const ObstacleLevel &num_obs);
    void GeneratePortalPair(const std::vector<Snake *> &allSnakes);
    void GenerateHalvePotion(const std::vector<Snake *> &allSnakes);

    void drawAll() const;

    Point getFoodPos() const;
    void addFood(Point p);

    Point getNearestFoodPos(Point from) const;
    int getItemCount(ItemType type) const;
    Item *getItemAt(Point p) const;
    void removeItem(Item *item);
    void removeTypeAll(ItemType itemtype);
    void clearAll();
};
