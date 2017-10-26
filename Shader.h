#pragma once
#ifndef SHADER_H
#define SHADER_H

class Shader {
	enum{SHADER, PROGRAM};
public:
	Shader(const char* vertexPath, const char* fragmentPath);
	//Shader(const Shader &oldShader);
	void use();
	void setFloat(const char* uniform, float param1);
	void setFloat(const char* uniform, float param1, float param2);
	void setFloat(const char* uniform, float param1, float param2, float param3);
	void setFloat(const char* uniform, float param1, float param2, float param3, float param4);
	void setMat4(const char* uniform, const float *mat);
	void setInt(const char* uniform, const int param1);

private:
	unsigned int shaderProgram, vertexShader, fragmentShader;
	void checkError(unsigned int shader, const int type);
};

#endif // !SHADER_H
