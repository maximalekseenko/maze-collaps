#pragma once

#include "spell.h"


struct Player
{
    public: // +++PLAYER TOOLS+++
        Spell spells[3];
        // Item items[1];


    public:
        int timeLeft;


    public:
        int score;
};