#pragma once
#ifndef RECTANGLE_H
#define RECTANGLE_H
#include "mesh.h"
#include "colorMesh.h"

namespace Object {
	Mesh getRectangle(const glm::vec3 &LB, const glm::vec3 &RB, const glm::vec3 &LT, const glm::vec3 &RT, const float &texL, unsigned int diffuse, unsigned int specular) {
		std::vector<Vertex>vertices;
		std::vector<unsigned int>indices;
		std::vector<Texture>textures;
		glm::vec3 normal = glm::normalize(glm::cross(RB - LB, RT - LB));
		vertices.push_back(Vertex(LB, normal, glm::vec2(0, 0)));
		vertices.push_back(Vertex(RB, normal, glm::vec2(texL, 0)));
		vertices.push_back(Vertex(LT, normal, glm::vec2(0, texL)));
		vertices.push_back(Vertex(RT, normal, glm::vec2(texL, texL)));
		indices.push_back(0);
		indices.push_back(1);
		indices.push_back(2);
		indices.push_back(1);
		indices.push_back(3);
		indices.push_back(2);
		textures.push_back(Texture(diffuse, "diffuse"));
		textures.push_back(Texture(specular, "specular"));
		return Mesh(vertices, indices, textures);
	}

	ColorMesh getRectangle(const glm::vec3 &LB, const glm::vec3 &RB, const glm::vec3 &LT, const glm::vec3 &RT, const glm::vec3 &color) {
		std::vector<ColorVertex>vertices;
		std::vector<unsigned int>indices;
		vertices.push_back(ColorVertex(LB, color));
		vertices.push_back(ColorVertex(RB, color));
		vertices.push_back(ColorVertex(LT, color));
		vertices.push_back(ColorVertex(RT, color));
		indices.push_back(0);
		indices.push_back(1);
		indices.push_back(2);
		indices.push_back(1);
		indices.push_back(3);
		indices.push_back(2);
		return ColorMesh(vertices, indices);
	}

	class Ground : public Mesh {
	public:
		Ground(const int &L, const int &texL, const unsigned int &diffuse, const unsigned int &specular):
			Mesh(getRectangle(glm::vec3(-L, 0, L), glm::vec3(L, 0, L), glm::vec3(-L, 0, -L), glm::vec3(L, 0, -L), texL, diffuse, specular)){}
	};

	class ColorGround : public ColorMesh{
	public:
		ColorGround(const int &L, const glm::vec3 &color) :
			ColorMesh(getRectangle(glm::vec3(-L, 0, L), glm::vec3(L, 0, L), glm::vec3(-L, 0, -L), glm::vec3(L, 0, -L), color)){}
	};
#endif // !RECTANGLE_H
}
