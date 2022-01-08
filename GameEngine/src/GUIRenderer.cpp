#include "GUIRenderer.h"
#include "Utils.h"
#include "Vertex.h"
#include "CrossPlatform.h"
#include <array>
#include <glm/glm.hpp>
#include <iostream>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GL/glew.h>

namespace UE {

	GUIRenderer::GUIRenderer(int window_width, int window_height) {
		std::string v_shader_source = LoadShaderSourceCode(g_ShaderDirectory + "GUI.vert");
		std::string f_shader_source = LoadShaderSourceCode(g_ShaderDirectory + "GUI.frag");

		const GLchar* v_source_array[] = { v_shader_source.c_str() };
		const GLchar* f_source_array[] = { f_shader_source.c_str() };

		if (!CompileProgram(v_source_array, f_source_array, &m_RenderData.programId)) {
			std::cerr << "Problem building billboard program.  Check console log for more information." << std::endl;
		}

		m_RenderData.vertexLocation = glGetAttribLocation(m_RenderData.programId, "vertexPos3D");

		if (m_RenderData.vertexLocation == -1) {
			std::cerr << "Problem getting vertex3DPos" << std::endl;
		}

		m_RenderData.vertexUVLocation = glGetAttribLocation(m_RenderData.programId, "vUV");

		if (m_RenderData.vertexUVLocation == -1) {
			std::cerr << "Problem getting vUV" << std::endl;
		}

		m_RenderData.projectionUniformId = glGetUniformLocation(m_RenderData.programId, "projection");
		m_RenderData.samplerId = glGetUniformLocation(m_RenderData.programId, "sampler");

		m_RenderData.projectionMat = glm::ortho(0.0f, (float)window_width, (float)window_height, 0.0f);
	}

	void GUIRenderer::DrawTexture(int x, int y, std::shared_ptr<Texture> texture)
	{
		float textureWidth = static_cast<float>(texture->GetWidth());
		float textureHeight = static_cast<float>(texture->GetHeight());

		float xPos = static_cast<float>(x);
		float yPos = static_cast<float>(y);

		GLuint vboTextQuad;

		std::array<Vertex<float>, 6> textureQuad = {
			Vertex<float>(xPos, yPos, 0.0f, 0.0f, 1.0f),
			Vertex<float>(xPos, yPos + textureHeight, 0.0f, 0.0f, 0.0f),
			Vertex<float>(xPos + textureWidth, yPos + textureHeight, 0.0f, 1.0f, 0.0f),

			Vertex<float>(xPos + textureWidth, yPos + textureHeight, 0.0f, 1.0f, 0.0f),
			Vertex<float>(xPos + textureWidth, yPos, 0.0f, 1.0f, 1.0f),
			Vertex<float>(xPos, yPos, 0.0f, 0.0f, 1.0f)
		};

		glGenBuffers(1, &vboTextQuad);
		glBindBuffer(GL_ARRAY_BUFFER, vboTextQuad);
		glBufferData(GL_ARRAY_BUFFER, sizeof(textureQuad), textureQuad.data(), GL_STATIC_DRAW);

		glEnable(GL_BLEND);
		glDisable(GL_DEPTH_TEST);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glUseProgram(m_RenderData.programId);

		glUniformMatrix4fv(m_RenderData.projectionUniformId, 1, GL_FALSE, glm::value_ptr(m_RenderData.projectionMat));
		glEnableVertexAttribArray(m_RenderData.vertexLocation);
		glBindBuffer(GL_ARRAY_BUFFER, vboTextQuad);

		glVertexAttribPointer(m_RenderData.vertexLocation, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex<float>), (void*)offsetof(Vertex<float>, x));
		glEnableVertexAttribArray(m_RenderData.vertexUVLocation);
		glVertexAttribPointer(m_RenderData.vertexUVLocation, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex<float>), (void*)offsetof(Vertex<float>, u));

		glActiveTexture(GL_TEXTURE0);
		glUniform1i(m_RenderData.samplerId, 0);
		glBindTexture(GL_TEXTURE_2D, texture->GetTextureName());

		glDrawArrays(GL_TRIANGLES, 0, 6);
		glBindTexture(GL_TEXTURE_2D, 0);

		glDisableVertexAttribArray(m_RenderData.vertexLocation);
		glDisableVertexAttribArray(m_RenderData.vertexUVLocation);

		glUseProgram(0);

		glDeleteBuffers(1, &vboTextQuad);

		glEnable(GL_DEPTH_TEST);
		glDisable(GL_BLEND);
	}
}
