#pragma once

#include "renderer.h"

#include <vector>
#include <string>



class VisualComponent
{
    public:
        class VisualComponentRule
        {
            public:
                VisualComponentRule(int __x, int __y, const char * __ch);
                VisualComponentRule(int __x, int __y, const char * __ch, Renderer::Color __colorF);
                VisualComponentRule(int __x, int __y, const char * __ch, Renderer::Color __colorF, Renderer::Color __colorB);
                ~VisualComponentRule();

            public:
                int x, y;
                const char * ch = " ";
                Renderer::Color colorF=Renderer::Color::WHITE;
                Renderer::Color colorB=Renderer::Color::BLACK;

            public:

        };


    public:
        VisualComponent();
        VisualComponent(std::vector<VisualComponentRule> __rules);
        ~VisualComponent();
        
    public:
        void Render(int __x, int __y);
        std::vector<VisualComponentRule> rules;
};