#pragma once
#include <GL/glew.h>
#include <SDL.h>
#include <SDL_opengl.h>

#include "Camera.h"
#include "ModelRenderer.h"
#include "Texture.h"

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
		ModelRenderer* m_ModelRenderer;
		Model* m_Model;
		Texture* m_Texture;
	};

	void DisplayInfoMessages(const char*);
}
