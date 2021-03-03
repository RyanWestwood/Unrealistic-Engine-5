#include <glm/glm.hpp>
#include <iostream>
#include "BillboardRenderer.h"
#include "ShaderUtils.h"
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace UE {

	struct Vertex1 {
		// Location
		float x, y, z;
		float u, v;

		// Constructors
		// Sets vertex to passed values
		Vertex1(float _x, float _y, float _z, float _u, float _v) {
			// Location
			x = _x;
			y = _y;
			z = _z;

			// Colour
			u = _u;
			v = _v;
		}

		// Default, set to 0.0
		Vertex1() {
			x = y = z = 0.0f;
			u = v = 0.0f;
		}
	};

	Vertex1 billboard[] = {
		Vertex1(0.5f, 1.0f, 0.0f, 1.0f, 1.0f),
		Vertex1(-0.5f, 1.0f, 0.0f, 0.0f, 1.0f),
		Vertex1(-0.5f, 0.0f, 0.0f, 0.0f, 0.0f),
			  
		Vertex1(-0.5f, 0.0f, 0.0f, 0.0f, 0.0f),
		Vertex1(0.5f, 0.0f, 0.0f, 1.0f, 0.0f),
		Vertex1(0.5f, 1.0f, 0.0f, 1.0f, 1.0f)
	};

	BillboardRenderer::BillboardRenderer()
	{

	}

	void BillboardRenderer::Init()
	{
		std::string shaderDir = "resources/shaders/";
		std::string v_shader_source = LoadShaderSourceCode("resources/shaders/billboard.vs");
		std::string f_shader_source = LoadShaderSourceCode("resources/shaders/billboard.fs");

		const GLchar* v_source_array[] = { v_shader_source.c_str() };
		const GLchar* f_source_array[] = { f_shader_source.c_str() };

		if (!CompileProgram(v_source_array, f_source_array, &programId)) {
			std::cerr << "Problem building billboard program.  Check console log for more information." << std::endl;
		}

		vertexLocation = glGetAttribLocation(programId, "vertexPos3D");
		if (vertexLocation == -1) {
			std::cerr << "Problem getting vertex3DPos" << std::endl;
		}
		vertexUVLocation = glGetAttribLocation(programId, "vUV");
		if (vertexUVLocation == -1) {
			std::cerr << "Problem getting vUV" << std::endl;
		}

		transformUniformId = glGetUniformLocation(programId, "transform");
		viewUniformId = glGetUniformLocation(programId, "view");
		projectionUniformId = glGetUniformLocation(programId, "projection");
		samplerId = glGetUniformLocation(programId, "sampler");

		glGenBuffers(1, &vboQuad);
		glBindBuffer(GL_ARRAY_BUFFER, vboQuad);

		glBufferData(GL_ARRAY_BUFFER, sizeof(billboard), billboard, GL_STATIC_DRAW);
	}

	void BillboardRenderer::Draw(Billboard* b, Camera* cam)
	{
		glEnable(GL_CULL_FACE);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glm::vec3 rotation = glm::vec3(0, 0, 0);

		glm::mat4 transformationMat = glm::mat4(1.0f);

		transformationMat = glm::translate(transformationMat, b->GetPosition());

		transformationMat = glm::rotate(transformationMat,
			glm::radians(360.0f - cam->GetYaw()),
			glm::vec3(0.0f, 1.0f, 0.0f));

		transformationMat = glm::scale(transformationMat, glm::vec3(b->GetScale().x, b->GetScale().y, 0.0f));

		glm::mat4 viewMat = cam->GetViewMatrix();
		glm::mat4 projectionMat = cam->GetProjectionMatrix();

		glUseProgram(programId);

		glUniformMatrix4fv(transformUniformId, 1, GL_FALSE, glm::value_ptr(transformationMat));
		glUniformMatrix4fv(viewUniformId, 1, GL_FALSE, glm::value_ptr(viewMat));
		glUniformMatrix4fv(projectionUniformId, 1, GL_FALSE, glm::value_ptr(projectionMat));

		glEnableVertexAttribArray(vertexLocation);
		glBindBuffer(GL_ARRAY_BUFFER, vboQuad);

		glVertexAttribPointer(vertexLocation, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex1), (void*)offsetof(Vertex1, x));

		glEnableVertexAttribArray(vertexUVLocation);
		glVertexAttribPointer(vertexUVLocation, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex1), (void*)offsetof(Vertex1, u));

		glActiveTexture(GL_TEXTURE0);
		glUniform1i(samplerId, 0);
		glBindTexture(GL_TEXTURE_2D, b->GetTexture()->GetTextureName());

		glDrawArrays(GL_TRIANGLES, 0, 6);

		glDisableVertexAttribArray(vertexLocation);
		glDisableVertexAttribArray(vertexUVLocation);

		glUseProgram(0);

		glDisable(GL_BLEND);
		glDisable(GL_CULL_FACE);
	}

}