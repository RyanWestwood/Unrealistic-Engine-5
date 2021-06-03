#pragma once
#include "Texture.h"
#include "FontRenderer.h"
#include "CrossPlatform.h"
#include <glm/glm.hpp>
#include <string>

// GUI classes
namespace UE {
	class GUIText {
	public:
		GUIText(int x, int y, std::string text, std::string ff) {
			m_Font = ff;
			m_FontRenderer = new FontRenderer();
			m_FontRenderer->Init();

			m_Texture = m_FontRenderer->RenderText(text, m_Font, 24);

			m_XPos = x;
			m_YPos = y;
		}

		~GUIText() {
			if (m_Texture) {
				delete m_Texture;
			}
			delete m_FontRenderer;
		}

		void setFontFileName(const std::string ffName) {
			m_Font = ffName;
		}

		void setText(const std::string& text) {
			if (m_Texture) {
				delete m_Texture;
				m_Texture = nullptr;
			}

			// Note, whilst can change the font size is fixed.
			// Left as an exercise to allow size to be changed
			m_Texture = m_FontRenderer->RenderText(text, m_Font, 24);
		}

		Texture* getTexture() { return m_Texture; }

		void setX(int newX) { m_XPos = newX; }
		void setY(int newY) { m_YPos = newY; }

		int getX() { return m_XPos; }
		int getY() { return m_YPos; }

	private:
		std::string m_Font;
		FontRenderer* m_FontRenderer;
		Texture* m_Texture;
		int m_XPos, m_YPos;
	};

	// GUI rendering class
	class GUIRenderer
	{
	public:
		GUIRenderer();

		void init(int window_width, int window_height);

		void drawText(GUIText* guiText) { drawTexture(guiText->getX(), guiText->getY(), guiText->getTexture()); }
		void destroy();

	private:
		void drawTexture(int x, int y, Texture* tex);

	private:
		GLuint programId;
		GLint vertexLocation;
		GLint vertexUVLocation;
		GLuint projectionUniformId;
		GLuint samplerId;
		glm::mat4 projectionMat;
	};
}