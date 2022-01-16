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

#include "ECS/ECS.h"
#include "ECS/systems/PhysicsSystem.h"
#include "ECS/systems/RendererSystem.h"
#include "ECS/components/SpriteComponent.h"
#include "ECS/components/TransformComponent.h"

namespace UE {

	class GameEngine {
	public:
		bool InitialiseSubsystems(bool);
		bool IsRunning();
		bool IsPaused();
		void Resume();

		void Input();
		void Update();
		void Draw();
		void Free();

		void SetFps(const int fps);
		void SetWindowTitle(const char* title);

	private:
		void SceneSetup();

		SDL_Window* m_Window;
		SDL_GLContext m_Context;

		std::array<bool, 512> m_KeyDown;
		bool m_IsRunning;

		std::vector<std::unique_ptr<Model>> m_Models;
		std::vector<std::shared_ptr<Billboard>> m_Billboard;
		std::unique_ptr<Skybox> m_Skybox;
		std::shared_ptr<Camera> m_Camera;
		std::unique_ptr<Font> m_FontX;
		std::unique_ptr<Font> m_FontY;
		std::unique_ptr<Font> m_FontZ;

		glm::vec3 dist;

		std::shared_ptr<PhysicsSystem> m_PhysicsSystem;
		std::shared_ptr<RendererSystem> m_RenderSystem;
	};

	void DisplayInfoMessages(const char* msg);
}