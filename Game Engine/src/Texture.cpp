#include "Texture.h"
#include <iostream>

namespace UE {

	void UE::Texture::LoadTexture(std::string filename)
	{
		SDL_Surface* surfaceImage = IMG_Load(filename.c_str());
		if (surfaceImage == nullptr) {
			std::cout << "Error loading texture\n";
			return;
		}

		m_Width = surfaceImage->w;
		m_Height = surfaceImage->h;
		m_Format = surfaceImage->format->format;

		switch (m_Format)
		{
		case SDL_PIXELFORMAT_RGBA32:
			m_Format = GL_RGBA;
			break;

		case SDL_PIXELFORMAT_RGB24:
			m_Format = GL_RGB;
			break;

		default:
			m_Format = GL_RGB;
			break;
		}

		glGenTextures(1, &m_TextureName);
		glBindTexture(GL_TEXTURE_2D, m_TextureName);

		glTexImage2D(GL_TEXTURE_2D, 0, m_Format, m_Width, m_Height, 0, m_Format, GL_UNSIGNED_BYTE, surfaceImage->pixels);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		SDL_FreeSurface(surfaceImage);
	}

}
