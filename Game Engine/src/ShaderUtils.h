#pragma once
#include <GL/glew.h>

namespace UE {
	bool CompileProgram(const GLchar* v_shader_sourcecode[], const GLchar* f_shader_sourcecode[], GLuint* programId);
	void DisplayShaderCompilerError(GLuint shaderId);
}