#pragma once


// #include "entitylogic.h"


class Entity
{
    public:
        Entity(int __pos);
        Entity(int __pos, int __vid);
        virtual ~Entity();

    private:
        static int nextId;

    public:
        const int id;
        int position;

    public:
        virtual const char* GetVisual();

    public:
        virtual void Turn();
};