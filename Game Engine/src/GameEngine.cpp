#include "GameEngine.h"

namespace UE {

	constexpr int SCREEN_WIDTH = 1024;
	constexpr int SCREEN_HEIGHT = 576;
	constexpr float ASPECT_RATIO = static_cast<float>(SCREEN_WIDTH) / SCREEN_HEIGHT;

	GameEngine::GameEngine() = default;

	GameEngine::~GameEngine() = default;
	//TODO: link sdl_mixer on windows. 
	bool GameEngine::Init(bool vsync)
	{

		if (TTF_Init() < 0) {
			std::cout << "SDL_TTF could not init! SDL_TTF error: " << TTF_GetError() << "\n";
		}

		if (SDL_Init(SDL_INIT_VIDEO) < 0) {
			std::cerr << "Unable to init SDL sub-systems! SDL Error: " << SDL_GetError() << "\n";
			return false;
		}

		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

		m_Window = SDL_CreateWindow("Unrealistic Engine 5", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
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
			std::cerr << "Unable to init glew! SDL Error: " << glewGetErrorString(status) << "\n";
			return false;
		}

		if (SDL_GL_SetSwapInterval(vsync) != 0) {
			std::cerr << "Warning: unable to set vsync! SDL Error: " << SDL_GetError() << "\n";
			return false;
		}

		//if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
		//	std::cout << "SDL_Mix could not init! SDL_Mix error: " << Mix_GetError() << "\n";
		//	return false;
		//}

		SDL_ShowCursor(SDL_DISABLE);

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

		//=== SYSTEM INTERGRATION ===



		//===========================

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

		m_FontX = std::make_unique<Font>(10,10, SCREEN_WIDTH, SCREEN_HEIGHT, "X: 0");
		m_FontY = std::make_unique<Font>(10,40, SCREEN_WIDTH, SCREEN_HEIGHT, "Y: 0");
		m_FontZ = std::make_unique<Font>(10,70, SCREEN_WIDTH, SCREEN_HEIGHT, "Z: 0");
		m_FontFPS = std::make_unique<Font>(10, 540, SCREEN_WIDTH, SCREEN_HEIGHT, "FPS: 0");

		m_Skybox = std::make_unique<Skybox>("front.png", "back.png",
			"left.png", "right.png",
			"top.png", "bottom.png");

		return true;
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

	bool GameEngine::IsPaused() {

		if (m_KeyDown[SDL_SCANCODE_ESCAPE] == true) {
			return true;
			SDL_ShowCursor(SDL_ENABLE);
		}
		return false;
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

	void GameEngine::Update()
	{
		const float c_CameraSpeed = 0.1F;
		auto a = m_Camera->GetPosition();
		if (m_KeyDown[SDL_SCANCODE_W] == true) {
			m_Camera->SetPosition(m_Camera->GetPosition() + m_Camera->GetTarget() * c_CameraSpeed);
			auto a = m_Camera->GetPosition();
			m_Camera->SetPosition({a.x, 1, a.z});
		}
		if (m_KeyDown[SDL_SCANCODE_S] == true) {
			m_Camera->SetPosition(m_Camera->GetPosition() - m_Camera->GetTarget() * c_CameraSpeed);
			auto a = m_Camera->GetPosition();
			m_Camera->SetPosition({a.x, 1, a.z});
		}
		if (m_KeyDown[SDL_SCANCODE_A] == true) {
			m_Camera->SetPosition(m_Camera->GetPosition() - glm::normalize(glm::cross(m_Camera->GetTarget(), m_Camera->GetUpDirection())) * c_CameraSpeed);
			auto a = m_Camera->GetPosition();
			m_Camera->SetPosition({a.x, 1, a.z});
		}
		if (m_KeyDown[SDL_SCANCODE_D] == true) {
			m_Camera->SetPosition(m_Camera->GetPosition() + glm::normalize(glm::cross(m_Camera->GetTarget(), m_Camera->GetUpDirection())) * c_CameraSpeed);
			auto a = m_Camera->GetPosition();
			m_Camera->SetPosition({a.x, 1, a.z});
		}

		std::stringstream x;
		x << "X: " << std::fixed << std::setprecision(2) << a.x;
		std::stringstream y;
		y << "Y: " << std::fixed << std::setprecision(2) << a.y;
		std::stringstream z;
		z << "Z: " << std::fixed << std::setprecision(2) << a.z;

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
		for (std::unique_ptr<Model>& model : m_Models)
			model->Draw(m_Camera);
		for(auto& billboard : m_Billboard){
			billboard->Draw();
		}
		m_FontX->Draw();
		m_FontY->Draw();
		m_FontZ->Draw();
		m_FontFPS->Draw();

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
		m_FontFPS->SetText(x.str());
	}

	void DisplayInfoMessages(const char* msg)
	{
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Unrealistic Engine", msg, nullptr);
	}
}// namespace UE