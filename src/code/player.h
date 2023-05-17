#pragma once

#include <map>
#include <string>
#include <functional>


struct Player
{
    public:
        enum SpellSource
        {
            NONE     = -1,
            CHAOS    = 0,
            WRATH    = 2,
            DIVINITY = 3,
        };
        static Player player;

    public:
        int position;
        SpellSource sources[3]{SpellSource::NONE, SpellSource::NONE, SpellSource::NONE};
        std::wstring visual = L"✪";


    public: // +++SPELL EFFECTS+++
        bool shield;
        bool stabilizing;


    public:

        Player(){}
        Player(int position)
            : position(position)
        {

        }


    public: // +++ACTIONS+++
        bool Prep(SpellSource);
        bool Cast();
        bool Move();
};