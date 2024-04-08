#ifndef __ENGINE_VISUALCOMPONENT_H
#define __ENGINE_VISUALCOMPONENT_H


#include <curses.h>
#include "color.h"
#include <vector>
#include <string>
#include <mutex>
#include <shared_mutex>


/// @brief Visual component, that is used for rendering and input handling.
class VisualComponent
{
    public: // +++ static +++

        /// @brief Layers of visual components. Used for rendering and input handling order.
        enum Layer {
            NONE        = -1,
            BACKGROUND  = 0,
            STATIC      = 1,
            DYNAMIC     = 2,
            BUTTONS     = 3,
        };
        #define __ENGINE_VISUALCOMPONENT_LAYER_AMOUNT 4

        /// @brief Lock for layers editing. (Thanks, lumi)
        static std::shared_mutex layers_lock;

        /// @brief Array of vectors of Active VisualComponents. Size of array is defined in __ENGINE_VISUALCOMPONENT_LAYER_AMOUNT.
        static std::vector<VisualComponent*> layers[__ENGINE_VISUALCOMPONENT_LAYER_AMOUNT];

        // UGH.. REMOVE THIS!!!!
        static VisualComponent::Layer lastUpdatedLayer;
        // AND THIS | THIS IS UGLY
        static int lastUpdateMinX, lastUpdateMaxX, lastUpdateMinY, lastUpdateMaxY;


    public: // +++ CONSTRUCTORS AND DESTRUCTORS +++
        
        /// @brief Simplest constructor for visual component.
        /// @param __x Starting position on x axis of visual component on the core window.
        /// @param __y Starting position on y axis of visual component on the core window.
        /// @param __w Starting width of visual component.
        /// @param __h Starting height of visual component.
        /// @param __layer Layer this visual component located in. (see VisualComponent::layers).
        VisualComponent(int __x, int __y, int __w, int __h, VisualComponent::Layer __layer);
        VisualComponent(int __x, int __y, int __w, int __h, VisualComponent::Layer, const char* __content);
        ~VisualComponent();


    public:
        /// @brief Activates or deactivates this visual component.
        /// @param __on Component will be activated if `__on` is `true`, deactivated otherwise. Default value is `true`.
        void Activate(bool __on=true);

    public:
        /// @brief Lock for accessing this visual component's functions.
        std::recursive_mutex lock;

    private:

        /// @brief Layer of this component.
        VisualComponent::Layer layer;

        /// @brief Is this component is currently active and should be rendered?
        bool is_active = false;


    protected:
        /// @brief 
        void Clear();
        void AddLine(int __x, int __y, const char* __content, Color __colorF=Color::WHITE, Color __colorB=Color::BLACK);

    public: // +++ USAGE FUNCTIONS +++

        /// @brief Renders this 
        void Render();

    public: // +++ ACCESS FUNCTIONS +++

        /// @brief Get current position on x axis of this visual component.
        /// @return Current position on x axis of this visual component.
        int GetX();

        /// @brief Get current position on y axis of this visual component.
        /// @return Current position on y axis of this visual component.
        int GetY();

        /// @brief Get current width of this visual component.
        /// @return Current width of this visual component.
        int GetW();

        /// @brief Get current height of this visual component.
        /// @return Current height of this visual component.
        int GetH();

        /// @brief Get current farthest position on x axis of this visual component.
        /// @return Current position on x axis of this visual component plus current width of this visual component.
        int GetMaxX();

        /// @brief Get current farthest position on y axis of this visual component.
        /// @return Current position on y axis of this visual component plus current height of this visual component.
        int GetMaxY();

        /// @brief Set current position on x axis of this visual component.
        /// @param __x New position on x axis.
        void SetX(int __x);

        /// @brief Set current position on y axis of this visual component.
        /// @param __y New position on y axis.
        void SetY(int __y);

        /// @brief Set current position on x and y axes of this visual component.
        /// @param __x New position on x axis.
        /// @param __y New position on y axis.
        void SetXY(int __x, int __y);

    protected:
        WINDOW* win;


    public: // +++ EVENTS PUBLIC +++

        /// @brief Attempt to hover over this visual component (!) Actual functions that does stuff on hover is VisualComponent::OnHover (!)
        /// @param __x Mouse position on x axis at moment of hover attempt.
        /// @param __y Mouse position on y axis at moment of hover attempt.
        /// @return `true` if mouse is over this visual component for this event.
        bool Hover(int __x, int __y);

        /// @brief Attempt to click over this visual component (!) Actual functions that does stuff on click is VisualComponent::OnClick (!)
        /// @param __x Mouse position on x axis at moment of click attempt.
        /// @param __y Mouse position on y axis at moment of click attempt.
        /// @return `true` if mouse is over this visual component for this event.
        bool Click(int __x, int __y);

    private: // +++ EVENTS PRIVATE +++

        /// @brief Was this visual component hovered (see VisualComponent::Hover) in previous frame?
        bool hovered=false;

        /// @brief Virtual fuck
        /// @param  fuck fuck
        /// @param  fuck fuck
        /// @param  fuck fuck
        virtual void OnHover(bool __on, int __x, int __y);
        virtual void OnClick(int __x, int __y);

};


#endif // __ENGINE_VISUALCOMPONENT_H