#pragma once

#include <map>


struct Player
{
    public:
        int x, y;
        std::wstring visual = L"✪";


    public:
        bool isSpellEyeActive = false;
        


    public:
        Player(){}
        Player(int x, int y)
            : x(x), y(y)
        {

        }


    public:
        int timeLeft;


    public:
        int score;



    void Cast(std::string name)
    {
        if (name == "bolt")
        {

        }
    }
};


std::map<std::string, int> SpellIds
{
    {"bolt", 0},
    {"aura", 1},
    {"breath", 2},
};