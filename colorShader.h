#pragma once
#include <glm\glm.hpp>
#include <glm\gtc\type_ptr.hpp>
#include "Shader.h"
#ifndef COLORSHADER_H
#define COLORSHADER_H

class ColorShader : public Shader {
public:
	ColorShader(const char* vertexPath, const char* fragmentPath) :
		Shader(vertexPath, fragmentPath) {}
	void setMatrix(const glm::mat4 &projection, const glm::vec4 &view, const glm::vec3 viewPos, const glm::mat4 &model) {
		setMat4("projection", glm::value_ptr(projection));
		setFloat("view", view.x, view.y, view.z, view.w);
		setFloat("viewPos", viewPos.x, viewPos.y, viewPos.z);
		setMat4("model", glm::value_ptr(model));
	}
	void setLight(const float &ka, const float &kd, const float &ks, const float &shininess, const glm::vec3 &lightDir, const glm::vec3 &lightColor) {
		setFloat("Ka", ka);
		setFloat("Kd", kd);
		setFloat("Ks", ks);
		setFloat("Shininess", shininess);
		setFloat("direction", lightDir.x, lightDir.y, lightDir.z);
		setFloat("lightColor", lightColor.x, lightColor.y, lightColor.z);
	}
};
#endif // !COLORSHADER_H
