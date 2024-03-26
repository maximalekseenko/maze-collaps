#include "interfaceelement.h"


std::vector<InterfaceElement*> InterfaceElement::layers[INTERFACE_ELEMENT_LAYERS_AMOUNT];
std::recursive_mutex InterfaceElement::layers_lock;


InterfaceElement::InterfaceElement(InterfaceElement::Layer __layer) 
: InterfaceElement(__layer, VisualComponent())
{}
InterfaceElement::InterfaceElement(InterfaceElement::Layer __layer, VisualComponent __vc)
: layer(__layer), visualComponent(__vc)
{
    std::lock_guard layers_locker(InterfaceElement::layers_lock);
    InterfaceElement::layers[this->layer].push_back(this);
}
InterfaceElement::~InterfaceElement()
{
    {
        std::lock_guard layers_locker(InterfaceElement::layers_lock);
        InterfaceElement::layers[this->layer].erase(
            std::remove(
                InterfaceElement::layers[this->layer].begin(), 
                InterfaceElement::layers[this->layer].end(), 
                this), 
            InterfaceElement::layers[this->layer].end()
        );
    }
}

void InterfaceElement::Hover(bool __on)
{
    if (hovered == __on) return;
    hovered = __on;
    OnHover(__on);
}
void InterfaceElement::Click(bool __on)
{
    OnClick(__on);
}

void InterfaceElement::OnHover(bool) {}
void InterfaceElement::OnClick(bool) {}