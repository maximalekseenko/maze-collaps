#pragma once

#include "entity.h"

class Enemy : public Entity
{
    public:
        Enemy(int __pos);

    public:
        bool activated=false;
        int type=-1;

    public:
        const char* GetVisual() override;
        void Turn() override;
};
