#pragma once
#ifndef SHADER_H
#define SHADER_H

#include <glad\glad.h>

class Shader {
	enum{SHADER, PROGRAM};
public:
	Shader(const GLchar* vertexPath, const GLchar* fragmentPath);
	void use();
	void setFloat(const char* uniform, GLfloat param1);
	void setFloat(const char* uniform, GLfloat param1, GLfloat param2);
	void setFloat(const char* uniform, GLfloat param1, GLfloat param2, GLfloat param3);
	void setFloat(const char* uniform, GLfloat param1, GLfloat param2, GLfloat param3, GLfloat param4);

private:
	GLuint shaderProgram;
	void checkError(GLuint shader, const int type);
};

#endif // !SHADER_H
