#pragma once
#include <GL/glew.h>
#include "CrossPlatform.h"
#include <string>

namespace UE {
	class Texture {
	public:
		Texture(std::string filename) {
			m_Width = m_Height = 0;
			m_TextureName = 0;
			LoadTexture(filename);
		}

		Texture(GLuint name){
			m_TextureName = name;

			glBindTexture(GL_TEXTURE_2D, m_TextureName);			
			glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &m_Width);
			glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &m_Height);
			glBindTexture(GL_TEXTURE_2D, 0);

			m_Format = GL_RGBA;
		}

		~Texture() {
			glDeleteTextures(1, &m_TextureName);
		}

		int GetWidth() { return m_Width; }
		int GetHeight() { return m_Height; }
		uint32_t GetFormat() { return m_Format; }
		GLuint GetTextureName() { return m_TextureName; }

	private:
		void LoadTexture(const std::string& filename);

		int m_Width;
		int m_Height;
		uint32_t m_Format;
		GLuint m_TextureName;
	};
}