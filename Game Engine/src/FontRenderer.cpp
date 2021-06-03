#include "FontRenderer.h"
#include <GL/glew.h>
#include <iostream>
#include "Utils.h"

namespace UE{

    void FontRenderer::Init(){
        if (TTF_Init() < 0) {
		    std::cout << "SDL_TTF could not init! SDL_TTF error: " << TTF_GetError() << "\n";
	    }
    }

    Texture* FontRenderer::RenderText(std::string text, std::string fontname, int size){
		TTF_Font* font = TTF_OpenFont((g_FontDirectory + fontname).c_str(), size); 

		if (font == nullptr) {
			std::cerr << "Error creating font " << text << "! " << TTF_GetError() << std::endl;
			return nullptr;
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
			return nullptr;
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
		return new Texture(textureName);
    }

    void FontRenderer::Destroy(){
        TTF_Quit();
    }
}