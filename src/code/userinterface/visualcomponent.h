#pragma once

#include "renderer.h"

#include <vector>
#include <string>
#include <mutex>

#include <ncurses.h>



class VisualComponent
{
    public:
        enum Layer {
            NONE        = -1,
            BACKGROUND  = 0,
            STATIC      = 1,
            DYNAMIC     = 2,
            BUTTONS     = 3,
        };
        #define VISUALCOMPONENT_LAYER_AMOUNT 4
        static std::recursive_mutex layers_lock;
        static std::vector<VisualComponent*> layers[VISUALCOMPONENT_LAYER_AMOUNT];
        static VisualComponent::Layer lastUpdatedLayer;
        static int lastUpdateMinX, lastUpdateMaxX, lastUpdateMinY, lastUpdateMaxY;

    public:
        VisualComponent(int __x, int __y, int __w, int __h, VisualComponent::Layer);
        VisualComponent(int __x, int __y, int __w, int __h, VisualComponent::Layer, const char* __content);
        ~VisualComponent();


    public:
        void Activate(bool=true);

    public:
        std::recursive_mutex lock;

    private:

        /// @brief Layer of this component.
        VisualComponent::Layer layer;

        /// @brief Is this component is currently active and should be rendered?
        bool is_active = false;


    public:
        void Clear();
        void AddLine(int __x, int __y, const char* __content, Renderer::Color __colorF=Renderer::Color::WHITE, Renderer::Color __colorB=Renderer::Color::BLACK);

        void Render();

        int GetX();
        int GetY();
        int GetW();
        int GetH();
        int GetMaxX();
        int GetMaxY();

        void SetX(int);
        void SetY(int);
        void SetXY(int, int);

    protected:
        WINDOW* win;


    public: // +++ EVENTS +++
        void Hover(bool, int, int);
        void Click(bool, int, int);
    private:
        bool hovered=false;
        virtual void OnHover(bool, int, int);
        virtual void OnClick(bool, int, int);

};