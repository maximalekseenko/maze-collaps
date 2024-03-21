#pragma once


// #include "entitylogic.h"


class Entity
{
    public:
        Entity(int __pos);
        virtual ~Entity();

    private:
        static int nextId;

    public:
        const int id;
        int position;

        int visual_id;

    public:
        virtual void Turn();
};