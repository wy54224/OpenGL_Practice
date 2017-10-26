#pragma once
#include "rectangle.h"
#ifndef CUBE_H
#define CUBE_H
namespace Object {
	class Cube {
		std::vector<Mesh> face;
		float L;
		glm::vec3 p[8];
	public:
		/*
		  7____ 6
		4/|___5/|
		||3__|_|2
		|/ __|/
		0      1
		*/
		Cube(const float &_L) :
			L(_L / 2) {
			p[0] = glm::vec3(-L, -L, L);
			p[1] = glm::vec3(L, -L, L);
			p[2] = glm::vec3(L, -L, -L);
			p[3] = glm::vec3(-L, -L, -L);
			p[4] = glm::vec3(-L, L, L);
			p[5] = glm::vec3(L, L, L);
			p[6] = glm::vec3(L, L, -L);
			p[7] = glm::vec3(-L, L, -L);
		};
		void setSide(const unsigned int &diffuse, const unsigned int &specular) {
			setForward(diffuse, specular);
			setBackward(diffuse, specular);
			setLeft(diffuse, specular);
			setRight(diffuse, specular);
		}
		void setTB(const unsigned int &diffuse, const unsigned int &specular) {
			setTop(diffuse, specular);
			setBottom(diffuse, specular);
		}
		void setAll(const unsigned int &diffuse, const unsigned int &specluar) {
			setSide(diffuse, specluar);
			setTB(diffuse, specluar);
		}
		void setForward(const unsigned int &diffuse, const unsigned int &specular) {
			face.push_back(getRectangle(p[0], p[1], p[4], p[5], 1, diffuse, specular));
		}
		void setBackward(const unsigned int &diffuse, const unsigned int &specular) {
			face.push_back(getRectangle(p[2], p[3], p[6], p[7], 1, diffuse, specular));
		}
		void setLeft(const unsigned int &diffuse, const unsigned int &specular) {
			face.push_back(getRectangle(p[3], p[0], p[7], p[4], 1, diffuse, specular));
		}
		void setRight(const unsigned int &diffuse, const unsigned int &specular) {
			face.push_back(getRectangle(p[1], p[2], p[5], p[6], 1, diffuse, specular));
		}
		void setTop(const unsigned int &diffuse, const unsigned int &specular) {
			face.push_back(getRectangle(p[4], p[5], p[7], p[6], 1, diffuse, specular));
		}
		void setBottom(const unsigned int &diffuse, const unsigned int &specular) {
			face.push_back(getRectangle(p[3], p[2], p[0], p[1], 1, diffuse, specular));
		}
		void Draw(Shader &shader) {
			for (int i = 0; i < face.size(); ++i)
				face[i].Draw(shader);
		}
	};

	class ColorCube {
		std::vector<ColorMesh> face;
		float L;
		glm::vec3 p[8];
	public:
		/*
		7____ 6
		4/|___5/|
		||3__|_|2
		|/ __|/
		0      1
		*/
		ColorCube(const float &_L) :
			L(_L / 2) {
			p[0] = glm::vec3(-L, -L, L);
			p[1] = glm::vec3(L, -L, L);
			p[2] = glm::vec3(L, -L, -L);
			p[3] = glm::vec3(-L, -L, -L);
			p[4] = glm::vec3(-L, L, L);
			p[5] = glm::vec3(L, L, L);
			p[6] = glm::vec3(L, L, -L);
			p[7] = glm::vec3(-L, L, -L);
		};
		void setSide(const glm::vec3 &color) {
			setForward(color);
			setBackward(color);
			setLeft(color);
			setRight(color);
		}
		void setTB(const glm::vec3 &color) {
			setTop(color);
			setBottom(color);
		}
		void setAll(const glm::vec3 &color) {
			setSide(color);
			setTB(color);
		}
		void setForward(const glm::vec3 &color) {
			face.push_back(getRectangle(p[0], p[1], p[4], p[5], color));
		}
		void setBackward(const glm::vec3 &color) {
			face.push_back(getRectangle(p[2], p[3], p[6], p[7], color));
		}
		void setLeft(const glm::vec3 &color) {
			face.push_back(getRectangle(p[3], p[0], p[7], p[4], color));
		}
		void setRight(const glm::vec3 &color) {
			face.push_back(getRectangle(p[1], p[2], p[5], p[6], color));
		}
		void setTop(const glm::vec3 &color) {
			face.push_back(getRectangle(p[4], p[5], p[7], p[6], color));
		}
		void setBottom(const glm::vec3 &color) {
			face.push_back(getRectangle(p[3], p[2], p[0], p[1], color));
		}
		void Draw(Shader &shader) {
			for (int i = 0; i < face.size(); ++i)
				face[i].Draw(shader);
		}
	};
}
#endif // !CUBE_H
