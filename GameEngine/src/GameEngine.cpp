#include "GameEngine.h"

namespace UE {

	constexpr int SCREEN_WIDTH = 1024;
	constexpr int SCREEN_HEIGHT = 576;
	constexpr float ASPECT_RATIO = static_cast<float>(SCREEN_WIDTH) / SCREEN_HEIGHT;

	Coordinator g_Coordinator;

	bool GameEngine::InitialiseSubsystems(bool vsync)
	{
		if (SDL_Init(SDL_INIT_VIDEO) < 0) {
			std::cerr << "Unable to init SDL sub-systems! SDL Error: " << SDL_GetError() << "\n";
			return false;
		}

		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

		m_Window = SDL_CreateWindow("Unrealistic Engine 5",
			SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
			SCREEN_WIDTH, SCREEN_HEIGHT,
			SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN
		);

		if (m_Window == nullptr) {
			std::cerr << "Unable to create window! SDL Error: " << SDL_GetError() << "\n";
			return false;
		}

		m_Context = SDL_GL_CreateContext(m_Window);
		if (m_Context == nullptr) {
			std::cerr << "Unable to create context! SDL Error: " << SDL_GetError() << "\n";
			return false;
		}

		GLenum status = glewInit();
		if (status != GLEW_OK) {
			std::cerr << "Unable to init GLEW! SDL Error: " << glewGetErrorString(status) << "\n";
			return false;
		}

		if (SDL_GL_SetSwapInterval(vsync) != 0) {
			std::cerr << "Warning: unable to set vsync! SDL Error: " << SDL_GetError() << "\n";
			return false;
		}

		if (TTF_Init() < 0) {
			std::cout << "SDL_TTF could not init! SDL_TTF error: " << TTF_GetError() << "\n";
		}

		SDL_ShowCursor(SDL_DISABLE);

		//	ECS System
		g_Coordinator.Init();
		g_Coordinator.RegisterComponent<TransformComponent>();
		g_Coordinator.RegisterComponent<SpriteComponent>();

		m_PhysicsSystem = g_Coordinator.RegisterSystem<PhysicsSystem>();
		m_PhysicsSystem->SetSystem();

		m_RenderSystem = g_Coordinator.RegisterSystem<RendererSystem>();
		m_RenderSystem->SetSystem();

		// TODO: Abstract to scene levels?
		SceneSetup();

		return true;
	}

	void GameEngine::SceneSetup()
	{
		//	ECS TEST
		std::vector<std::uint32_t> entities(1);
		for (auto& entity : entities)
		{
			entity = g_Coordinator.CreateEntity();
			g_Coordinator.AddComponent(entity, TransformComponent{
				.position = glm::vec3(1),
				.scale = glm::vec3(1),
				.rotation = glm::quat()
				});
			g_Coordinator.AddComponent(entity, SpriteComponent{
				.renderer = std::make_shared<GUIRenderer>(SCREEN_WIDTH, SCREEN_HEIGHT),
				.sprite = std::make_shared<Texture>((g_TextureDirectory + std::string("tree.png")).c_str())
				});
		}
		//	===========================

		m_Camera = std::make_shared<Camera>(
			glm::vec3(0.0f, 1.0f, 5.0f),
			glm::vec3(0.0f, 0.0f, 0.0f) + dist,
			glm::vec3(0.0f, 1.0f, 0.0f),
			45.0f, ASPECT_RATIO, 0.1f, 1000.0f
			);

		SDL_WarpMouseInWindow(m_Window, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
		m_Camera->SetOldMouseX(SCREEN_WIDTH / 2);
		m_Camera->SetOldMouseY(SCREEN_HEIGHT / 2);

		int mouseX = 512, mouseY = 512;
		SDL_GetMouseState(&mouseX, &mouseY);

		int diffX = mouseX - m_Camera->GetOldMouseX();
		int diffY = m_Camera->GetOldMouseY() - mouseY;

		m_Camera->SetYaw(m_Camera->GetYaw() + (diffX * .1F));
		m_Camera->SetPitch(m_Camera->GetPitch() + (diffY * .1F));

		glm::vec3 direction;
		direction.x = cos(glm::radians(m_Camera->GetYaw())) * cos(glm::radians(m_Camera->GetPitch()));
		direction.y = sin(glm::radians(m_Camera->GetPitch()));
		direction.z = sin(glm::radians(m_Camera->GetYaw())) * cos(glm::radians(m_Camera->GetPitch()));
		m_Camera->SetTarget(glm::normalize(direction));

		m_Camera->UpdateCameraMatrices();
		m_Camera->SetOldMouseX(SCREEN_WIDTH / 2);
		m_Camera->SetOldMouseY(SCREEN_HEIGHT / 2);

		dist = glm::vec3(0.0f, 0.0f, -100.0f);

		m_Models.reserve(100);
		m_Models.emplace_back(std::make_unique<Model>("ground_grass.obj", "textureGreen.jpg"));
		m_Models.back()->SetPosition({ 0.0F, 0.0F, 0.0F });
		m_Models.back()->SetScale({ 100.0F, 100.0F, 100.0F });
		m_Models.emplace_back(std::make_unique<Model>("bed.obj", "textureOrange.jpg"));
		m_Models.back()->SetPosition({ -3.0F, 0.0F, 1.0F });
		m_Models.emplace_back(std::make_unique<Model>("tree_palmShort.obj", "textureBlue.jpg"));
		m_Models.back()->SetPosition({ 0.0F, 0.0F, -3.0F });
		m_Models.emplace_back(std::make_unique<Model>("campfire_stones.obj", "textureOrange.jpg"));
		m_Models.back()->SetPosition({ 1.0F, 0.0F, 1.0F });
		m_Models.emplace_back(std::make_unique<Model>("tent_smallOpen.obj", "textureBlue.jpg"));
		m_Models.back()->SetPosition({ 4.0F, 0.0F, -3.0F });
		m_Models.back()->SetRotation({ 0.0F, -45.0F, 0.0F });
		m_Models.emplace_back(std::make_unique<Model>("log_stack.obj", "textureOrange.jpg"));
		m_Models.back()->SetPosition({ 2.0F, 0.0F, -3.0F });
		m_Models.back()->SetScale({ 1.0F, 1.0F, 1.0F });

		for (unsigned short i = 1; i < 6; i++)
		{
			m_Billboard.emplace_back(std::make_shared<Billboard>("tree.png", m_Camera));
			m_Billboard[i - 1]->SetScale({ 5.0F, 5.0F, 0.0F });
		}
		m_Billboard[0]->SetPosition({ -7.F, 0.0F, -7.0F });
		m_Billboard[1]->SetPosition({ 0.F, 0.0F, -7.0F });
		m_Billboard[2]->SetPosition({ -10.F, 0.0F, -3.0F });
		m_Billboard[3]->SetPosition({ 3.F, 0.0F, 2.0F });
		m_Billboard[4]->SetPosition({ 6.F, 0.0F, 7.0F });

		m_FontX = std::make_unique<Font>(10, 10, SCREEN_WIDTH, SCREEN_HEIGHT, "X: 0");
		m_FontY = std::make_unique<Font>(10, 40, SCREEN_WIDTH, SCREEN_HEIGHT, "Y: 0");
		m_FontZ = std::make_unique<Font>(10, 70, SCREEN_WIDTH, SCREEN_HEIGHT, "Z: 0");

		m_Skybox = std::make_unique<Skybox>("front.png", "back.png",
			"left.png", "right.png",
			"top.png", "bottom.png");

		m_IsRunning = false;
	}

	bool GameEngine::IsRunning()
	{
		SDL_PumpEvents();

		SDL_Event event;
		if (SDL_PeepEvents(&event, 1, SDL_GETEVENT, SDL_QUIT, SDL_QUIT)) {
			return false;
		}
		if (m_KeyDown[SDL_SCANCODE_X] == true) {
			return false;
		}

		return true;
	}

	bool GameEngine::IsPaused() 
	{
		return m_IsRunning;
	}

	void GameEngine::Resume()
	{
		SDL_Event e;
		while (SDL_PollEvent(&e) != 0) {
			if (e.type == SDL_KEYDOWN) {
				if (e.key.keysym.sym == SDLK_ESCAPE) {
					m_IsRunning = !m_IsRunning;
					m_KeyDown[SDLK_ESCAPE] = false;
					SDL_ShowCursor(m_IsRunning);
				}
			}
		}
	}

	void GameEngine::Input()
	{
		const float c_MouseSensitivity = 0.01F;
		int mouseX = 0, mouseY = 0;
		SDL_GetMouseState(&mouseX, &mouseY);

		int diffX = mouseX - m_Camera->GetOldMouseX();
		int diffY = m_Camera->GetOldMouseY() - mouseY;

		m_Camera->SetYaw(m_Camera->GetYaw() + (diffX * c_MouseSensitivity));
		m_Camera->SetPitch(m_Camera->GetPitch() + (diffY * c_MouseSensitivity));

		glm::vec3 direction;
		direction.x = cos(glm::radians(m_Camera->GetYaw())) * cos(glm::radians(m_Camera->GetPitch()));
		direction.y = sin(glm::radians(m_Camera->GetPitch()));
		direction.z = sin(glm::radians(m_Camera->GetYaw())) * cos(glm::radians(m_Camera->GetPitch()));
		m_Camera->SetTarget(glm::normalize(direction));

		SDL_Event e;
		while (SDL_PollEvent(&e) != 0) {
			if (e.type == SDL_KEYDOWN) {
				if (e.key.keysym.scancode < 512) {
					m_KeyDown[e.key.keysym.scancode] = true;
				}
				if (e.key.keysym.sym == SDLK_ESCAPE) {
					m_IsRunning = !m_IsRunning;
					SDL_ShowCursor(m_IsRunning);					
				}
			}
			else if (e.type == SDL_KEYUP) {
				if (e.key.keysym.scancode < 512) {
					m_KeyDown[e.key.keysym.scancode] = false;
				}
			}
		}
		m_Camera->UpdateCameraMatrices();
		m_Camera->SetOldMouseX(SCREEN_WIDTH / 2);
		m_Camera->SetOldMouseY(SCREEN_HEIGHT / 2);

		SDL_WarpMouseInWindow(m_Window, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
	}

	// #TODO: might look jank because not using delta time for updates?
	// find a way to add both movements together?
	void GameEngine::Update()
	{
		const float c_CameraSpeed = 0.1F;
		const auto cameraUp = m_Camera->GetUpDirection();
		const auto cameraTarget = m_Camera->GetTarget();
		auto cameraPosition = m_Camera->GetPosition();
		auto movement = cameraPosition;

		if (m_KeyDown[SDL_SCANCODE_W] == true) {
			movement = cameraPosition + cameraTarget * c_CameraSpeed;
		}
		if (m_KeyDown[SDL_SCANCODE_S] == true) {
			movement = cameraPosition - cameraTarget * c_CameraSpeed;
		}
		movement.y = 1;
		m_Camera->SetPosition(movement);
		cameraPosition = m_Camera->GetPosition();

		if (m_KeyDown[SDL_SCANCODE_A] == true) {
			movement = cameraPosition - glm::normalize(glm::cross(cameraTarget, cameraUp)) * c_CameraSpeed;
		}
		if (m_KeyDown[SDL_SCANCODE_D] == true) {
			movement = cameraPosition + glm::normalize(glm::cross(cameraTarget, cameraUp)) * c_CameraSpeed;
		}
		movement.y = 1;
		m_Camera->SetPosition(movement);

		m_PhysicsSystem->Update();

		std::stringstream x;
		x << "X: " << std::fixed << std::setprecision(2) << cameraPosition.x;
		std::stringstream y;
		y << "Y: " << std::fixed << std::setprecision(2) << cameraPosition.y;
		std::stringstream z;
		z << "Z: " << std::fixed << std::setprecision(2) << cameraPosition.z;

		m_FontX->SetText(x.str());
		m_FontY->SetText(y.str());
		m_FontZ->SetText(z.str());
		m_Camera->UpdateCameraMatrices();
	}

	void GameEngine::Draw()
	{
		glClearColor(0.1F, 0.1F, 0.1F, 1.0F);
		glEnable(GL_DEPTH_TEST);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		m_Skybox->Draw(m_Camera);
		
		for (auto& model : m_Models)
			model->Draw(m_Camera);
		for (auto& billboard : m_Billboard)
			billboard->Draw();

		m_RenderSystem->Draw();

		m_FontX->Draw();
		m_FontY->Draw();
		m_FontZ->Draw();

		SDL_GL_SwapWindow(m_Window);
	}

	void GameEngine::Free()
	{
		SDL_DestroyWindow(m_Window);
		m_Window = nullptr;
		SDL_Quit();
	}

	void GameEngine::SetWindowTitle(const char* title)
	{
		SDL_SetWindowTitle(m_Window, title);
	}

	void GameEngine::SetFps(const int fps)
	{
		std::stringstream x;
		x << "FPS: " << std::fixed << std::setprecision(5) << fps;
	}

	void DisplayInfoMessages(const char* msg)
	{
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Unrealistic Engine", msg, nullptr);
	}
}// namespace UE