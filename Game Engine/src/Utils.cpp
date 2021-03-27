#include "Utils.h"

namespace UE {
	void DisplayShaderCompilationError(GLuint shaderID) {
		GLint MsgLen = 0;

		glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &MsgLen);
		if (MsgLen > 1) {
			GLchar* Msg = new GLchar[MsgLen + 1];
			glGetShaderInfoLog(shaderID, MsgLen, nullptr, Msg);
			std::cerr << "Error compiling shader" << Msg << std::endl;

			delete[] Msg;
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

	std::string LoadShaderSourceCode(std::string filename)
	{
		std::ifstream ifs(filename);

		if (!ifs.is_open()) {
			std::cerr << "Problem opening file: " << filename << " Check file is in the directory" << std::endl;
			return std::string("no shader");
		}

		std::string shader_source((std::istreambuf_iterator<char>(ifs)),
			(std::istreambuf_iterator<char>()));

		return shader_source;
	}

	bool CompileProgram(const GLuint& v_id, const GLchar* v_shader_sourcecode[], const GLuint& f_id, const GLchar* f_shader_sourcecode[], GLuint& programId) {
		glShaderSource(v_id, 1, v_shader_sourcecode, NULL);
		glCompileShader(v_id);

		GLint isShaderCompiledOK = GL_FALSE;
		glGetShaderiv(v_id, GL_COMPILE_STATUS, &isShaderCompiledOK);
		if (isShaderCompiledOK != GL_TRUE) {
			std::cerr << "Unable to compile vertex shader" << std::endl;
			DisplayShaderCompilationError(v_id);
			return false;
		}

		glShaderSource(f_id, 1, f_shader_sourcecode, NULL);
		glCompileShader(f_id);

		isShaderCompiledOK = GL_FALSE;
		glGetShaderiv(f_id, GL_COMPILE_STATUS, &isShaderCompiledOK);
		if (isShaderCompiledOK != GL_TRUE) {
			std::cerr << "Unable to compile fragment shader" << std::endl;
			DisplayShaderCompilationError(f_id);
			return false;
		}

		programId = glCreateProgram();

		glAttachShader(programId, v_id);
		glAttachShader(programId, f_id);

		glLinkProgram(programId);

		GLint isProgramLinked = GL_FALSE;
		glGetProgramiv(programId, GL_LINK_STATUS, &isProgramLinked);
		if (isProgramLinked != GL_TRUE) {
			std::cerr << "Failed to link program" << std::endl;

			return false;
		}

		return true;
	}
}