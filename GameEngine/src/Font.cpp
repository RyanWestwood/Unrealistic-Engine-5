#include "Font.h"
#include <GL/glew.h>
#include <iostream>
#include "Utils.h"

namespace UE {

	Font::Font(const int x, const int y, const int width, const int height, const std::string& msg) {
		m_TextRenderer = std::make_shared<GUIRenderer>(width, height);
		m_Font = "font.ttf";
		SetRenderTexture(msg, m_Font, 24);
		m_XPos = x;
		m_YPos = y;
	}

	void Font::SetText(const std::string& msg)
	{
		SetRenderTexture(msg, m_Font, 24);
	}

	void Font::SetRenderTexture(std::string text, std::string fontname, int size) {
		TTF_Font* font = TTF_OpenFont((g_FontDirectory + fontname).c_str(), size);

		if (font == nullptr) {
			std::cerr << "Error creating font " << text << "! " << TTF_GetError() << std::endl;
		}

		SDL_Color White = { 255, 255, 255 };
		SDL_Surface* surfaceImage = TTF_RenderText_Solid(font, text.c_str(), White);
		SDL_Surface* converted = SDL_ConvertSurfaceFormat(surfaceImage, SDL_PIXELFORMAT_RGBA32, 0);
		if (converted != nullptr) {
			SDL_FreeSurface(surfaceImage);
			surfaceImage = converted;
		}
		else {
			std::cerr << "Error converting font " << text << "! " << SDL_GetError() << std::endl;
		}

		int width = surfaceImage->w;
		int height = surfaceImage->h;

		if (SDL_MUSTLOCK(surfaceImage)) {
			SDL_LockSurface(surfaceImage);
		}

		int bytesPerRow = width * surfaceImage->format->BytesPerPixel;

		// Exchange rows of pixels from top to bottom
		char* temp = new char[bytesPerRow];
		for (int y = 0; y < height / 2; y++) {
			// Copy bottom row into temp
			memcpy(temp, ((char*)surfaceImage->pixels) + ((height - y - 1) * bytesPerRow), bytesPerRow);

			// Copy top row to bottom row
			memcpy(((char*)surfaceImage->pixels) + ((height - y - 1) * bytesPerRow), (char*)surfaceImage->pixels + (y * bytesPerRow), bytesPerRow);

			// Copy temp to top row
			memcpy((char*)surfaceImage->pixels + (y * bytesPerRow), temp, bytesPerRow);
		}
		delete[] temp;

		if (SDL_MUSTLOCK(surfaceImage)) {
			SDL_UnlockSurface(surfaceImage);
		}

		Uint32 format = GL_RGBA;

		GLuint textureName;
		glGenTextures(1, &textureName);
		glBindTexture(GL_TEXTURE_2D, textureName);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format,
		GL_UNSIGNED_BYTE, surfaceImage->pixels);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		SDL_FreeSurface(surfaceImage);
		TTF_CloseFont(font);

		m_Texture.reset();
		m_Texture = std::make_shared<Texture>(textureName);
	}

	void Font::Draw()
	{
		m_TextRenderer->DrawTexture(m_XPos, m_YPos, m_Texture);
	}
}