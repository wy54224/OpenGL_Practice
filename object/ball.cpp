#include "ball.h"
#include <glad\glad.h>
#include <cmath>
#include <iostream>
#define STB_IMAGE_IMPLEMENTATION
#include <stb\stb_image.h>
#define PI 3.1415926535897932384626

BALL::BALL(const int &m, const int &n, const float & r, const char *path)
{
	double AngZ = 0, AngXY = 0, IncAngZ = PI / m, IncAngXY = PI * 2 / n;
	int index = 0, vNum = (m + 1) * (n + 1) * 3, iNum = n * m * 6, tNum = (m + 1) * (n + 1) * 2, index2 = 0;
	this->count = iNum;
	GLfloat *vertices = new GLfloat[vNum];
	GLfloat *textures = new GLfloat[tNum];
	GLuint *indices = new GLuint[iNum];
	for (int i = 0; i <= m; ++i) {
		double RsinZ = r * sin(AngZ), RcosZ = r * cos(AngZ);
		float tX = i / (float)m;
		for (int j = 0; j <= n; ++j) {
			vertices[index++] = RsinZ * cos(AngXY);
			vertices[index++] = RsinZ * sin(AngXY);
			vertices[index++] = RcosZ;
			textures[index2++] = j / (float)n;
			textures[index2++] = tX;
			AngXY += IncAngXY;
		}
		AngXY = 0;
		AngZ += IncAngZ;
	}
	index = 0;
	for(int i = 1; i <= m; ++i)
		for (int j = 1; j <= n; ++j) {
			int left_top, left_buttom, right_top, right_buttom;
			right_buttom = i * (n + 1) + j;
			left_buttom = right_buttom - 1;
			right_top = right_buttom - n - 1;
			left_top = right_top - 1;
			indices[index++] = left_top;
			indices[index++] = left_buttom;
			indices[index++] = right_top;
			indices[index++] = right_top;
			indices[index++] = left_buttom;
			indices[index++] = right_buttom;
		}

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	GLuint VBO[2], EBO;
	glGenBuffers(2, VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
	glBufferData(GL_ARRAY_BUFFER, vNum * sizeof(GLfloat), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
	glBufferData(GL_ARRAY_BUFFER, tNum * sizeof(GLfloat), textures, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(1);
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, iNum * sizeof(GLuint), indices, GL_STATIC_DRAW);
	int width, height, nrChannels;
	unsigned char *data = stbi_load(path, &width, &height, &nrChannels, 0);
	if (data) {
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
		stbi_image_free(data);
	}
	else {
		std::cout << "Failed to load texture" << std::endl;
	}
	delete[] vertices;
	delete[] indices;
}

BALL::~BALL()
{
	glDeleteTextures(1, &texture);
	glDeleteVertexArrays(1, &VAO);
}

void BALL::Draw()
{
	glBindTexture(GL_TEXTURE_2D, this->texture);
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, this->count, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}
