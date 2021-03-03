#include "SkyboxRenderer.h"
#include "ShaderUtils.h"
#include <SDL_image.h>
#include <iostream>
#include <glm/gtc/type_ptr.hpp>

namespace UE {

	struct CubeVertex {
		float x, y, z;

		CubeVertex() { x = y = z = 0.0f; }
		CubeVertex(float x, float y, float z) : x(x), y(y), z(z) {}
	};

	const float SIDE = 100.0f;

	CubeVertex cube[] = {
		//back
		CubeVertex(-SIDE,  SIDE, -SIDE),
		CubeVertex(-SIDE, -SIDE, -SIDE),
		CubeVertex(SIDE, -SIDE, -SIDE),

		CubeVertex(SIDE,  SIDE, -SIDE),
		CubeVertex(-SIDE, SIDE, -SIDE),
		CubeVertex(SIDE, -SIDE, -SIDE),

		//front
		CubeVertex(-SIDE,  SIDE, SIDE),
		CubeVertex(-SIDE, -SIDE, SIDE),
		CubeVertex(SIDE, -SIDE, SIDE),

		CubeVertex(SIDE,  SIDE, SIDE),
		CubeVertex(-SIDE, SIDE, SIDE),
		CubeVertex(SIDE, -SIDE, SIDE),

		//top
		CubeVertex(-SIDE,  SIDE, SIDE),
		CubeVertex(-SIDE, SIDE, -SIDE),
		CubeVertex(SIDE, SIDE, -SIDE),

		CubeVertex(SIDE,  SIDE, SIDE),
		CubeVertex(-SIDE, SIDE, SIDE),
		CubeVertex(SIDE, SIDE, -SIDE),

		//bottom
		CubeVertex(-SIDE,  -SIDE, SIDE),
		CubeVertex(-SIDE, -SIDE, -SIDE),
		CubeVertex(SIDE, -SIDE, -SIDE),

		CubeVertex(SIDE,  -SIDE, SIDE),
		CubeVertex(-SIDE, -SIDE, SIDE),
		CubeVertex(SIDE, -SIDE, -SIDE),

		//right
		CubeVertex(SIDE, -SIDE, SIDE),
		CubeVertex(SIDE, -SIDE, -SIDE),
		CubeVertex(SIDE, SIDE, -SIDE),

		CubeVertex(SIDE,  SIDE, SIDE),
		CubeVertex(SIDE, -SIDE, SIDE),
		CubeVertex(SIDE, SIDE, -SIDE),

		//left
		CubeVertex(-SIDE, -SIDE, SIDE),
		CubeVertex(-SIDE, -SIDE, -SIDE),
		CubeVertex(-SIDE, SIDE, -SIDE),

		CubeVertex(-SIDE,  SIDE, SIDE),
		CubeVertex(-SIDE, -SIDE, SIDE),
		CubeVertex(-SIDE, SIDE, -SIDE),
	};

	void SkyboxRenderer::Draw(Camera* camera)
	{
		bool isDepthTestEnable = glIsEnabled(GL_DEPTH_TEST);
		glDisable(GL_DEPTH_TEST);

		glm::mat4 cameraView = camera->GetViewMatrix();
		glm::mat4 projection = camera->GetProjectionMatrix();

		cameraView[3][0] = 0.0f;
		cameraView[3][1] = 0.0f;
		cameraView[3][2] = 0.0f;

		glUseProgram(m_SkyboxProgramID);
		glUniformMatrix4fv(m_ViewUniformID, 1, GL_FALSE, glm::value_ptr(cameraView));
		glUniformMatrix4fv(m_ProjectionUniformID, 1, GL_FALSE, glm::value_ptr(projection));

		glBindBuffer(GL_ARRAY_BUFFER, m_VboSkybox);

		glEnableVertexAttribArray(m_VertexLocation);
		glVertexAttribPointer(m_VertexLocation, 3, GL_FLOAT, GL_FALSE, sizeof(CubeVertex), (void*)offsetof(CubeVertex, x));

		glActiveTexture(GL_TEXTURE0);
		glUniform1i(m_SamplerID, 0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, m_SkyboxCubeMapName);
		glDrawArrays(GL_TRIANGLES, 0, sizeof(cube) / sizeof(CubeVertex));
		glDisableVertexAttribArray(m_VertexLocation);
		glUseProgram(0);
		if (isDepthTestEnable) {
			glEnable(GL_DEPTH_TEST);
		}
	}

	void SkyboxRenderer::Free()
	{
		glDeleteProgram(m_SkyboxProgramID);
		glDeleteBuffers(1, &m_VboSkybox);
		glDeleteTextures(1, &m_SkyboxCubeMapName);
	}

	void SkyboxRenderer::CreateCubemap(std::array<std::string,6> filenames)
	{
		glGenTextures(1, &m_SkyboxCubeMapName);
		glBindTexture(GL_TEXTURE_CUBE_MAP, m_SkyboxCubeMapName);

		for (int faceNum = 0; faceNum < 6; faceNum++)
		{
			SDL_Surface* surfaceImage = IMG_Load(filenames[faceNum].c_str());
			if (surfaceImage == nullptr) {
				return;
			}
			GLenum format = surfaceImage->format->format;
			switch (format) {
			case SDL_PIXELFORMAT_RGBA32:
				format = GL_RGBA;
				break;
			case SDL_PIXELFORMAT_RGB24:
				format = GL_RGB;
				break;
			default:
				format = GL_RGB;
				break;
			}
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + faceNum, 0, format,
				surfaceImage->w, surfaceImage->h, 0, format,
				GL_UNSIGNED_BYTE, surfaceImage->pixels);
			SDL_FreeSurface(surfaceImage);
		}
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

		glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

	}

	void SkyboxRenderer::CreateCubeVBO()
	{
		glGenBuffers(1, &m_VboSkybox);
		glBindBuffer(GL_ARRAY_BUFFER, m_VboSkybox);
		glBufferData(GL_ARRAY_BUFFER, sizeof(cube), cube, GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, m_VboSkybox);
	}

	void SkyboxRenderer::CreateSkyboxProgram()
	{
		const GLchar* V_ShaderCode[] = {
			"#version 140\n"
			"in vec3 vertexPos3D;\n"
			"out vec3 texCoord;\n"
			"uniform mat4 view;\n"
			"uniform mat4 projection;\n"
			"void main(){\n"
				"vec4 v = vec4(vertexPos3D.xyz, 1);\n"
				"v = projection * view * v;\n"
				"gl_Position = v;\n"
				"texCoord = vertexPos3D;\n"
			"}\n"
		};

		const GLchar* F_ShaderCode[] = {
			"#version 140\n"
			"in vec3 texCoord;\n"
			"uniform samplerCube sampler;\n"
			"out vec4 fragmentColour;\n"
			"void main(){\n"
				"fragmentColour = vec4(texture(sampler, texCoord).rgb, 1.0f);\n"
			"}\n"
		};

		bool result = CompileProgram(V_ShaderCode, F_ShaderCode, &m_SkyboxProgramID);
		if (!result) {
			std::cerr << "Failed tocreate skybox renderer program. Check console for errors: \n";
			return;
		}

		m_VertexLocation = glGetAttribLocation(m_SkyboxProgramID, "vertexPos3D");
		if (m_VertexLocation == -1) {
			std::cerr << "Problem getting vertex3dPos\n";
		}

		m_ViewUniformID = glGetUniformLocation(m_SkyboxProgramID, "view");
		m_ProjectionUniformID = glGetUniformLocation(m_SkyboxProgramID, "projection");
		m_SamplerID = glGetUniformLocation(m_SkyboxProgramID, "sampler");
	}
}