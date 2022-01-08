#pragma once
#include <GL/glew.h>
#include "CrossPlatform.h"

#include "Camera.h"
#include "Utils.h"
#include "Utils.h"
#include <vector>
#include <array>
#include <string>
#include <iostream>
#include <glm/gtc/type_ptr.hpp>
#include <memory>

namespace UE {
	struct SkyboxRenderData {
		GLuint m_SkyboxCubeMapName		= 0;
		GLuint m_SkyboxProgramID		= 0;
		GLuint m_VboSkybox				= 0;
		GLint m_VertexLocation			= 0;
										
		GLuint m_ViewUniformID			= 0;
		GLuint m_ProjectionUniformID	= 0;
		GLuint m_SamplerID				= 0;
	};

	class Skybox {
	public:

		Skybox(std::string front, std::string back,
			std::string left, std::string right,
			std::string top, std::string bottom);

		~Skybox();

		void Draw(const std::shared_ptr<Camera>& camera);

	private:
		void CreateCubemap(std::array<std::string, 6> filenames);
		void CreateCubeVBO();
		void CreateSkyboxProgram();

	private:
		SkyboxRenderData m_RenderData;
	};
}