#include "Skybox.h"

namespace UE {
	constexpr float SIDE = 100.0F;

	std::array<glm::vec3, 36> cube = {
		//back
		glm::vec3(-SIDE,  SIDE, -SIDE),
		glm::vec3(-SIDE, -SIDE, -SIDE),
		glm::vec3(SIDE, -SIDE, -SIDE),

		glm::vec3(SIDE,  SIDE, -SIDE),
		glm::vec3(-SIDE, SIDE, -SIDE),
		glm::vec3(SIDE, -SIDE, -SIDE),

		//front
		glm::vec3(-SIDE,  SIDE, SIDE),
		glm::vec3(-SIDE, -SIDE, SIDE),
		glm::vec3(SIDE, -SIDE, SIDE),

		glm::vec3(SIDE,  SIDE, SIDE),
		glm::vec3(-SIDE, SIDE, SIDE),
		glm::vec3(SIDE, -SIDE, SIDE),

		//top
		glm::vec3(-SIDE,  SIDE, SIDE),
		glm::vec3(-SIDE, SIDE, -SIDE),
		glm::vec3(SIDE, SIDE, -SIDE),

		glm::vec3(SIDE,  SIDE, SIDE),
		glm::vec3(-SIDE, SIDE, SIDE),
		glm::vec3(SIDE, SIDE, -SIDE),

		//bottom
		glm::vec3(-SIDE,  -SIDE, SIDE),
		glm::vec3(-SIDE, -SIDE, -SIDE),
		glm::vec3(SIDE, -SIDE, -SIDE),

		glm::vec3(SIDE,  -SIDE, SIDE),
		glm::vec3(-SIDE, -SIDE, SIDE),
		glm::vec3(SIDE, -SIDE, -SIDE),

		//right
		glm::vec3(SIDE, -SIDE, SIDE),
		glm::vec3(SIDE, -SIDE, -SIDE),
		glm::vec3(SIDE, SIDE, -SIDE),

		glm::vec3(SIDE,  SIDE, SIDE),
		glm::vec3(SIDE, -SIDE, SIDE),
		glm::vec3(SIDE, SIDE, -SIDE),

		//left
		glm::vec3(-SIDE, -SIDE, SIDE),
		glm::vec3(-SIDE, -SIDE, -SIDE),
		glm::vec3(-SIDE, SIDE, -SIDE),

		glm::vec3(-SIDE,  SIDE, SIDE),
		glm::vec3(-SIDE, -SIDE, SIDE),
		glm::vec3(-SIDE, SIDE, -SIDE),
	};

	Skybox::Skybox(std::string front, std::string back,
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

	Skybox::~Skybox()
	{
		glDeleteProgram(m_RenderData.m_SkyboxProgramID);
		glDeleteBuffers(1, &m_RenderData.m_VboSkybox);
		glDeleteTextures(1, &m_RenderData.m_SkyboxCubeMapName);
	}

	void Skybox::Draw(const std::shared_ptr<Camera>& camera)
	{
		bool isDepthTestEnable = glIsEnabled(GL_DEPTH_TEST);
		glDisable(GL_DEPTH_TEST);

		glm::mat4 cameraView = camera->GetViewMatrix();
		glm::mat4 projection = camera->GetProjectionMatrix();

		cameraView[3][0] = 0.0F;
		cameraView[3][1] = 0.0F;
		cameraView[3][2] = 0.0F;

		glUseProgram(m_RenderData.m_SkyboxProgramID);
		glUniformMatrix4fv(m_RenderData.m_ViewUniformID, 1, GL_FALSE, glm::value_ptr(cameraView));
		glUniformMatrix4fv(m_RenderData.m_ProjectionUniformID, 1, GL_FALSE, glm::value_ptr(projection));

		glBindBuffer(GL_ARRAY_BUFFER, m_RenderData.m_VboSkybox);

		glEnableVertexAttribArray(m_RenderData.m_VertexLocation);
		glVertexAttribPointer(m_RenderData.m_VertexLocation, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), reinterpret_cast<void*>(offsetof(glm::vec3, x)));

		glActiveTexture(GL_TEXTURE0);
		glUniform1i(m_RenderData.m_SamplerID, 0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, m_RenderData.m_SkyboxCubeMapName);
		glDrawArrays(GL_TRIANGLES, 0, sizeof(cube) / sizeof(glm::vec3));
		glDisableVertexAttribArray(m_RenderData.m_VertexLocation);
		glUseProgram(0);
		if (isDepthTestEnable) {
			glEnable(GL_DEPTH_TEST);
		}
	}

	void Skybox::CreateCubemap(std::array<std::string, 6> filenames)
	{
		glGenTextures(1, &m_RenderData.m_SkyboxCubeMapName);
		glBindTexture(GL_TEXTURE_CUBE_MAP, m_RenderData.m_SkyboxCubeMapName);

		for (int face = 0; face < 6; face++)
		{
			SDL_Surface* surfaceImage = IMG_Load(filenames[face].c_str());
			if (surfaceImage == nullptr) return;
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
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + face, 0,
				format,
				surfaceImage->w, surfaceImage->h,
				0,
				format,
				GL_UNSIGNED_BYTE,
				surfaceImage->pixels
			);
			SDL_FreeSurface(surfaceImage);
		}

		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

		glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	}

	void Skybox::CreateCubeVBO()
	{
		glGenBuffers(1, &m_RenderData.m_VboSkybox);
		glBindBuffer(GL_ARRAY_BUFFER, m_RenderData.m_VboSkybox);
		glBufferData(GL_ARRAY_BUFFER, sizeof(cube), cube.data(), GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, m_RenderData.m_VboSkybox);
	}

	void Skybox::CreateSkyboxProgram()
	{
		GLuint v_id = glCreateShader(GL_VERTEX_SHADER);
		std::string v_shader_source = LoadShaderSourceCode(g_ShaderDirectory + "skybox.vert");
		std::vector<const GLchar*> v_shader_array = { v_shader_source.c_str() };

		GLuint f_id = glCreateShader(GL_FRAGMENT_SHADER);
		std::string f_shader_source = LoadShaderSourceCode(g_ShaderDirectory + "skybox.frag");
		std::vector<const GLchar*> f_shader_array = { f_shader_source.c_str() };

		bool result = CompileProgram(v_id, v_shader_array, f_id, f_shader_array, m_RenderData.m_SkyboxProgramID);
		if (!result) {
			std::cerr << "Failed to create skybox renderer program. Check console for errors: \n";
			return;
		}

		m_RenderData.m_VertexLocation = glGetAttribLocation(m_RenderData.m_SkyboxProgramID, "vertexPos3D");
		if (m_RenderData.m_VertexLocation == -1) {
			std::cerr << "Problem getting vertex3dPos\n";
			return;
		}

		m_RenderData.m_ProjectionUniformID = glGetUniformLocation(m_RenderData.m_SkyboxProgramID, "projection");
		m_RenderData.m_SamplerID = glGetUniformLocation(m_RenderData.m_SkyboxProgramID, "sampler");
		m_RenderData.m_ViewUniformID = glGetUniformLocation(m_RenderData.m_SkyboxProgramID, "view");

		glDeleteShader(v_id);
		glDeleteShader(f_id);
	}
} // namespace UE