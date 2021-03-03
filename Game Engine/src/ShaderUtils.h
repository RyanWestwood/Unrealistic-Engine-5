#pragma once
#include <GL/glew.h>
#include <iostream>

namespace UE {
	bool CompileProgram(const GLchar* v_shader_sourcecode[], const GLchar* f_shader_sourcecode[], GLuint* programId);
	void DisplayShaderCompilerError(GLuint shaderId);
	std::string LoadShaderSourceCode(std::string filename);
}