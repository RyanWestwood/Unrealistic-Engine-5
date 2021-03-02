#include "GameEngine.h"
#include <iostream>

#define SCREEN_WIDTH 1024
#define SCREEN_HEIGHT 576
#define ASPECT_RATIO SCREEN_WIDTH / SCREEN_HEIGHT

UE::GameEngine::GameEngine()
{
}

UE::GameEngine::~GameEngine()
{
}

bool UE::GameEngine::Init(bool vsync)
{
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		std::cerr << "Unable to init SDL sub-systems! SDL Error: " << SDL_GetError() << "\n";
		return false;
	}

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

	m_Window = SDL_CreateWindow("Unrealistic Engine", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
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

	dist = glm::vec3(0.0f, 0.0f, -100.0f);

	m_Camera = new Camera(
		glm::vec3(0.0f, 0.0f, 5.0f),
		glm::vec3(0.0f, 0.0f, 0.0f) + dist,
		glm::vec3(0.0f, 1.0f, 0.0f),
		45.0f, ASPECT_RATIO, 0.1f, 1000.0f
	);

	m_Model = new Model();
	bool result = m_Model->LoadFromFile("resources/models/bed.obj");
	if (!result) {
		std::cerr << "Failed to load model!\n";
	}

	m_Texture = new Texture("resources/models/texture.jpg");

	m_ModelRenderer = new ModelRenderer(m_Model);
	m_ModelRenderer->Init();
	m_ModelRenderer->SetRotation({ 0.0f, 0.0f, 0.0f });
	m_ModelRenderer->SetScale({ 3,3,3 });
	m_ModelRenderer->SetMaterial(m_Texture);

	m_Skybox = new SkyboxRenderer( "resources/skybox/front.png", "resources/skybox/back.png",
								   "resources/skybox/left.png",  "resources/skybox/right.png",
								   "resources/skybox/top.png",   "resources/skybox/bottom.png" );
							
	return true;
}

bool UE::GameEngine::IsRunning()
{
	SDL_PumpEvents();

	SDL_Event event;
	if (SDL_PeepEvents(&event, 1, SDL_GETEVENT, SDL_QUIT, SDL_QUIT)) {
		return false;
	}
	return true;
}

void UE::GameEngine::Input()
{
	const float c_CameraSpeed = 1.0f;
	const float c_MouseSensitivity = 0.1f;

	int mouseX, mouseY;
	SDL_GetMouseState(&mouseX, &mouseY);

	float diffX = mouseX - m_Camera->GetOldMouseX();
	float diffY = m_Camera->GetOldMouseY() - mouseY;

	m_Camera->SetYaw((m_Camera->GetYaw() + diffX) * c_MouseSensitivity);
	m_Camera->SetPitch((m_Camera->GetPitch() + diffY) * c_MouseSensitivity);

	glm::vec3 direction;
	direction.x = cos(glm::radians(m_Camera->GetYaw())) * cos(glm::radians(m_Camera->GetPitch()));
	direction.y = sin(glm::radians(m_Camera->GetPitch()));
	direction.z = sin(glm::radians(m_Camera->GetYaw())) * cos(glm::radians(m_Camera->GetPitch()));
	m_Camera->SetTarget(glm::normalize(direction));

	bool keyStates[4];
	memset(keyStates, false, sizeof(keyStates));

	enum {
		UP = 0,
		DOWN,
		LEFT,
		RIGHT
	};

	SDL_Event e;
	if (SDL_PollEvent(&e)) {
		if (e.type == SDL_KEYDOWN) {
			switch (e.key.keysym.scancode) {
			case SDL_SCANCODE_W:
			case SDL_SCANCODE_UP:
				keyStates[UP] = true;
				break;
			case SDL_SCANCODE_S:
			case SDL_SCANCODE_DOWN:
				keyStates[DOWN] = true;
				break;
			case SDL_SCANCODE_A:
			case SDL_SCANCODE_LEFT:
				keyStates[LEFT] = true;
				break;
			case SDL_SCANCODE_D:
			case SDL_SCANCODE_RIGHT:
				keyStates[RIGHT] = true;
				break;
			}
		}
		if (e.type == SDL_KEYUP) {
			switch (e.key.keysym.scancode) {
			case SDL_SCANCODE_W:
			case SDL_SCANCODE_UP:
				keyStates[UP] = false;
				break;
			case SDL_SCANCODE_S:
			case SDL_SCANCODE_DOWN:
				keyStates[DOWN] = false;
				break;
			case SDL_SCANCODE_A:
			case SDL_SCANCODE_LEFT:
				keyStates[LEFT] = false;
				break;
			case SDL_SCANCODE_D:
			case SDL_SCANCODE_RIGHT:
				keyStates[RIGHT] = false;
				break;
			}
		}
	}

	if (keyStates[UP]) {
		m_Camera->SetPosition(m_Camera->GetPosition() + m_Camera->GetTarget() * c_CameraSpeed);
	}
	if (keyStates[DOWN]) {
		m_Camera->SetPosition(m_Camera->GetPosition() - m_Camera->GetTarget() * c_CameraSpeed);
	}
	if (keyStates[LEFT]) {
		m_Camera->SetPosition(m_Camera->GetPosition() - glm::normalize(glm::cross(m_Camera->GetTarget(), m_Camera->GetUpDirection())) * c_CameraSpeed);
	}
	if (keyStates[RIGHT]) {
		m_Camera->SetPosition(m_Camera->GetPosition() + glm::normalize(glm::cross(m_Camera->GetTarget(), m_Camera->GetUpDirection())) * c_CameraSpeed);
	}

	m_Camera->UpdateCameraMatrices();
	m_Camera->SetOldMouseX(SCREEN_WIDTH / 2);
	m_Camera->SetOldMouseY(SCREEN_HEIGHT / 2);
}

void UE::GameEngine::Update()
{

}

void UE::GameEngine::Draw()
{
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glEnable(GL_DEPTH_TEST);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	m_Skybox->Draw(m_Camera);
	m_ModelRenderer->Draw(m_Camera);

	SDL_GL_SwapWindow(m_Window);
}

void UE::GameEngine::Free()
{
	m_ModelRenderer->Free();
	delete m_Model;
	delete m_Camera;
	delete m_Skybox;
	SDL_DestroyWindow(m_Window);
	m_Window = nullptr;
	SDL_Quit();
}

void UE::GameEngine::SetWindowTitle(const char* title)
{
	SDL_SetWindowTitle(m_Window, title);
}

void UE::DisplayInfoMessages(const char* msg)
{
	SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Unrealistic Engine", msg, nullptr);
}

