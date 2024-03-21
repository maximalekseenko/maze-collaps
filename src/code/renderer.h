#pragma once


#include "map.h"
#include "entity/entity.h"


class Renderer {
    private:
        static bool initialized;

    public:
        static void Init();

    public:


        static void Render_Game(
            int __center=0,
            bool __safe=true
        );
    public: 

        /// @brief Renders map on the screen.
        /// @param __map Pointer on map to render.
        /// @param __center Coordinate to center screen on. 
        /// @param __safe Do extra stuff for safety.
        static void Render_Map(
            Map *__map, 
            int __offset_x=0,
            int __offset_y=0,
            int __center=0,
            bool __safe=true
        );


        /// @brief Renders entity on the screen.
        /// @param __map Pointer on map for this entity.
        /// @param __ent Pointer on entity to render.
        /// @param __center Coordinate to center screen on. 
        /// @param __safe Do extra stuff for safety.
        static void Render_Entity(
            Map *__map,
            Entity *__ent, 
            int __offset_x=0,
            int __offset_y=0,
            int __center=0,
            bool __safe=true
        );
};