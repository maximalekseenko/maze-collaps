#pragma once

#include <vector>
#include <ncurses.h>
#include "visualcomponent.h"
#include <mutex>


class InterfaceElement 
{
    public: 
        /// @brief Layers of interface. Used for rendering. Higher the number - later it will be rendered.
        enum Layer {
            /// @brief Layer for background elements with only decorative purpose.
            BACKGROUND  = 0,
            /// @brief Layer for static interface elements (e.g stats)
            STATIC      = 1,
            /// @brief Second layer of game elements (e.g. stats, map).
            DYNAMIC     = 2,
            BUTTONS     = 3,
        };
        #define INTERFACE_ELEMENT_LAYERS_AMOUNT 4

        static std::vector<InterfaceElement*> layers[INTERFACE_ELEMENT_LAYERS_AMOUNT];
        static std::recursive_mutex layers_lock;
    private: 


    public:
        InterfaceElement(Layer __layer);
        InterfaceElement(Layer __layer, VisualComponent __vc);
        virtual ~InterfaceElement();
        
    public: // +++ PUBLIC VARIABLES +++
        const Layer layer;
        
        int x, y, w, h;
        VisualComponent visualComponent;
        bool enabled = true;

    public: // +++ 
        std::recursive_mutex lock;

    public: // +++ EVENTS +++
    public:
        void Hover(bool);
        void Click(bool);
    private:
        bool hovered=false;

        virtual void OnHover(bool);
        virtual void OnClick(bool);
    
};