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
			glGetProgramInfoLog(programID, msgLen, nullptr, msg);
			std::cerr << "Error linking program: " << msg << "\n";

			delete[] msg;
		}
	}

	std::string LoadShaderSourceCode(const std::string filename)
	{
		std::ifstream ifs(filename);

		if (!ifs.is_open()) {
			std::cerr << "Problem opening file: " << filename << " Check file is in the directory" << std::endl;
			return std::string("No shader");
		}

		std::string shader_source((std::istreambuf_iterator<char>(ifs)),
			(std::istreambuf_iterator<char>()));

		return shader_source;
	}

	bool CompileProgram(const GLuint& v_id, std::vector<const GLchar*>& v_shader_sourcecode, const GLuint& f_id, std::vector<const GLchar*>& f_shader_sourcecode, GLuint& programId) {
		glShaderSource(v_id, 1, v_shader_sourcecode.data(), nullptr);
		glCompileShader(v_id);

		GLint isShaderCompiledOK = GL_FALSE;
		glGetShaderiv(v_id, GL_COMPILE_STATUS, &isShaderCompiledOK);
		if (isShaderCompiledOK != GL_TRUE) {
			std::cerr << "Unable to compile vertex shader" << std::endl;
			DisplayShaderCompilationError(v_id);
			return false;
		}

		glShaderSource(f_id, 1, f_shader_sourcecode.data(), nullptr);
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

	bool CompileProgram(const GLchar* v_shader_sourcecode[], const GLchar* f_shader_sourcecode[], GLuint* programId) {
		// Create the vertex shader first.
		// Order doesn't matter but shaders must be created and compiled before
		// attaching to program
		GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);

		// Copy the source to OpenGL ready for compilation
		glShaderSource(vertexShader, 1, v_shader_sourcecode, nullptr);

		// Compile the code
		glCompileShader(vertexShader);

		// Check for compiler errors
		// Presume shader didn't compile
		GLint isShaderCompiledOK = GL_FALSE;

		// Get the compile status from OpenGL
		glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &isShaderCompiledOK);

		// Has the shader failed to compile?
		if (isShaderCompiledOK != GL_TRUE) {
			// Yes, so display an error message
			std::cerr << "Unable to compile vertex shader" << std::endl;

			DisplayProgramCompilationError(vertexShader);

			return false;
		}

		// Do the same for the fragment shader
		GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

		// Transfer the shader code
		glShaderSource(fragmentShader, 1, f_shader_sourcecode, NULL);

		// Compile it
		glCompileShader(fragmentShader);

		// Check for errors.  Code is same as above of getting status
		// and displaying error message, if necessary
		isShaderCompiledOK = GL_FALSE;
		glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &isShaderCompiledOK);

		if (isShaderCompiledOK != GL_TRUE) {
			std::cerr << "Unable to compile fragment shader" << std::endl;

			DisplayProgramCompilationError(fragmentShader);

			return false;
		}

		// Create the program object
		*programId = glCreateProgram();

		// Attach shaders to the program object
		glAttachShader(*programId, vertexShader);
		glAttachShader(*programId, fragmentShader);

		// Now link the program to create an executable program we
		// and use to render the object
		// Program executable will exist in graphics memory
		glLinkProgram(*programId);

		// Check for linking errors
		GLint isProgramLinked = GL_FALSE;
		glGetProgramiv(*programId, GL_LINK_STATUS, &isProgramLinked);
		if (isProgramLinked != GL_TRUE) {
			std::cerr << "Failed to link program" << std::endl;

			return false;
		}

		// Got this far so must be okay, return true
		return true;
	}
} // namespace UE