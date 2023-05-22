#pragma once

#include <map>
#include <string>
#include <functional>


#define TURNCONTINUE true
#define TURNBREAK false


struct Player
{
    public:
        enum Element
        {
            NONE = -1,
            A = 0,
            B = 2,
            C = 3,
        };
        static Player player;

    public:
        int position;
        Element slots[3]{Element::NONE, Element::NONE, Element::NONE};
        const char* visual = "✪";


    public:

        Player(){}
        Player(int position)
            : position(position)
        {

        }


    public: // +++ACTIONS+++
        bool Turn();


    private: // +++ACTIONS+++
        bool Cast(Element);
        bool Move(int dx, int dy);
};
