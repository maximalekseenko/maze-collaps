#pragma once


// #include "entitylogic.h"


class Entity
{
    public:
        Entity(int __pos);
        ~Entity();

    private:
        static int nextId;

    public:
        const int id;
        int position;
        // EntityLogic logic;

        int visual_id;
};