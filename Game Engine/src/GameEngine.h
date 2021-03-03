#pragma once
#include <GL/glew.h>
#include <SDL.h>
#include <SDL_opengl.h>

#include "SkyboxRenderer.h"
#include "Model.h"
#include "Billboard.h"
#include "BillboardRenderer.h"

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
		Camera* m_Camera;
		std::vector<Model*> m_Models;
		SkyboxRenderer* m_Skybox;

		Billboard* m_Billboard;
		BillboardRenderer* m_BillboardRender;

		glm::vec3 dist;
	};

	void DisplayInfoMessages(const char*);
}
