#include "colorShader.h"
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
//OpenGLÄ¬ÈÏÁÐÖ÷Ðò

const GLsizei SRC_WIDTH = 800;
const GLsizei SRC_HEIGHT = 800;
const char* TITLE = "ImGUI";
Camera camera(glm::vec3(0));
GLfloat lastFrame = 0.0f, deltaTime = 0.0f;
int CursorMode = GLFW_CURSOR_NORMAL;
bool isPhongOrGouraud = true;

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
	if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS)
		isPhongOrGouraud = true;
	if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS)
		isPhongOrGouraud = false;

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
	ColorShader gouraudShader("Shader/gcolor.vert", "Shader/gcolor.frag");
	Object::ColorCube cube(1);
	cube.setForward(glm::vec3(1));
	cube.setBackward(glm::vec3(1, 0, 0));
	cube.setLeft(glm::vec3(0, 1, 0));
	cube.setRight(glm::vec3(0, 0, 1));
	cube.setTop(glm::vec3(1, 1, 0));
	cube.setBottom(glm::vec3(1, 0, 1));

	float ka = 0.1f, kd = 0.5f, ks = 0.8f, shininess = 32;
	glm::vec3 lightDir(-1), lightColor(1);

	while (!glfwWindowShouldClose(window)) {
		GLfloat currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		glfwPollEvents();
		processInput(window);
		glfwSetInputMode(window, GLFW_CURSOR, CursorMode);
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		//glEnable(GL_CULL_FACE);
		glEnable(GL_DEPTH_TEST);
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), 1.0f, 0.01f, 100.0f);
		glm::vec4 view = camera.GetQuat();
		glm::mat4 model;
		float time = glfwGetTime();
		model = glm::translate(glm::mat4(1), glm::vec3(0, 0, -3.5f));
		model = glm::rotate(model, time, glm::vec3(1));
		phongShader.setMatrix(projection, view, camera.Position, model);
		phongShader.setLight(ka, kd, ks, shininess, lightDir, lightColor);
		gouraudShader.setMatrix(projection, view, camera.Position, model);
		gouraudShader.setLight(ka, kd, ks, shininess, lightDir, lightColor);

		if (isPhongOrGouraud)
			cube.Draw(phongShader);
		else
			cube.Draw(gouraudShader);

		ImGui_ImplGlfwGL3_NewFrame(CursorMode);
		ImGui::Begin("", NULL, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_AlwaysAutoResize);
			ImGui::Text("Press Esc to exit.");
			ImGui::Text("Press P to use phong shading.");
			ImGui::Text("Press G to use gouraud shading.");
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