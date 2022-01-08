#include "Billboard.h"
#include "Texture.h"
#include "Camera.h"
#include "Vertex.h"
#include "Utils.h"

#include <array>
#include <vector>

namespace UE {
	std::array<Vertex<float>, 6> square = {
		Vertex<float>(0.5F, 1.0F, 0.0F, 1.0F, 1.0F),
		Vertex<float>(-0.5F, 1.0F, 0.0F, 0.0F, 1.0F),
		Vertex<float>(-0.5F, 0.0F, 0.0F, 0.0F, 0.0F),

		Vertex<float>(-0.5F, 0.0F, 0.0F, 0.0F, 0.0F),
		Vertex<float>(0.5F, 0.0F, 0.0F, 1.0F, 0.0F),
		Vertex<float>(0.5F, 1.0F, 0.0F, 1.0F, 1.0F)
	};

	Billboard::Billboard(std::string texturePath, std::shared_ptr<Camera> camera)
	{
		m_Camera = camera;
		m_Texture = std::make_unique<Texture>((g_TextureDirectory + texturePath).c_str());
		m_Position = { 0.0F,0.0F,0.0F };
		m_Scale = { 0.0F,0.0F,0.0F };

		GLuint v_id = glCreateShader(GL_VERTEX_SHADER);
		std::string v_shader_source = LoadShaderSourceCode(g_ShaderDirectory + "billboard.vert");
		std::vector<const GLchar*> v_source_array = { v_shader_source.c_str() };

		GLuint f_id = glCreateShader(GL_FRAGMENT_SHADER);
		std::string f_shader_source = LoadShaderSourceCode(g_ShaderDirectory + "billboard.frag");
		std::vector<const GLchar*> f_source_array = { f_shader_source.c_str() };

		if (!CompileProgram(v_id, v_source_array, f_id, f_source_array, m_RenderData.programId)) {
			std::cerr << "Problem building billboard program.  Check console log for more information." << std::endl;
		}

		m_RenderData.vertexLocation = glGetAttribLocation(m_RenderData.programId, "vertexPos3D");
		if (m_RenderData.vertexLocation == -1) { std::cerr << "Problem getting vertex3DPos" << std::endl; }

		m_RenderData.vertexUVLocation = glGetAttribLocation(m_RenderData.programId, "vUV");
		if (m_RenderData.vertexUVLocation == -1) { std::cerr << "Problem getting vUV" << std::endl; }

		m_RenderData.transformUniformId = glGetUniformLocation(m_RenderData.programId, "transform");
		m_RenderData.viewUniformId = glGetUniformLocation(m_RenderData.programId, "view");
		m_RenderData.projectionUniformId = glGetUniformLocation(m_RenderData.programId, "projection");
		m_RenderData.samplerId = glGetUniformLocation(m_RenderData.programId, "sampler");

		glGenBuffers(1, &m_RenderData.vboQuad);
		glBindBuffer(GL_ARRAY_BUFFER, m_RenderData.vboQuad);

		glBufferData(GL_ARRAY_BUFFER, sizeof(square), square.data(), GL_STATIC_DRAW);
	}

	void Billboard::Draw()
	{
		glEnable(GL_CULL_FACE);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glm::vec3 rotation = glm::vec3(0, 0, 0);

		glm::mat4 transformationMat = glm::mat4(1.0F);

		transformationMat = glm::translate(transformationMat, m_Position);

		transformationMat = glm::rotate(transformationMat,
			glm::radians(270 - m_Camera->GetYaw()),
			glm::vec3(0.0F, 1.0F, 0.0F));

		transformationMat = glm::scale(transformationMat, glm::vec3(m_Scale.x, m_Scale.y, 0.0F));

		glm::mat4 viewMat = m_Camera->GetViewMatrix();
		glm::mat4 projectionMat = m_Camera->GetProjectionMatrix();

		glUseProgram(m_RenderData.programId);

		glUniformMatrix4fv(m_RenderData.transformUniformId, 1, GL_FALSE, glm::value_ptr(transformationMat));
		glUniformMatrix4fv(m_RenderData.viewUniformId, 1, GL_FALSE, glm::value_ptr(viewMat));
		glUniformMatrix4fv(m_RenderData.projectionUniformId, 1, GL_FALSE, glm::value_ptr(projectionMat));

		glEnableVertexAttribArray(m_RenderData.vertexLocation);
		glBindBuffer(GL_ARRAY_BUFFER, m_RenderData.vboQuad);

		glVertexAttribPointer(m_RenderData.vertexLocation, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex<float>), reinterpret_cast<void*>(offsetof(Vertex<float>, x)));

		glEnableVertexAttribArray(m_RenderData.vertexUVLocation);
		glVertexAttribPointer(m_RenderData.vertexUVLocation, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex<float>), reinterpret_cast<void*>(offsetof(Vertex<float>, u)));

		glActiveTexture(GL_TEXTURE0);
		glUniform1i(m_RenderData.samplerId, 0);
		glBindTexture(GL_TEXTURE_2D, m_Texture->GetTextureName());

		glDrawArrays(GL_TRIANGLES, 0, 6);

		glDisableVertexAttribArray(m_RenderData.vertexLocation);
		glDisableVertexAttribArray(m_RenderData.vertexUVLocation);

		glUseProgram(0);

		glDisable(GL_BLEND);
		glDisable(GL_CULL_FACE);
	}
} // namespace UE