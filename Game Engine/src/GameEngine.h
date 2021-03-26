#pragma once
#include <GL/glew.h>
#include <SDL.h>
#include <SDL_opengl.h>
#include <functional>
#include <iostream>
#include <algorithm>
#include "Skybox.h"
#include "Model.h"
#include "Billboard.h"

namespace UE {

	class GameEngine {
	public:

		GameEngine();
		virtual ~GameEngine();

		bool Init(bool);
		bool IsRunning();

		void Input();
		void Update();
		void Draw();
		void Free();

		void SetWindowTitle(const char* title);

	private:
		SDL_Window* m_Window;
		SDL_GLContext m_Context;

		std::vector<std::unique_ptr<Model>> m_Models;
		std::unique_ptr<Skybox> m_Skybox;
		std::shared_ptr<Camera> m_Camera;
		std::shared_ptr<Billboard> m_Billboard;
		glm::vec3 dist;
	};

	void DisplayInfoMessages(const char*);
}
