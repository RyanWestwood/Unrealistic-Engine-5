#include "GameEngine.h"
#include "Font.h"

namespace UE {

	constexpr int SCREEN_WIDTH = 1024;
	constexpr int SCREEN_HEIGHT = 576;
	constexpr float ASPECT_RATIO = static_cast<float>(SCREEN_WIDTH) / SCREEN_HEIGHT;

	GameEngine::GameEngine() = default;

	GameEngine::~GameEngine() = default;
	//TODO: link sdl_mixer on windows. 
	bool GameEngine::Init(bool vsync)
	{
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

		m_Billboard = std::make_shared<Billboard>("tree.png", m_Camera);
		m_Billboard->Init();
		m_Billboard->SetScale({ 5.0F, 5.0F, 0.0F });
		m_Billboard->SetPosition({ 7.0F,0.0F, -7.0F });

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
		//TODO: Debugging close?
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
		const float c_CameraSpeed = 0.1F;
		const float c_MouseSensitivity = 0.1F;

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

		if (m_KeyDown[SDL_SCANCODE_W] == true) {
			m_Camera->SetPosition(m_Camera->GetPosition() + m_Camera->GetTarget() * c_CameraSpeed);
		}
		if (m_KeyDown[SDL_SCANCODE_S] == true) {
			m_Camera->SetPosition(m_Camera->GetPosition() - m_Camera->GetTarget() * c_CameraSpeed);
		}
		if (m_KeyDown[SDL_SCANCODE_A] == true) {
			m_Camera->SetPosition(m_Camera->GetPosition() - glm::normalize(glm::cross(m_Camera->GetTarget(), m_Camera->GetUpDirection())) * c_CameraSpeed);
		}
		if (m_KeyDown[SDL_SCANCODE_D] == true) {
			m_Camera->SetPosition(m_Camera->GetPosition() + glm::normalize(glm::cross(m_Camera->GetTarget(), m_Camera->GetUpDirection())) * c_CameraSpeed);
		}

		m_Camera->UpdateCameraMatrices();
		m_Camera->SetOldMouseX(SCREEN_WIDTH / 2);
		m_Camera->SetOldMouseY(SCREEN_HEIGHT / 2);

		SDL_WarpMouseInWindow(m_Window, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
	}

	void GameEngine::Update()
	{

	}

	void GameEngine::Draw()
	{
		glClearColor(0.1F, 0.1F, 0.1F, 1.0F);
		glEnable(GL_DEPTH_TEST);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		m_Skybox->Draw(m_Camera);
		for (std::unique_ptr<Model>& model : m_Models)
			model->Draw(m_Camera);
		m_Billboard->Draw();

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

	void DisplayInfoMessages(const char* msg)
	{
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Unrealistic Engine", msg, nullptr);
	}
}// namespace UE