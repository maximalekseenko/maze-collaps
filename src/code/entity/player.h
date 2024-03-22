#pragma once

#include "entity.h"

class Player : public Entity
{
    using Entity::Entity;

    public:
        const char* GetVisual() override;
        void Turn() override;
};
