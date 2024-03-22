#include "entity.h"

#include <vector>

int Entity::nextId = 0;

Entity::Entity(int __pos)
    : id(Entity::nextId++), position(__pos)
{}

Entity::~Entity() {
}
const char* Entity::GetVisual() { return "?"; }

void Entity::Turn() {}