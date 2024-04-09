#include "visualcomponent.h"


#include "engine.h"
#include "color.h"



std::shared_mutex VisualComponent::layers_lock;
std::vector<VisualComponent*> VisualComponent::layers[__ENGINE_VISUALCOMPONENT_LAYER_AMOUNT];
VisualComponent::Layer VisualComponent::lastUpdatedLayer;
int VisualComponent::lastUpdateMinX = -1, VisualComponent::lastUpdateMaxX = INT_MAX,
    VisualComponent::lastUpdateMinY = -1, VisualComponent::lastUpdateMaxY = INT_MAX;
void UpdateLayer(VisualComponent::Layer __layer, int __x, int __y, int __w, int __h)
{

    if (VisualComponent::lastUpdatedLayer > __layer || VisualComponent::lastUpdatedLayer == VisualComponent::Layer::NONE)
        VisualComponent::lastUpdatedLayer = __layer;

    if (VisualComponent::lastUpdateMinX > __x)
        VisualComponent::lastUpdateMinX = __x;

    if (VisualComponent::lastUpdateMinY > __y)
        VisualComponent::lastUpdateMinY = __y;

    if (VisualComponent::lastUpdateMaxX < __x + __w)
        VisualComponent::lastUpdateMaxX = __x + __w;

    if (VisualComponent::lastUpdateMaxY < __y + __h)
        VisualComponent::lastUpdateMaxY = __y + __h;
}



VisualComponent::VisualComponent(int __x, int __y, int __w, int __h, VisualComponent::Layer __layer)
: VisualComponent(__x, __y, __w, __h, __layer, "")
{}
VisualComponent::VisualComponent(int __x, int __y, int __w, int __h, VisualComponent::Layer __layer, const char* __content)
{
    this->layer = __layer;
    {   std::lock_guard ncurses_locker(Engine::ncurses_lock);
        this->win = newwin(__h, __w, __y, __x);
        wbkgd(this->win, COLOR_PAIR(GetColorPairId(0, Engine::Renderer::GetBackground())));
        wprintw(this->win, __content);
    }
}
VisualComponent::~VisualComponent()
{
    this->Clear();
    wbkgd(this->win, COLOR_PAIR(GetColorPairId(0, Engine::Renderer::GetBackground())));
    this->Render();
    {   std::lock_guard ncurses_locker(Engine::ncurses_lock);
        delwin(this->win);
    }
    this->Activate(false);
}


#include "../lib/log.h"
void VisualComponent::Activate(bool __on)
{
    // Already activated/deactivated
    if (this->is_active == __on) return;

    this->is_active = __on;

    if (__on) // activating -> add self to array
    {   std::unique_lock layers_locker(VisualComponent::layers_lock);
        VisualComponent::layers[this->layer].push_back(this);
    }
    else // deactivating -> remove self from array
    {   std::unique_lock layers_locker(VisualComponent::layers_lock);
        VisualComponent::layers[this->layer].erase(
            std::remove(
                VisualComponent::layers[this->layer].begin(), 
                VisualComponent::layers[this->layer].end(), 
                this
            ), 
            VisualComponent::layers[this->layer].end()
        );
    }

    UpdateLayer(
        this->layer, 
        this->GetX(), this->GetY(), 
        this->GetW(), this->GetH()
    );
}



void VisualComponent::Clear()
{
    {   std::lock_guard ncurses_locker(Engine::ncurses_lock);
        wclear(this->win);
    }
}



void VisualComponent::AddLine(int __x, int __y, const char* __content, Color __colorF, Color __colorB)
{
    // add content on window
    {   std::lock_guard ncurses_locker(Engine::ncurses_lock);

        // set color
        wattron(this->win, COLOR_PAIR(GetColorPairId(__colorF, __colorB)));
        if (__colorF / 8 == 1) wattron(this->win, A_BOLD);
        else                  wattroff(this->win, A_BOLD);

        // print
        mvwprintw(this->win, __y, __x, __content);
    }

    // update if active
    if (this->is_active)
    {
        UpdateLayer(
            this->layer,
            this->GetX(), this->GetY(),
            this->GetW(), this->GetH()
        );
    }
}



void VisualComponent::Render()
{
    {   std::lock_guard ncurses_locker(Engine::ncurses_lock);
        redrawwin(this->win);
        wrefresh(this->win);
    }
}



int VisualComponent::GetX()    { std::lock_guard ncurses_locker(Engine::ncurses_lock); return getbegx(this->win); }
int VisualComponent::GetY()    { std::lock_guard ncurses_locker(Engine::ncurses_lock); return getbegy(this->win); }
int VisualComponent::GetW()    { std::lock_guard ncurses_locker(Engine::ncurses_lock); return getmaxx(this->win); }
int VisualComponent::GetH()    { std::lock_guard ncurses_locker(Engine::ncurses_lock); return getmaxy(this->win); }
int VisualComponent::GetMaxX() { std::lock_guard ncurses_locker(Engine::ncurses_lock); return GetX() + GetW(); }
int VisualComponent::GetMaxY() { std::lock_guard ncurses_locker(Engine::ncurses_lock); return GetY() + GetH(); }

void VisualComponent::SetX(int __x) { mvwin(this->win, this->GetY(), __x); }
void VisualComponent::SetY(int __y) { mvwin(this->win, __y, this->GetX()); }
void VisualComponent::SetXY(int __x, int __y) { mvwin(this->win, __y, __x); }



/// @brief Help function to check if point is in this visual component.
/// @param __x Point position on x axis.
/// @param __y Point position on y axis.
/// @param __me Visual component to check with.
/// @return `true` if point is located withing borders of rectangle.
bool IsPointWithinMe(int __x, int __y, VisualComponent* __me)
{
    return __me->GetX() <= __x 
        && __me->GetMaxX() > __x
        && __me->GetY() <= __y 
        && __me->GetMaxY() > __y;
}



bool VisualComponent::Hover(int __x, int __y)
{
    // Is mouse is over me?
    int __on = IsPointWithinMe(__x, __y, this);

    // Should it be updated?
    // It should if it actually hovered by mouse
    // Or if it was under hover in previous frame, buy not this one.
    if (this->hovered == false && __on == false) return false;

    // Update hovered status
    this->hovered = __on;

    // do stuff and return ... 
    return OnHover(__on, __x - this->GetX(), __y - this->GetY());
}



bool VisualComponent::Click(int __x, int __y)
{
    // Check for mouse to be over
    if (!IsPointWithinMe(__x, __y, this)) return false;

    // Do stuff and return ...
    return OnClick(__x - this->GetX(), __y - this->GetY());
}

bool VisualComponent::OnHover(bool, int, int) {}
bool VisualComponent::OnClick(int, int) {}