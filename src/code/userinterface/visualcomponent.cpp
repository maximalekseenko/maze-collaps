#include "visualcomponent.h"

VisualComponent::VisualComponentRule::VisualComponentRule(int __x, int __y, const char * __ch)
: VisualComponentRule(__x, __y, __ch, Renderer::Color::WHITE, Renderer::Color::BLACK)
{}
VisualComponent::VisualComponentRule::VisualComponentRule(int __x, int __y, const char * __ch, Renderer::Color __colorF)
: VisualComponentRule(__x, __y, __ch, __colorF, Renderer::Color::BLACK)
{}
VisualComponent::VisualComponentRule::VisualComponentRule(int __x, int __y, const char * __ch, Renderer::Color __colorF, Renderer::Color __colorB)
: x(__x), y(__y), ch(__ch), colorF(__colorF), colorB(__colorB)
{}
VisualComponent::VisualComponentRule::~VisualComponentRule()
{}


VisualComponent::VisualComponent()
{}
VisualComponent::VisualComponent(std::vector<VisualComponentRule> __rules)
: rules(__rules)
{}
VisualComponent::~VisualComponent()
{}


void VisualComponent::Render(int __x, int __y) 
{
    for (auto &_rule : rules)
        Renderer::RenderText(
            __x + _rule.x,
            __y + _rule.y,
            _rule.ch,
            _rule.colorF,
            _rule.colorB
        );
}