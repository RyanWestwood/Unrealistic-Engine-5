#pragma once
#include <GL/glew.h>
#include <SDL.h>
#include <SDL_opengl.h>

#include "Camera.h"
#include "ModelRenderer.h"

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

		void Free();

		void SetWindowTitle(const char* title);

	private:
		SDL_Window* m_Window;
		SDL_GLContext m_Context;
		Camera* m_Camera;
		ModelRenderer* m_ModelRenderer;
		Model* m_Model;
	};

	void DisplayInfoMessages(const char*);
}
