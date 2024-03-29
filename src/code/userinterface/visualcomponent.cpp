#include "visualcomponent.h"


#include "userinterface.h"


std::recursive_mutex VisualComponent::layers_lock;
std::vector<VisualComponent*> VisualComponent::layers[VISUALCOMPONENT_LAYER_AMOUNT];




VisualComponent::VisualComponent(int __x, int __y, int __w, int __h, VisualComponent::Layer __layer)
: VisualComponent(__x, __y, __w, __h, __layer, "")
{}
VisualComponent::VisualComponent(int __x, int __y, int __w, int __h, VisualComponent::Layer __layer, const char* __content)
{
    this->layer = __layer;
    {   std::lock_guard ncurses_lock(UserInterface::ncurses_lock);
        this->win = newwin(__h, __w, __y, __x);
        wprintw(this->win, __content);
    }
}
VisualComponent::~VisualComponent()
{
    {   std::lock_guard layers_lock(VisualComponent::layers_lock);
        this->Clear();
        this->Render();
        {   std::lock_guard ncurses_lock(UserInterface::ncurses_lock);
            delwin(this->win);
        }
        this->Activate(false);
    }
}



void VisualComponent::Activate(bool __on)
{
    if (this->is_active != __on)
    {
        if (__on)
        {   std::lock_guard layers_lock(VisualComponent::layers_lock);
            VisualComponent::layers[this->layer].push_back(this);
        }
        else 
        {   std::lock_guard layers_lock(VisualComponent::layers_lock);
            VisualComponent::layers[this->layer].erase(
                std::remove(
                    VisualComponent::layers[this->layer].begin(), 
                    VisualComponent::layers[this->layer].end(), 
                    this
                ), 
                VisualComponent::layers[this->layer].end()
            );
        }
    }
    this->is_active = __on;
}



void VisualComponent::Clear()
{
    {   std::lock_guard ncurses_lock(UserInterface::ncurses_lock);
        wclear(this->win);
    }
}



void VisualComponent::AddLine(int __x, int __y, const char* __content, Renderer::Color __colorF, Renderer::Color __colorB)
{
    {   std::lock_guard ncurses_lock(UserInterface::ncurses_lock);
        Renderer::RenderText(this->win, __x, __y, __content, __colorF, __colorB);
    }
}



void VisualComponent::Render()
{
    {   std::lock_guard ncurses_lock(UserInterface::ncurses_lock);
        wrefresh(this->win);
    }
}



int VisualComponent::GetX() { return getbegx(this->win); }
int VisualComponent::GetY() { return getbegy(this->win); }
int VisualComponent::GetW() { return getmaxx(this->win); }
int VisualComponent::GetH() { return getmaxy(this->win); }

void VisualComponent::SetX(int __x) { mvwin(this->win, this->GetY(), __x); }
void VisualComponent::SetY(int __y) { mvwin(this->win, __y, this->GetX()); }
void VisualComponent::SetXY(int __x, int __y) { mvwin(this->win, __y, __x); }



void VisualComponent::Hover(bool __on)
{
    if (hovered == __on) return;
    hovered = __on;
    OnHover(__on);
}
void VisualComponent::Click(bool __on)
{
    OnClick(__on);
}

void VisualComponent::OnHover(bool) {}
void VisualComponent::OnClick(bool) {}