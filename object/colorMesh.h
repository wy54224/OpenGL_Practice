#pragma once
#ifndef COLORMESH_H
#define COLORMESH_H
#include <glm\glm.hpp>
#include <string>
#include <vector>
#include "Shader.h"
struct ColorVertex
{
	glm::vec3 Position;
	glm::vec3 Color;
	ColorVertex(const glm::vec3 &_Position = glm::vec3(0), const glm::vec3 &_Color = glm::vec3(0)) :
		Position(_Position),
		Color(_Color) {}
};

class ColorMesh
{
public:
	std::vector<ColorVertex> vertices;
	std::vector<unsigned int>indices;

	ColorMesh(const std::vector<ColorVertex>&_vertices, const std::vector<unsigned int>&_indices);
	void Draw(Shader &shader);

private:
	unsigned int VAO, VBO, EBO;
	void setupMesh();
};

#endif // !MESH_H
