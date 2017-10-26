#pragma once
#ifndef MESH_H
#define MESH_H
#include <glm\glm.hpp>
#include <string>
#include <vector>
#include "Shader.h"
struct Vertex
{
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoord;
	Vertex(const glm::vec3 &_Position = glm::vec3(0), const glm::vec3 &_Normal = glm::vec3(0), const glm::vec2 &_TexCoord = glm::vec2(0)):
		Position(_Position),
		Normal(_Normal),
		TexCoord(_TexCoord){}
};

struct Texture {
	unsigned int id;
	std::string type;
	Texture(const unsigned int &_id = 0, const std::string &_type = ""):
		id(_id),
		type(_type){}
};

class Mesh
{
public:
	std::vector<Vertex> vertices;
	std::vector<unsigned int>indices;
	std::vector<Texture> textures;

	Mesh(const std::vector<Vertex>&_vertices, const std::vector<unsigned int>&_indices, const std::vector<Texture>&_textures);
	void Draw(Shader &shader);
	static unsigned int Load(const std::string &path);
	static unsigned int Load(const char *path);

private:
	unsigned int VAO, VBO, EBO;
	void setupMesh();
};

#endif // !MESH_H
