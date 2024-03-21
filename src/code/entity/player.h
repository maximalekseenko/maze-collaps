#pragma once

#include "entity.h"

class Player : public Entity
{
    using Entity::Entity;

    public:
        void Turn() override;
};
