#include "colorShader.h"
#include "TextureShader.h"
#include "rasterization.h"
#include "camera.h"
#include "object\ball.h"
#include "object\cube.h"
#include <GLFW\glfw3.h>
#include <iostream>
#include <imgui\imgui.h>
#include <imgui_impl_glfw_gl3.h>
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm\gtc\quaternion.hpp>
#include <glm\gtx\quaternion.hpp>
//OpenGL默认列主序

GLsizei SRC_WIDTH = 800;
GLsizei SRC_HEIGHT = 800;
const char* TITLE = "ImGUI";
Camera camera(glm::vec3(0));
GLfloat lastFrame = 0.0f, deltaTime = 0.0f;
int CursorMode = GLFW_CURSOR_NORMAL;

void processInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.CameraPositionMove(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.CameraPositionMove(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.CameraPositionMove(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.CameraPositionMove(RIGHT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

}

float lastX = SRC_WIDTH / 2, lastY = SRC_HEIGHT / 2, pitch = 0, yaw = 90.0f;
bool isFirstMoveCursor = true;
void mouse_callback(GLFWwindow* window, double xPos, double yPos) {
	if (isFirstMoveCursor) {
		lastX = xPos;
		lastY = yPos;
		isFirstMoveCursor = false;
	}
	//std::cout << xPos - lastX << ' ' << lastY - yPos << std::endl;
	//camera.CameraDirectionChange(xPos - lastX, lastY - yPos);
	lastX = xPos;
	lastY = yPos;
}

void scroll_callback(GLFWwindow *window, double xOffset, double yOffset) {
	camera.CameraZoomChange(yOffset);
}

void error_callback(int error, const char* description) {
	std::cout << "Error " << error << ": " << description << std::endl;
	system("pause");
}

void framebuffer_size_callback(GLFWwindow* window, GLsizei width, GLsizei height) {
	SRC_WIDTH = width;
	SRC_HEIGHT = height;
	glViewport((width - MIN(width, height)) / 2, (height - MIN(width, height)) / 2, MIN(width, height), MIN(width, height));
}

int main() {
	
	glfwSetErrorCallback(error_callback);
	if (!glfwInit()) {
		std::cout << "Failed to execute glfwInit()!" << std::endl;
		system("pause");
		return -1;
	}
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	GLFWwindow* window = glfwCreateWindow(SRC_WIDTH, SRC_HEIGHT, TITLE, NULL, NULL);
	if (!window) {
		std::cout << "Failed to create GLFW window!" << std::endl;
		glfwTerminate();
		system("pause");
		return -1;
	}
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwMakeContextCurrent(window);
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize glad!" << std::endl;
		system("pause");
		return -1;
	}
	ImGui_ImplGlfwGL3_Init(window, true);
	glViewport(0, 0, SRC_WIDTH, SRC_HEIGHT);
	glfwSetScrollCallback(window, scroll_callback);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	
	ColorShader phongShader("Shader/color.vert", "Shader/color.frag");
	TextureShader groundShader("Shader/texture.vert", "Shader/texture.frag");
	Shader simpleDepthShader("Shader/simpleDepth.vert", "Shader/simpleDepth.frag");
	Shader depthTestShader("Shader/simpleTexture.vert", "Shader/simpleTexture.frag");
	Object::ColorCube cube(1);
	Object::Ground ground(30, 40, Mesh::Load("Texture/woodDiffuse.jpg"), Mesh::Load("Texture/woodSpecular.jpg"));
	cube.setForward(glm::vec3(1));
	cube.setBackward(glm::vec3(1, 0, 0));
	cube.setLeft(glm::vec3(0, 1, 0));
	cube.setRight(glm::vec3(0, 0, 1));
	cube.setTop(glm::vec3(1, 1, 0));
	cube.setBottom(glm::vec3(1, 0, 1));

	float ka = 0.1f, kd = 0.5f, ks = 0.8f, shininess = 32;
	glm::vec3 lightDir(-1), lightColor(1);

	GLuint depthMapFBO;
	glGenFramebuffers(1, &depthMapFBO);
	const GLuint SHADOW_WIDTH = 4096, SHADOW_HEIGHT = 4096;
	GLuint depthMap;
	glGenTextures(1, &depthMap);
	glBindTexture(GL_TEXTURE_2D, depthMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	GLfloat borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	while (!glfwWindowShouldClose(window)) {
		GLfloat currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		glfwPollEvents();
		processInput(window);
		glfwSetInputMode(window, GLFW_CURSOR, CursorMode);

		//model1——地面的model矩阵，model2——正方体的model矩阵
		glm::mat4 model1, model2;
		float time = glfwGetTime();
		model1 = glm::translate(glm::mat4(1), glm::vec3(0, -7, 0));
		model1 = glm::rotate(model1, glm::radians(45.0f), glm::vec3(1, 0, 0));
		model2 = glm::translate(glm::mat4(1), glm::vec3(0, 0, -3.5f));
		model2 = glm::rotate(model2, time, glm::vec3(1));

		GLfloat near_plane = 0.1f, far_plane = 20.0f;
		glm::mat4 lightProjection = glm::ortho(-20.0f, 20.0f, -20.0f, 20.0f, near_plane, far_plane);
		glm::vec3 tmpPos = glm::normalize(-lightDir);
		tmpPos *= 3;
		tmpPos += glm::vec3(0, 0, -3.5f);
		//std::cout << tmpPos.x << ' ' << tmpPos.y << ' ' << tmpPos.z << std::endl;
		glm::mat4 lightView = glm::lookAt(tmpPos, glm::vec3(0, 0, -3.5f), glm::vec3(0, 1, 0));
		glm::mat4 lightSpaceMatrix = lightProjection * lightView;
		glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
		glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
		glClear(GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);
		glCullFace(GL_FRONT);
		simpleDepthShader.use();
		simpleDepthShader.setMat4("lightSpaceMatrix", glm::value_ptr(lightSpaceMatrix));
		simpleDepthShader.setMat4("model", glm::value_ptr(model1));
		ground.Draw(simpleDepthShader);
		simpleDepthShader.setMat4("model", glm::value_ptr(model2));
		cube.Draw(simpleDepthShader);

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glViewport((SRC_WIDTH - MIN(SRC_WIDTH, SRC_HEIGHT)) / 2, (SRC_HEIGHT - MIN(SRC_WIDTH, SRC_HEIGHT)) / 2, MIN(SRC_WIDTH, SRC_HEIGHT), MIN(SRC_WIDTH, SRC_HEIGHT));
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		//glEnable(GL_CULL_FACE);
		glEnable(GL_DEPTH_TEST);
		glCullFace(GL_BACK);

		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), 1.0f, 0.01f, 100.0f);
		glm::vec4 view = camera.GetQuat();
		groundShader.setMatrix(projection, view, camera.Position, model1);
		groundShader.setLight(ka, kd, ks, shininess, lightDir, lightColor);
		groundShader.setMat4("lightSpaceMatrix", glm::value_ptr(lightSpaceMatrix));
		glActiveTexture(GL_TEXTURE10);
		groundShader.setInt("shadow", 10);
		glBindTexture(GL_TEXTURE_2D, depthMap);
		phongShader.setMatrix(projection, view, camera.Position, model2);
		phongShader.setLight(ka, kd, ks, shininess, lightDir, lightColor);
		phongShader.setMat4("lightSpaceMatrix", glm::value_ptr(lightSpaceMatrix));
		glActiveTexture(GL_TEXTURE10);
		groundShader.setInt("shadow", 10);
		glBindTexture(GL_TEXTURE_2D, depthMap);
		cube.Draw(phongShader);
		ground.Draw(groundShader);
		/*depthTestShader.use();
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, depthMap);
		GLuint VAO, VBO, EBO;
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glGenBuffers(1, &EBO);
		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		GLfloat vertices[] = {
			-1, -1,  0,  0,  0,
			 1, -1,  0,  1,  0,
			-1,  1,  0,  0,  1,
			 1,  1,  0,  1,  1
		};
		GLuint indices[] = {
			0, 1, 2, 1, 2, 3
		};

		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), 0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, NULL);
		glBindVertexArray(0);*/

		ImGui_ImplGlfwGL3_NewFrame(CursorMode);
		ImGui::Begin("", NULL, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_AlwaysAutoResize);
			ImGui::Text("Press Esc to exit.");
			ImGui::SliderFloat("Ka", &ka, 0.0f, 1.0f);
			ImGui::SliderFloat("Kd", &kd, 0.0f, 1.0f);
			ImGui::SliderFloat("Ks", &ks, 0.0f, 1.0f);
			ImGui::SliderFloat("Shininess", &shininess, 1, 128);
			ImGui::SliderFloat3("Light direction", &lightDir[0], -1.0f, 1.0f);
			ImGui::ColorEdit3("Light color", &lightColor[0]);
		ImGui::End();
		ImGui::Render();
		glfwSwapBuffers(window);
	}

	glfwDestroyWindow(window);
	glfwTerminate();
}