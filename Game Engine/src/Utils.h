#pragma once
#include <GL/glew.h>
#include <iostream>
#include <fstream>
#include <vector>

namespace UE {
	bool CompileProgram(const GLuint& v_id, std::vector<const GLchar*>& v_shader_sourcecode, const GLuint& f_id, std::vector<const GLchar*>& f_shader_sourcecode, GLuint& programId);
	void DisplayShaderCompilationError(GLuint shaderID);
	void DisplayProgramCompilationError(GLuint programID);
	std::string LoadShaderSourceCode(std::string filename);

	const std::string g_ShaderDirectory = "./resources/shaders/";
	const std::string g_ModelDirectory = "./resources/models/";
	const std::string g_TextureDirectory = "./resources/textures/";
	const std::string g_SkyboxDirectory = "./resources/skybox/";
	const std::string g_SoundDirectory = "./resources/sound/";
}