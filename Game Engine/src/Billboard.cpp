#include "Billboard.h"

namespace UE {
	Vertex billboard[] = {
		Vertex(0.5f, 1.0f, 0.0f, 1.0f, 1.0f),
		Vertex(-0.5f, 1.0f, 0.0f, 0.0f, 1.0f),
		Vertex(-0.5f, 0.0f, 0.0f, 0.0f, 0.0f),

		Vertex(-0.5f, 0.0f, 0.0f, 0.0f, 0.0f),
		Vertex(0.5f, 0.0f, 0.0f, 1.0f, 0.0f),
		Vertex(0.5f, 1.0f, 0.0f, 1.0f, 1.0f)
	};

	void BillboardRenderer::Init()
	{
		GLuint v_id = glCreateShader(GL_VERTEX_SHADER);
		std::string v_shader_source = LoadShaderSourceCode(g_ShaderDirectory + "billboard.vert");
		const GLchar* v_source_array[] = { v_shader_source.c_str() };

		GLuint f_id = glCreateShader(GL_FRAGMENT_SHADER);
		std::string f_shader_source = LoadShaderSourceCode(g_ShaderDirectory + "billboard.frag");
		const GLchar* f_source_array[] = { f_shader_source.c_str() };

		if (!CompileProgram(v_id, v_source_array, f_id, f_source_array, programId)) {
			std::cerr << "Problem building billboard program.  Check console log for more information." << std::endl;
		}

		vertexLocation = glGetAttribLocation(programId, "vertexPos3D");
		if (vertexLocation == -1) { std::cerr << "Problem getting vertex3DPos" << std::endl; }

		vertexUVLocation = glGetAttribLocation(programId, "vUV");
		if (vertexUVLocation == -1) { std::cerr << "Problem getting vUV" << std::endl; }

		transformUniformId = glGetUniformLocation(programId, "transform");
		viewUniformId = glGetUniformLocation(programId, "view");
		projectionUniformId = glGetUniformLocation(programId, "projection");
		samplerId = glGetUniformLocation(programId, "sampler");

		glGenBuffers(1, &vboQuad);
		glBindBuffer(GL_ARRAY_BUFFER, vboQuad);

		glBufferData(GL_ARRAY_BUFFER, sizeof(billboard), billboard, GL_STATIC_DRAW);
	}

	void BillboardRenderer::Draw()
	{
		glEnable(GL_CULL_FACE);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glm::vec3 rotation = glm::vec3(0, 0, 0);

		glm::mat4 transformationMat = glm::mat4(1.0f);

		transformationMat = glm::translate(transformationMat, m_Billboard->GetPosition());

		transformationMat = glm::rotate(transformationMat,
			glm::radians(270 - m_Camera->GetYaw()),
			glm::vec3(0.0f, 1.0f, 0.0f));

		transformationMat = glm::scale(transformationMat, glm::vec3(m_Billboard->GetScale().x, m_Billboard->GetScale().y, 0.0f));

		glm::mat4 viewMat = m_Camera->GetViewMatrix();
		glm::mat4 projectionMat = m_Camera->GetProjectionMatrix();

		glUseProgram(programId);

		glUniformMatrix4fv(transformUniformId, 1, GL_FALSE, glm::value_ptr(transformationMat));
		glUniformMatrix4fv(viewUniformId, 1, GL_FALSE, glm::value_ptr(viewMat));
		glUniformMatrix4fv(projectionUniformId, 1, GL_FALSE, glm::value_ptr(projectionMat));

		glEnableVertexAttribArray(vertexLocation);
		glBindBuffer(GL_ARRAY_BUFFER, vboQuad);

		glVertexAttribPointer(vertexLocation, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, x));

		glEnableVertexAttribArray(vertexUVLocation);
		glVertexAttribPointer(vertexUVLocation, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, u));

		glActiveTexture(GL_TEXTURE0);
		glUniform1i(samplerId, 0);
		glBindTexture(GL_TEXTURE_2D, m_Billboard->GetTexture()->GetTextureName());

		glDrawArrays(GL_TRIANGLES, 0, 6);

		glDisableVertexAttribArray(vertexLocation);
		glDisableVertexAttribArray(vertexUVLocation);

		glUseProgram(0);

		glDisable(GL_BLEND);
		glDisable(GL_CULL_FACE);
	}

	Billboard::Billboard(std::string texturePath, std::shared_ptr<Camera> camera)
	{
		m_Camera = camera;
		m_Texture = std::make_unique<Texture>((g_TextureDirectory + texturePath).c_str());
		m_Position = { 0.0f,0.0f,0.0f };
		m_Scale = { 0.0f,0.0f,0.0f };
	}

	void Billboard::Init()
	{
		m_Renderer = std::make_unique<BillboardRenderer>(shared_from_this(), m_Camera);
		m_Renderer->Init();
	}

	void Billboard::Draw()
	{
		m_Renderer->Draw();
	}
}