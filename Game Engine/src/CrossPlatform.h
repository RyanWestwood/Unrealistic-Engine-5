#pragma once

#if defined(_WIN32) || defined(_WIN64)
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#endif

#if defined(__unix__) || defined(__linux__)
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <SDL2/SDL_image.h>
#endif
