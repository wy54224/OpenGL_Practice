#include <Shader.h>
#include <glad\glad.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

Shader::Shader(const char* vertexPath, const char* fragmentPath) {
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
		return;
	}

	const char* vShaderCode = vertexCode.c_str();
	const char* fShaderCode = fragmentCode.c_str();

	this->vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vShaderCode, NULL);
	glCompileShader(vertexShader);
	checkError(vertexShader, SHADER);
	this->fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fShaderCode, NULL);
	glCompileShader(fragmentShader);
	checkError(fragmentShader, SHADER);
	this->shaderProgram = glCreateProgram();
	glAttachShader(this->shaderProgram, this->vertexShader);
	glAttachShader(this->shaderProgram, this->fragmentShader);
	glLinkProgram(this->shaderProgram);
	checkError(this->shaderProgram, PROGRAM);
	glDeleteShader(this->vertexShader);
	glDeleteShader(this->fragmentShader);
	this->vertexShader = -1;
	this->fragmentShader = -1;
}

/*Shader::Shader(const Shader &oldShader)
{
	if (oldShader.vertexShader < 0 || oldShader.fragmentShader < 0) {
		this->shaderProgram = -1;
		this->vertexShader = -1;
		this->fragmentShader = -1;
		std::cout << "An error appear in class Shader copy constructor" << std::endl;
		return;
	}
	this->shaderProgram = glCreateProgram();
	glAttachShader(this->shaderProgram, oldShader.vertexShader);
	glAttachShader(this->shaderProgram, oldShader.fragmentShader);
	glLinkProgram(this->shaderProgram);
	this->vertexShader = oldShader.vertexShader;
	this->fragmentShader = oldShader.fragmentShader;
	checkError(this->shaderProgram, PROGRAM);
}*/

void Shader::use() {
	glUseProgram(this->shaderProgram);
}

void Shader::checkError(unsigned int shader, const int type) {
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
		glGetProgramiv(shader, GL_LINK_STATUS, &success);
		if (!success) {
			glGetProgramInfoLog(shader, 512, NULL, infoLog);
			std::cout << infoLog << std::endl;
		}
	}
}

void Shader::setFloat(const char* uniform, float param1)
{
	use();
	glUniform1f(glGetUniformLocation(this->shaderProgram, uniform), param1);
}

void Shader::setFloat(const char* uniform, float param1, float param2)
{
	use();
	glUniform2f(glGetUniformLocation(this->shaderProgram, uniform), param1, param2);
}

void Shader::setFloat(const char* uniform, float param1, float param2, float param3)
{
	use();
	glUniform3f(glGetUniformLocation(this->shaderProgram, uniform), param1, param2, param3);
}

void Shader::setFloat(const char* uniform, float param1, float param2, float param3, float param4)
{
	use();
	glUniform4f(glGetUniformLocation(this->shaderProgram, uniform), param1, param2, param3, param4);
}

void Shader::setMat4(const char * uniform, const float * mat)
{
	use();
	glUniformMatrix4fv(glGetUniformLocation(this->shaderProgram, uniform), 1, GL_FALSE, mat);
}

void Shader::setInt(const char * uniform, const int param1)
{
	use();
	glUniform1i(glGetUniformLocation(this->shaderProgram, uniform), param1);
}
