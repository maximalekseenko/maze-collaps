#include "entity.h"

int Entity::nextId = 0;

Entity::Entity()
    : id(Entity::nextId++)
{

}

Entity::~Entity() {

}