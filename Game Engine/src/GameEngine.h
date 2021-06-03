#pragma once
#include <GL/glew.h>
#include <functional>
#include <iostream>
#include <algorithm>
#include <array>
#include <sstream>
#include <iomanip>
#include "Font.h"
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

		void SetFps(const int fps);
		void SetWindowTitle(const char* title);

	private:
		SDL_Window* m_Window;
		SDL_GLContext m_Context;

		std::array<bool, 512> m_KeyDown;

		std::vector<std::unique_ptr<Model>> m_Models;
		std::vector<std::shared_ptr<Billboard>> m_Billboard;
		std::unique_ptr<Skybox> m_Skybox;
		std::shared_ptr<Camera> m_Camera;
		std::unique_ptr<Font> m_FontX;
		std::unique_ptr<Font> m_FontY;
		std::unique_ptr<Font> m_FontZ;
		std::unique_ptr<Font> m_FontFPS;

		glm::vec3 dist;
	};

	void DisplayInfoMessages(const char* msg);
}