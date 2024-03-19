#include "entity.h"

int Entity::nextId = 0;

Entity::Entity(int __pos)
    : id(Entity::nextId++), position(__pos)
{

}

Entity::~Entity() {

}