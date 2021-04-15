#pragma once
#include <GL/glew.h>
#include <functional>
#include <iostream>
#include <algorithm>
#include <array>

#include "Skybox.h"
#include "Model.h"
#include "Billboard.h"
#include "CrossPlatform.h"

namespace UE {

	class GameEngine {
	public:

		GameEngine();
		virtual ~GameEngine();

		bool Init(bool);
		bool IsRunning();
		bool IsPaused();

		void Input();
		void Update();
		void Draw();
		void Free();

		void SetWindowTitle(const char* title);

	private:
		SDL_Window* m_Window;
		SDL_GLContext m_Context;

		std::array<bool, 512> m_KeyDown;

		std::vector<std::unique_ptr<Model>> m_Models;
		std::unique_ptr<Skybox> m_Skybox;
		std::shared_ptr<Camera> m_Camera;
		std::shared_ptr<Billboard> m_Billboard;
		glm::vec3 dist;
	};

	void DisplayInfoMessages(const char* msg);
}