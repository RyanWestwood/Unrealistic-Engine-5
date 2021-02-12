#pragma once
#include <SDL.h>
#include <GL/glew.h>
#include <SDL_image.h>
#include <string>

namespace UE {
	class Texture {
	public:
		Texture(std::string filename) {
			m_Width = m_Height = 0;
			m_TextureName = 0;
			LoadTexture(filename);
		}

		~Texture() {}

		int GetWidth() { return m_Width; }
		int GetHeight() { return m_Height; }
		uint32_t GetFormat() { return m_Format; }
		GLuint GetTextureName() { return m_TextureName; }

	private:
		void LoadTexture(std::string filename);

		int m_Width;
		int m_Height;
		uint32_t m_Format;
		GLuint m_TextureName;
	};
}