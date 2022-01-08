#pragma once
#include <GL/glew.h>
#include "CrossPlatform.h"
#include <string>

namespace UE {
	class Texture {
	public:
		Texture(std::string filename);
		Texture(GLuint name);

		~Texture() { glDeleteTextures(1, &m_TextureName); }

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