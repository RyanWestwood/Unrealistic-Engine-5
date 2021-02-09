#include "GameEngine.h"
#include <iostream>

#define SCREEN_WIDTH 1024
#define SCREEN_HEIGHT 576
#define ASPECT_RATIO SCREEN_WIDTH / SCREEN_HEIGHT

Divide::GameEngine::GameEngine()
{
}

Divide::GameEngine::~GameEngine()
{
}

bool Divide::GameEngine::Init(bool vsync)
{
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		std::cerr << "Unable to init SDL sub-systems! SDL Error: " << SDL_GetError() << "\n";
		return false;
	}

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

	m_Window = SDL_CreateWindow("Divide", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
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

	m_Camera = new Camera(
		glm::vec3(0.0f, 0.0f, 5.0f),
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(0.0f, 1.0f, 0.0f),
		100.0f, ASPECT_RATIO, 0.1f, 1000.0f
	);

	m_Model = new Model();
	bool result = m_Model->LoadFromFile("resources/models/monkey.obj");
	if (!result) {
		std::cerr << "Failed to load model!\n";
	}

	m_ModelRenderer = new ModelRenderer(m_Model);
	m_ModelRenderer->Init();

	return true;
}

bool Divide::GameEngine::isRunning()
{
	SDL_PumpEvents();

	SDL_Event event;
	if (SDL_PeepEvents(&event, 1, SDL_GETEVENT, SDL_QUIT, SDL_QUIT)) {
		return false;
	}
	return true;
}

void Divide::GameEngine::Input()
{
}

void Divide::GameEngine::Update()
{
	m_ModelRenderer->SetRotation({ 0.0f, m_ModelRenderer->GetRotation().y + 1, 0.0f });
}

void Divide::GameEngine::Draw()
{
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	m_ModelRenderer->Draw(m_Camera);

	SDL_GL_SwapWindow(m_Window);
}

void Divide::GameEngine::Free()
{
	m_ModelRenderer->Free();
	delete m_Model;
	delete m_Camera;
	SDL_DestroyWindow(m_Window);
	m_Window = nullptr;
	SDL_Quit();
}

void Divide::GameEngine::SetWindowTitle(const char* title)
{
	SDL_SetWindowTitle(m_Window, title);
}

void Divide::DisplayInfoMessages(const char* msg)
{
	SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Basic Game Engine", msg, nullptr);
}

