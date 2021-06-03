#pragma once
#include "GUIRenderer.h"
#include <memory>

namespace UE{

    class Font{
        public:
            Font(const int x, const int y, const int width, const int height, const std::string& msg){
                m_Text = std::make_shared<GUIText>(x,y, msg, "font.ttf");
                m_TextRenderer = std::make_shared<GUIRenderer>();
                m_TextRenderer->init(width, height);
            }

            ~Font(){   }

            void Draw(){ m_TextRenderer->drawText(m_Text.get()); }


        public:

        std::shared_ptr<GUIText> m_Text;
		std::shared_ptr<GUIRenderer> m_TextRenderer;
    };
}