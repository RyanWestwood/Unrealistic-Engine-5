#pragma once
#include "GUIRenderer.h"
#include <memory>

namespace UE{
    class Font{
    public:
		Font(const int x, const int y, const int width, const int height, const std::string& msg);

		void SetRenderTexture(std::string text, std::string fontname, int size);
		void SetText(const std::string& msg);
		
		void SetX(int newX) { m_XPos = newX; }
		void SetY(int newY) { m_YPos = newY; }

		int GetX() { return m_XPos; }
		int GetY() { return m_YPos; }

		void Draw();

    public:
		std::shared_ptr<GUIRenderer> m_TextRenderer;
		std::shared_ptr<Texture> m_Texture;
		std::string m_Font;
		int m_XPos, m_YPos;
    };
}