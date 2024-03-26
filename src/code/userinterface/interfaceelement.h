#pragma once

#include <vector>
#include <ncurses.h>
#include "visualcomponent.h"


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
    private: 


    public:
        InterfaceElement(Layer __layer);
        InterfaceElement(Layer __layer, VisualComponent __vc);
        ~InterfaceElement();
        
    public: // +++ PUBLIC VARIABLES +++
        const Layer layer;
        
        int x, y, w, h;
        VisualComponent visualComponent;
        bool enabled = true;

    public: // +++ 
        // void Render();

    public: // +++ EVENTS +++
        // virtual void OnHover();
        // virtual void OnClick();
};