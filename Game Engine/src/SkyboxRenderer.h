#pragma once
#include <GL/glew.h>
#include <SDL_opengl.h>
#include <vector>
#include <array>
#include <string>
#include "Camera.h"

namespace UE {

	class SkyboxRenderer {
	public: 

		// Filenames
		SkyboxRenderer( std::string front, std::string back,
						std::string left,  std::string right,
						std::string top,   std::string bottom ) 
		{

			std::array<std::string,6> filenames;
			filenames[0] = right;
			filenames[1] = left;
			filenames[2] = top;
			filenames[3] = bottom;
			filenames[4] = front;
			filenames[5] = back;

			CreateCubemap(filenames);
			CreateCubeVBO();
			CreateSkyboxProgram();
		}

		~SkyboxRenderer() {}

		void Draw(Camera* camera);
		void Free();

	private:
		void CreateCubemap(std::array<std::string,6> filenames);
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