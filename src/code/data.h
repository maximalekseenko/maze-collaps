#pragma once

#include <set>
#include <tuple>
#include <functional>
#include <string>

// #include "player.h"
#include "spell.h"
// #include "item.h"



std::tuple<std::string, int, std::function<bool()>> Spells[] = 
{
    {"shield", 12, [](){return false;}},
    {"bolt"  , 12, [](){return false;}},
    {"aura"  , 12, [](){return false;}},
    {"breath", 12, [](){return false;}},
    {"fear"  , 12, [](){return false;}},
    {"dig"   , 12, [](){return false;}},
    {"stable", 12, [](){return false;}}
};
