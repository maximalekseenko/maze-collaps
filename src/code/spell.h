#pragma once

#include <functional>


struct Spell
{
    public: // +++FUNCTIONS TO SET+++
        int BaseCost;
        std::function<bool()> OnUse;


    public: // +++VARIABLES+++
        int Cost;
        int Level;
};