#pragma once
#include "Entity.h"

class Item : public Entity
{
public:
    Item(Point p, COLORREF c, ItemType t)
        : Entity(p, c, t) {}

    virtual ~Item() {}
    
    //放Item类型专属的接口
};