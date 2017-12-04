#pragma once
#ifndef TEXTURESHADER_H
#define TEXTURESHADER_H
#include "Shader.h"
#include <glm\gtc\type_ptr.hpp>

class TextureShader : public Shader {
public:
	TextureShader(const char* vertexPath, const char* fragmentPath) :
		Shader(vertexPath, fragmentPath) {}

	void setLight(const float &ka, const float &kd, const float &ks, const float &shininess, const glm::vec3 &lightDir, const glm::vec3 &lightColor) {
		setFloat("dirlight.direction", lightDir.x, lightDir.y, lightDir.z);
		setFloat("material.shininess", shininess);
		setFloat("dirlight.ambient", lightColor.x * ka, lightColor.y * ka, lightColor.z * ka);
		setFloat("dirlight.diffuse", lightColor.x * kd, lightColor.y * kd, lightColor.z * kd);
		setFloat("dirlight.specular", lightColor.x * ks, lightColor.y * ks, lightColor.z * ks);
	}
	void setMatrix(const glm::mat4 &projection, const glm::vec4 &view, const glm::vec3 viewPos, const glm::mat4 &model) {
		setMat4("projection", glm::value_ptr(projection));
		setFloat("view", view.x, view.y, view.z, view.w);
		setFloat("viewPos", viewPos.x, viewPos.y, viewPos.z);
		setMat4("model", glm::value_ptr(model));
	}
	void setModel(const glm::mat4 &model) {
		setMat4("model", glm::value_ptr(model));
	}
};

#endif // !TEXTURESHADER_H
