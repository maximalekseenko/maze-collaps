#pragma once

#include <functional>
#include <string>


struct Spell
{
    public:
        Spell(const wchar_t* name, const wchar_t* description, int cost, std::function<bool()> Onuse, Spell nextLevelSpell)
            : nextLevelSpell(nextLevelSpell), Spell()
        {

        }
        Spell(const wchar_t* name, const wchar_t* description, int cost, std::function<bool()> Onuse)
        {

        }
    public: // +++FUNCTIONS TO SET+++
        Spell nextLevelSpell;
        const wchar_t* name;
        const wchar_t* description;
        int baseCost;
        std::function<bool()> OnUse;


    public: // +++VARIABLES+++
        int cost;
        int level;
};