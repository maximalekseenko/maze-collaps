#include "visualcomponent.h"


#include "userinterface.h"


std::recursive_mutex VisualComponent::layers_lock;
std::vector<VisualComponent*> VisualComponent::layers[VISUALCOMPONENT_LAYER_AMOUNT];
VisualComponent::Layer VisualComponent::lastUpdatedLayer;
int VisualComponent::lastUpdateMinX = -1, VisualComponent::lastUpdateMaxX = INT_MAX,
    VisualComponent::lastUpdateMinY = -1, VisualComponent::lastUpdateMaxY = INT_MAX;
void UpdateLayer(VisualComponent::Layer __layer, int __x, int __y, int __w, int __h)
{   std::lock_guard layers_locker(VisualComponent::layers_lock);

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
    {   std::lock_guard ncurses_locker(UserInterface::ncurses_lock);
        this->win = newwin(__h, __w, __y, __x);
        wprintw(this->win, __content);
    }
}
VisualComponent::~VisualComponent()
{
    {   std::lock_guard layers_locker(VisualComponent::layers_lock);
        this->Clear();
        this->Render();
        {   std::lock_guard ncurses_locker(UserInterface::ncurses_lock);
            delwin(this->win);
        }
        this->Activate(false);
    }
}



void VisualComponent::Activate(bool __on)
{
    if (this->is_active != __on)
    {   std::lock_guard layers_locker(VisualComponent::layers_lock);

        if (__on)
        {
            VisualComponent::layers[this->layer].push_back(this);
        }
        else 
        {
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
        this->is_active = __on;
    }
}



void VisualComponent::Clear()
{
    {   std::lock_guard ncurses_locker(UserInterface::ncurses_lock);
        wclear(this->win);
    }
}



void VisualComponent::AddLine(int __x, int __y, const char* __content, Renderer::Color __colorF, Renderer::Color __colorB)
{
    // add content on window
    {   std::lock_guard ncurses_locker(UserInterface::ncurses_lock);
        Renderer::RenderText(this->win, __x, __y, __content, __colorF, __colorB);
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
    {   std::lock_guard ncurses_locker(UserInterface::ncurses_lock);
        redrawwin(this->win);
        wrefresh(this->win);
    }
}



int VisualComponent::GetX()    { std::lock_guard ncurses_locker(UserInterface::ncurses_lock); return getbegx(this->win); }
int VisualComponent::GetY()    { std::lock_guard ncurses_locker(UserInterface::ncurses_lock); return getbegy(this->win); }
int VisualComponent::GetW()    { std::lock_guard ncurses_locker(UserInterface::ncurses_lock); return getmaxx(this->win); }
int VisualComponent::GetH()    { std::lock_guard ncurses_locker(UserInterface::ncurses_lock); return getmaxy(this->win); }
int VisualComponent::GetMaxX() { std::lock_guard ncurses_locker(UserInterface::ncurses_lock); return GetX() + GetW(); }
int VisualComponent::GetMaxY() { std::lock_guard ncurses_locker(UserInterface::ncurses_lock); return GetY() + GetH(); }

void VisualComponent::SetX(int __x) { mvwin(this->win, this->GetY(), __x); }
void VisualComponent::SetY(int __y) { mvwin(this->win, __y, this->GetX()); }
void VisualComponent::SetXY(int __x, int __y) { mvwin(this->win, __y, __x); }



void VisualComponent::Hover(bool __on, int __x, int __y)
{
    if (this->hovered == false && __on == false) return;

    OnHover(__on, __x - this->GetX(), __y - this->GetY());
    this->hovered = __on;
}
void VisualComponent::Click(bool __on, int __x, int __y)
{
    OnClick(__on, __x - this->GetX(), __y - this->GetY());
}

void VisualComponent::OnHover(bool, int, int) {}
void VisualComponent::OnClick(bool, int, int) {}