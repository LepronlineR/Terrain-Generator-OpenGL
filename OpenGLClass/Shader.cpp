#include "Shader.h"

// online solution
std::string getFile(const char* filename) {
	std::ifstream in(filename);
	if (in) {
		std::string str((std::istreambuf_iterator<char>(in)),
			std::istreambuf_iterator<char>());
		return str;
	}
	throw(errno);
}

Shader::Shader(const char* vertexFile, const char* fragFile) {
	std::string vertex = getFile(vertexFile);
	std::string frag = getFile(fragFile);
	// convert to char* for opengl
	const char* vertexSource = vertex.c_str();
	const char* fragSource = frag.c_str();

	// ============================ SHADERS ============================
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexSource, NULL);
	glCompileShader(vertexShader);
	compile(vertexShader, "VERTEX");

	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragSource, NULL);
	glCompileShader(fragmentShader);
	compile(fragmentShader, "FRAGMENT");

	Shader::ID = glCreateProgram();
	glAttachShader(Shader::ID, fragmentShader);
	glAttachShader(Shader::ID, vertexShader);
	glLinkProgram(Shader::ID);
	compile(Shader::ID, "PROGRAM");

	glDeleteShader(vertexShader);
	glDeleteShader(vertexShader);

}

// Checks if the shaders have compiled properly
void Shader::compile(unsigned int shader, const char* type) {
	GLint hasCompiled;
	char infoLog[1024];
	if (type == "PROGRAM") {
		glGetProgramiv(shader, GL_LINK_STATUS, &hasCompiled);
		if (hasCompiled == GL_FALSE) {
			glGetProgramInfoLog(shader, 1024, NULL, infoLog);
			std::cout << "Shader linking error: " << type << "\n" << infoLog << std::endl;
		}
	} else {
		glGetShaderiv(shader, GL_COMPILE_STATUS, &hasCompiled);
		if (hasCompiled == GL_FALSE) {
			glGetShaderInfoLog(shader, 1024, NULL, infoLog);
			std::cout << "Shader compiling error: " << type << "\n" << infoLog << std::endl;
		}
	}
}

void Shader::Activate() {
	glUseProgram(Shader::ID);
}

void Shader::Delete() {
	glDeleteProgram(Shader::ID);
}

GLuint Shader::getID() {
	return Shader::ID;
}
