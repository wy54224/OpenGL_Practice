#include <Shader.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

Shader::Shader(const GLchar* vertexPath, const GLchar* fragmentPath) {
	std::string vertexCode, fragmentCode;
	std::ifstream vertexFile, fragmentFile;
	vertexFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fragmentFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

	try {
		vertexFile.open(vertexPath);
		fragmentFile.open(fragmentPath);

		std::stringstream vertexStream, fragmentStream;
		vertexStream << vertexFile.rdbuf();
		fragmentStream << fragmentFile.rdbuf();
		vertexFile.close();
		fragmentFile.close();
		vertexCode = vertexStream.str();
		fragmentCode = fragmentStream.str();
	}
	catch (std::ifstream::failure) {
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULY_READ" << std::endl;
	}

	const char* vShaderCode = vertexCode.c_str();
	const char* fShaderCode = fragmentCode.c_str();

	GLuint vertexShader, fragmentShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vShaderCode, NULL);
	glCompileShader(vertexShader);
	checkError(vertexShader, SHADER);
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fShaderCode, NULL);
	glCompileShader(fragmentShader);
	checkError(fragmentShader, SHADER);
	this->shaderProgram = glCreateProgram();
	glAttachShader(this->shaderProgram, vertexShader);
	glAttachShader(this->shaderProgram, fragmentShader);
	glLinkProgram(this->shaderProgram);
	checkError(this->shaderProgram, PROGRAM);
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}

void Shader::use() {
	glUseProgram(this->shaderProgram);
}

void Shader::checkError(GLuint shader, const int type) {
	int success;
	char infoLog[512];
	if (type == SHADER) {
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
		if (!success) {
			glGetShaderInfoLog(shader, 512, NULL, infoLog);
			std::cout << infoLog << std::endl;
		}
	}else
	if (type == PROGRAM) {
		glGetShaderiv(shader, GL_LINK_STATUS, &success);
		if (!success) {
			glGetProgramInfoLog(shader, 512, NULL, infoLog);
			std::cout << infoLog << std::endl;
		}
	}
}

void Shader::setFloat(const char* uniform, GLfloat param1)
{
	glUniform1f(glGetUniformLocation(this->shaderProgram, uniform), param1);
}

void Shader::setFloat(const char* uniform, GLfloat param1, GLfloat param2)
{
	glUniform2f(glGetUniformLocation(this->shaderProgram, uniform), param1, param2);
}

void Shader::setFloat(const char* uniform, GLfloat param1, GLfloat param2, GLfloat param3)
{
	glUniform3f(glGetUniformLocation(this->shaderProgram, uniform), param1, param2, param3);
}

void Shader::setFloat(const char* uniform, GLfloat param1, GLfloat param2, GLfloat param3, GLfloat param4)
{
	glUniform4f(glGetUniformLocation(this->shaderProgram, uniform), param1, param2, param3, param4);
}
