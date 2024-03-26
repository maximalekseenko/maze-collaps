#include "interfaceelement.h"

std::vector<InterfaceElement*> InterfaceElement::layers[INTERFACE_ELEMENT_LAYERS_AMOUNT];

InterfaceElement::InterfaceElement(InterfaceElement::Layer __layer) 
: layer(__layer)
{
    InterfaceElement::layers[this->layer].push_back(this);
}
InterfaceElement::InterfaceElement(InterfaceElement::Layer __layer, VisualComponent __vc)
: layer(__layer), visualComponent(__vc)
{
    InterfaceElement::layers[this->layer].push_back(this);
}

InterfaceElement::~InterfaceElement()
{
    InterfaceElement::layers[this->layer].erase(
        std::remove(
            InterfaceElement::layers[this->layer].begin(), 
            InterfaceElement::layers[this->layer].end(), 
            this), 
        InterfaceElement::layers[this->layer].end()
    );
}