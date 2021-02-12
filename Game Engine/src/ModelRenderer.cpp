#include "ModelRenderer.h"
#include <iostream>
#include <glm/gtc/type_ptr.hpp>
#include <vector>

namespace UE {

	void DisplayShaderCompilationError(GLuint shaderID) {
		GLint msgLen = 0;
		glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &msgLen);
		if (msgLen > 0) {
			GLchar* msg = new GLchar[int64_t(msgLen) + 1];
			glGetShaderInfoLog(shaderID, msgLen, NULL, msg);
			std::cerr << "Error compiling shader: " << msg << "\n";

			delete[] msg;
		}
	}
	void DisplayProgramCompilationError(GLuint programID) {
		GLint msgLen = 0;
		glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &msgLen);
		if (msgLen > 0) {
			GLchar* msg = new GLchar[int64_t(msgLen) + 1];
			glGetProgramInfoLog(programID, msgLen, NULL, msg);
			std::cerr << "Error linking program: " << msg << "\n";

			delete[] msg;
		}
	}

	UE::ModelRenderer::ModelRenderer(Model* model)
	{
		m_Position = { 0.0f, 0.0f, 0.0f };
		m_Rotation = { 0.0f, 0.0f, 0.0f };
		m_Scale = { 1.0f, 1.0f, 1.0f };
		m_Model = model;
	}

	UE::ModelRenderer::~ModelRenderer()
	{
	}

	void UE::ModelRenderer::Init()
	{
		GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
		/*	const GLchar* vertexShaderCode[] = {
			"#version 140\n"
			"in vec3 vertexPos3D;\n"
			"in vec4 vColour;\n"
			"out vec4 fColour;\n"
			"uniform mat4 transform;\n"
			"uniform mat4 view;\n"
			"uniform mat4 projection;\n"
			"void main() {\n"
				"vec4 v = vec4(vertexPos3D.xyz, 1);\n"
				"v = projection * view * transform * v;\n"
				"gl_Position = v;\n"
				"fColour = vColour;\n"
			"}\n"
			};*/

		const GLchar* vertexShaderCode[] = {
			"#version 140\n"
			"in vec3 vertexPos3D;\n"
			"in vec2 vUV;\n"
			"out vec2 uv;\n"
			"uniform mat4 transform;\n"
			"uniform mat4 view;\n"
			"uniform mat4 projection;\n"
			"void main() {\n"
				"vec4 v = vec4(vertexPos3D.xyz, 1);\n"
				"v = projection * view * transform * v;\n"
				"gl_Position = v;\n"
				"uv = vUV;\n"
			"}\n"
		};

		glShaderSource(vertexShader, 1, vertexShaderCode, NULL);
		glCompileShader(vertexShader);

		GLint shaderCompiled = GL_FALSE;
		glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &shaderCompiled);
		if (shaderCompiled != GL_TRUE) {
			std::cerr << "Unable to compile vertex shader: \n";
			DisplayShaderCompilationError(vertexShader);
			return;
		}

		GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
		//const GLchar* fragmentShaderCode[] = {
		//	"#version 140\n"
		//	"in vec4 fColour;\n"
		//	"out vec4 fragmentColour;\n"
		//	"void main()"
		//	"{"
		//		"fragmentColour = fColour;"
		//	"}"
		//};
		const GLchar* fragmentShaderCode[] = {
			"#version 140\n"
			"in vec2 uv;\n"
			"uniform sampler2D sampler;\n"
			"out vec4 fragmentColour;\n"
			"void main()"
			"{"
				"fragmentColour = texture(sampler, uv).rgba;"
			"}"
		};

		glShaderSource(fragmentShader, 1, fragmentShaderCode, NULL);
		glCompileShader(fragmentShader);

		shaderCompiled = GL_FALSE;
		glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &shaderCompiled);
		if (shaderCompiled != GL_TRUE) {
			std::cerr << "Unable to compile fragment shader: \n";
			DisplayShaderCompilationError(fragmentShader);
			return;
		}


		m_ProgramID = glCreateProgram();

		glAttachShader(m_ProgramID, vertexShader);
		glAttachShader(m_ProgramID, fragmentShader);

		glLinkProgram(m_ProgramID);

		GLint programLinked = GL_FALSE;
		glGetProgramiv(m_ProgramID, GL_LINK_STATUS, &programLinked);
		if (programLinked != GL_TRUE) {
			std::cerr << "Failed to link program!\n";
			DisplayProgramCompilationError(m_ProgramID);
			return;
		}

		m_VertexPos3DLocation = glGetAttribLocation(m_ProgramID, "vertexPos3D");
		if (m_VertexPos3DLocation == -1) {
			std::cerr << "Problem getting vertexPos3D\n";
		}

		m_VertexUVLocation = glGetAttribLocation(m_ProgramID, "vUV");
		if (m_VertexUVLocation == -1) {
			std::cerr << "Problem getting vUV\n";
		}

		m_TransformUniformID = glGetUniformLocation(m_ProgramID, "transform");
		m_ViewUniformID = glGetUniformLocation(m_ProgramID, "view");
		m_ProjectionUniformID = glGetUniformLocation(m_ProgramID, "projection");
		m_SamplerID = glGetUniformLocation(m_ProgramID, "sampler");

		glGenBuffers(1, &m_VboModel);
		glBindBuffer(GL_ARRAY_BUFFER, m_VboModel);

		glBufferData(GL_ARRAY_BUFFER, m_Model->GetNumVertices() * sizeof(Vertex), m_Model->GetVertices(), GL_STATIC_DRAW);
	}

	void UE::ModelRenderer::Update()
	{
	}

	void UE::ModelRenderer::Draw(Camera* camera)
	{
		glEnable(GL_CULL_FACE);

		glm::mat4 transformationMatrix = glm::mat4(1.0f);
		transformationMatrix = glm::translate(transformationMatrix, glm::vec3(m_Position.x, m_Position.y, m_Position.z));
		transformationMatrix = glm::rotate(transformationMatrix, glm::radians(m_Rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
		transformationMatrix = glm::rotate(transformationMatrix, glm::radians(m_Rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
		transformationMatrix = glm::rotate(transformationMatrix, glm::radians(m_Rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
		transformationMatrix = glm::scale(transformationMatrix, glm::vec3(m_Scale.x, m_Scale.y, m_Scale.z));

		glm::mat4 viewMatrix = camera->GetViewMatrix();
		glm::mat4 projectionMatrix = camera->GetProjectionMatrix();

		glUseProgram(m_ProgramID);

		glUniformMatrix4fv(m_TransformUniformID, 1, GL_FALSE, glm::value_ptr(transformationMatrix));
		glUniformMatrix4fv(m_ViewUniformID, 1, GL_FALSE, glm::value_ptr(viewMatrix));
		glUniformMatrix4fv(m_ProjectionUniformID, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

		glVertexAttribPointer(m_VertexPos3DLocation, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, x));
		glEnableVertexAttribArray(m_VertexPos3DLocation);

		glVertexAttribPointer(m_VertexUVLocation, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, u));
		glEnableVertexAttribArray(m_VertexUVLocation);

		glBindBuffer(GL_ARRAY_BUFFER, m_VboModel);

		glActiveTexture(GL_TEXTURE0);
		glUniform1i(m_SamplerID, 0);
		glBindTexture(GL_TEXTURE_2D, m_Texture->GetTextureName());

		glDrawArrays(GL_TRIANGLES, 0, m_Model->GetNumVertices());

		glDisableVertexAttribArray(m_VertexPos3DLocation);
		glDisableVertexAttribArray(m_VertexUVLocation);

		glUseProgram(0);

		glDisable(GL_CULL_FACE);
	}

	void UE::ModelRenderer::Free()
	{
		glDeleteProgram(m_ProgramID);
		glDeleteBuffers(1, &m_VboModel);
	}
}