#pragma once
#include "Camera.h"
#include "Utils.h"
#include "Utils.h"
#include <GL/glew.h>
#include <SDL_opengl.h>
#include <vector>
#include <array>
#include <string>
#include <SDL_image.h>
#include <iostream>
#include <glm/gtc/type_ptr.hpp>

namespace UE {
	class Skybox {
	public:

		// Filenames
		Skybox(std::string front, std::string back,
			std::string left, std::string right,
			std::string top, std::string bottom)
		{
			std::array<std::string, 6> filenames;
			filenames[0] = g_SkyboxDirectory + right;
			filenames[1] = g_SkyboxDirectory + left;
			filenames[2] = g_SkyboxDirectory + top;
			filenames[3] = g_SkyboxDirectory + bottom;
			filenames[4] = g_SkyboxDirectory + front;
			filenames[5] = g_SkyboxDirectory + back;

			CreateCubemap(filenames);
			CreateCubeVBO();
			CreateSkyboxProgram();
		}

		~Skybox() {}

		void Draw(std::shared_ptr<Camera> camera);
		void Free();

	private:
		void CreateCubemap(std::array<std::string, 6> filenames);
		void CreateCubeVBO();
		void CreateSkyboxProgram();

	private:
		GLuint m_SkyboxCubeMapName;
		GLuint m_SkyboxProgramID;
		GLuint m_VboSkybox;
		GLint m_VertexLocation;

		GLuint m_ViewUniformID;
		GLuint m_ProjectionUniformID;
		GLuint m_SamplerID;
	};
}