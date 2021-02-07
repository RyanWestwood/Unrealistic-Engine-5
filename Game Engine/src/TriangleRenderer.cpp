#include "TriangleRenderer.h"
#include <iostream>
#include <glm/gtc/type_ptr.hpp>

GLfloat vertexData[] = {
	-1.0f, 0.0f,
	1.0f, 0.0f,
	0.0f, 1.0f
};

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

Divide::TriangleRenderer::TriangleRenderer()
{
	m_Position = { 0.0f, 0.0f, 0.0f };
	m_Rotation = { 0.0f, 0.0f, 0.0f };
	m_Scale    = { 1.0f, 1.0f, 1.0f };
}

Divide::TriangleRenderer::~TriangleRenderer()
{
}

void Divide::TriangleRenderer::Init()
{
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	//const GLchar* vertexShaderCode[] = {
	//	"#version 140\n"
	//	"in vec2 vertexPos2D;"
	//	"void main()"
	//	"{"
	//		"gl_Position = vec4(vertexPos2D.x, vertexPos2D.y, 0, 1);"
	//	"}"
	//};

	const GLchar* vertexShaderCode[] = {
	"#version 140\n"
	"in vec2 vertexPos2D;\n"
	"uniform mat4 transform;\n"
	"uniform mat4 view;\n"
	"uniform mat4 projection;\n"
	"void main()\n"
	"{\n"
		"vec4 v = vec4(vertexPos2D.x, vertexPos2D.y, 0, 1);\n"
		"v = projection * view * transform * v;\n"
		"gl_Position = v;\n"
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
	const GLchar* fragmentShaderCode[] = {
		"#version 140\n"
		"out vec4 fragmentColour;"
		"void main()"
		"{"
			"fragmentColour = vec4(1.0,0.0,0.0,1.0);"
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

	m_ProgramId = glCreateProgram();

	glAttachShader(m_ProgramId, vertexShader);
	glAttachShader(m_ProgramId, fragmentShader);

	glLinkProgram(m_ProgramId);

	GLint programLinked = GL_FALSE;
	glGetProgramiv(m_ProgramId, GL_LINK_STATUS, &programLinked);
	if (programLinked != GL_TRUE) {
		std::cerr << "Failed to link program!\n";
		DisplayProgramCompilationError(m_ProgramId);
		return;
	}

	m_VertexPos2DLocation = glGetAttribLocation(m_ProgramId, "vertexPos2D");
	if (m_VertexPos2DLocation == -1) {
		std::cerr << "Problem getting vertexPos2D\n";
	}

	m_TransformUniformID = glGetUniformLocation(m_ProgramId, "transform");
	m_ViewUniformID = glGetUniformLocation(m_ProgramId, "view");
	m_ProjectionUniformID = glGetUniformLocation(m_ProgramId, "projection");

	glGenBuffers(1, &m_VboTriangle);
	glBindBuffer(GL_ARRAY_BUFFER, m_VboTriangle);

	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_STATIC_DRAW);
}

void Divide::TriangleRenderer::Update()
{
}

void Divide::TriangleRenderer::Draw(Camera* camera)
{
	//	Should be TRS? or SRT?
	glm::mat4 transformationMatrix = glm::mat4(1.0f);
	transformationMatrix = glm::translate(transformationMatrix, glm::vec3(m_Position.x, m_Position.y, m_Position.z));
	transformationMatrix = glm::rotate(transformationMatrix, glm::radians(m_Rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
	transformationMatrix = glm::rotate(transformationMatrix, glm::radians(m_Rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
	transformationMatrix = glm::rotate(transformationMatrix, glm::radians(m_Rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
	transformationMatrix = glm::scale(transformationMatrix, glm::vec3(m_Scale.x, m_Scale.y, m_Scale.z));

	glm::mat4 viewMatrix = camera->GetViewMatrix();
	glm::mat4 projectionMatrix = camera->GetProjectionMatrix();

	glUseProgram(m_ProgramId);

	glUniformMatrix4fv(m_TransformUniformID, 1, GL_FALSE, glm::value_ptr(transformationMatrix));
	glUniformMatrix4fv(m_ViewUniformID, 1, GL_FALSE, glm::value_ptr(viewMatrix));
	glUniformMatrix4fv(m_ProjectionUniformID, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

	glVertexAttribPointer(m_VertexPos2DLocation, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GL_FLOAT), nullptr);
	glEnableVertexAttribArray(m_VertexPos2DLocation);
	glBindBuffer(GL_ARRAY_BUFFER, m_VboTriangle);

	glDrawArrays(GL_TRIANGLES, 0, 3);

	glEnableVertexAttribArray(m_VertexPos2DLocation);

	glUseProgram(0);
}

void Divide::TriangleRenderer::Free()
{
	glDeleteProgram(m_ProgramId);
	glDeleteBuffers(1, &m_VboTriangle);
}