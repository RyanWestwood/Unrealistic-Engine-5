#pragma once
#include <GL/glew.h>
#include <SDL.h>
#include <SDL_opengl.h>

namespace Divide {

	class GameEngine {
	public:

		GameEngine();
		virtual ~GameEngine();

		bool Init(bool);
		bool isRunning();

		void Input();
		void Update();
		void Draw();

		void Clean();

		void SetWindowTitle(const char* title);

	private:
		SDL_Window* m_Window;
		SDL_GLContext m_Context;
	};

	void DisplayInfoMessages(const char*);
}
