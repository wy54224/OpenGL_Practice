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
//OpenGL默认列主序

const GLsizei SRC_WIDTH = 800;
const GLsizei SRC_HEIGHT = 800;
const char* TITLE = "ImGUI";
Camera camera(glm::vec3(0));
GLfloat lastFrame = 0.0f, deltaTime = 0.0f;
int CursorMode = GLFW_CURSOR_NORMAL;
int cameraMode = 1;

void processInput(GLFWwindow* window) {
	if (cameraMode == 1) {
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
			camera.CameraPositionMove(FORWARD, deltaTime);
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
			camera.CameraPositionMove(BACKWARD, deltaTime);
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
			camera.CameraPositionMove(LEFT, deltaTime);
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
			camera.CameraPositionMove(RIGHT, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS)
		cameraMode = 0;
	if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
		cameraMode = 1;
	if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS)
		//glfwSetWindowShouldClose(window, true);
		CursorMode = GLFW_CURSOR_NORMAL;
	if (glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS)
		CursorMode = GLFW_CURSOR_DISABLED;
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
	if(cameraMode == 1)
		camera.CameraDirectionChange(xPos - lastX, lastY - yPos);
	lastX = xPos;
	lastY = yPos;
}

void scroll_callback(GLFWwindow *window, double xOffset, double yOffset) {
	if(cameraMode == 1)
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
	
	ColorShader shader("Shader/color.vert", "Shader/color.frag");
	Object::ColorCube cube(1);
	cube.setForward(glm::vec3(1));
	cube.setBackward(glm::vec3(1, 0, 0));
	cube.setLeft(glm::vec3(0, 1, 0));
	cube.setRight(glm::vec3(0, 0, 1));
	cube.setTop(glm::vec3(1, 1, 0));
	cube.setBottom(glm::vec3(1, 0, 1));
	//初始设置为透视投影(true)
	bool whichProjection = true;
	float Far, Near, Left, Right, Top, Bottom;
	Far = Right = Top = 5;
	Near = Left = Bottom = -5;
	float PFar = 100.0f, PNear = 0.01, aspect = 1;

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
		glm::mat4 projection;
		if (whichProjection) {
			//透视投影
			projection = glm::perspective(glm::radians(camera.Zoom), aspect, PNear, PFar);
			float tanHF = tan(glm::radians(camera.Zoom) / 2);
			projection = glm::mat4(0);
			projection[0][0] = 1 / (aspect * tanHF);
			projection[1][1] = 1 / tanHF;
			projection[2][2] = -(PFar + PNear) / (PFar - PNear);
			projection[3][2] = -(2 * PFar * PNear) / (PFar - PNear);
			projection[2][3] = -1;
		}
		else {
			//正交投影
			//projection = glm::ortho(Left, Right, Bottom, Top, Near, Far);
			projection = glm::mat4(1);
			projection[0][0] = 2 / (Right - Left);
			projection[3][0] = -(Right + Left) / (Right - Left);
			projection[1][1] = 2 / (Top - Bottom);
			projection[3][1] = -(Top + Bottom) / (Top - Bottom);
			projection[2][2] = -2 / (Far - Near);
			projection[3][2] = -(Far + Near) / (Far - Near);

		}
		glm::vec4 view(0, 0, 0, 1);
		glm::mat4 model;
		if (cameraMode != 2) {
			view = camera.GetQuat();
			model = glm::translate(glm::mat4(1), glm::vec3(0.5f, 0.5f, -1.5f));
			shader.setMatrix(projection, view, camera.Position, model);
			//std::cout << camera.Position.x << ' ' << camera.Position.y << ' ' << camera.Position.z << std::endl;
		}
		else {
			model = glm::mat4(1);
			float theta = glfwGetTime();
			view.w = cos(theta / 2 + glm::radians(45.0));
			view.x = 0;
			view.y = -sin(theta / 2 + glm::radians(45.0));
			view.z = 0;
			shader.setMatrix(projection, view, glm::vec3(4 * cos(theta), 0, -4 * sin(theta)), model);
		}

		cube.Draw(shader);

		ImGui_ImplGlfwGL3_NewFrame(CursorMode);
		if (ImGui::BeginMainMenuBar()) {
			if (ImGui::BeginMenu("Projection")) {
				if (ImGui::MenuItem("Orthogonal Projection")) whichProjection = false;
				if (ImGui::MenuItem("Perspective Projection")) whichProjection = true;
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Mode")) {
				if (ImGui::MenuItem("Camera Free")) cameraMode = 1;
				if (ImGui::MenuItem("Camera Static")) cameraMode = 0;
				if (ImGui::MenuItem("Camera Round")) cameraMode = 2;
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("CursorMode")) {
				if (ImGui::MenuItem("Normal"))CursorMode = GLFW_CURSOR_NORMAL;
				if (ImGui::MenuItem("Disappear"))CursorMode = GLFW_CURSOR_DISABLED;
				ImGui::EndMenu();
			}
			ImGui::EndMainMenuBar();
		}
		if (cameraMode != 2) {
			ImGui::Begin("", NULL, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_AlwaysAutoResize);
			if (cameraMode)
				ImGui::Text("Press P to forbid camera Move.");
			else
				ImGui::Text("Press R to allow camera Move.");
			if (CursorMode == GLFW_CURSOR_DISABLED)
				ImGui::Text("Press J to quit cursor disable mode.");
			else
				ImGui::Text("Press H to hide cursor.");
			if (whichProjection) {
				ImGui::InputFloat("Near", &PNear);
				ImGui::InputFloat("Far", &PFar);
				ImGui::InputFloat("Aspect", &aspect);
			}
			else {
				ImGui::InputFloat("Near", &Near);
				ImGui::InputFloat("Far", &Far);
				ImGui::InputFloat("Left", &Left);
				ImGui::InputFloat("Right", &Right);
				ImGui::InputFloat("Buttom", &Bottom);
				ImGui::InputFloat("Top", &Top);
			}
			ImGui::End();
		}
		ImGui::Render();
		glfwSwapBuffers(window);
	}

	glfwDestroyWindow(window);
	glfwTerminate();
}