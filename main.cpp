#include "Shader.h"
#include "rasterization.h"
#include "object\ball.h"
#include <GLFW\glfw3.h>
#include <iostream>
#include <imgui\imgui.h>
#include <imgui_impl_glfw_gl3.h>
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>
//OpenGL默认列主序

const GLsizei SRC_WIDTH = 800;
const GLsizei SRC_HEIGHT = 800;
const char* TITLE = "ImGUI";

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
	glViewport(0, 0, SRC_WIDTH, SRC_HEIGHT);

	GLfloat vertices[] = {
		-0.3f, -0.3f,  0.3f,
		-0.3f,  0.3f,  0.3f,
		 0.3f, -0.3f,  0.3f,
		 0.3f,  0.3f,  0.3f,
		-0.3f, -0.3f, -0.3f,
		-0.3f,  0.3f, -0.3f,
		 0.3f, -0.3f, -0.3f,
		 0.3f,  0.3f, -0.3f
	};

	GLuint indices[] = {
		2, 0, 1, 3, 6, 4, 5, 7, 2, 0, 4, 6, 1, 3, 7, 5
	};

	GLuint VAO, VBO, EBO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(0);
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	
	Shader program("shader/simple.vert", "shader/simple.frag");
	Shader program2("shader/UseTexture.vert", "shader/UseTexture.frag");
	Shader programRotate("shader/Rotate.vert", "shader/simple.frag");
	Shader programTranslate("shader/Translate.vert", "shader/simple.frag");
	Shader programScale("shader/Scale.vert", "shader/simple.frag");
	GLfloat color[3];
	color[0] = color[1] = color[2] = 0.0f;
	//在使用glUniform{1, 2, 3, 4}{f, v, fv}()时必须得先glUseProgram()
	//否则glGetError()会返回GL_INVALID_OPERATION错误，uniform值也无法传入
	program.use();
	program.setFloat("uColor", color[0], color[1], color[2], 1.0f);

	int count = 3;
	bool show_color_picker = false, useTransShader = false;
	enum DRAW_TYPE{FILL, LINE, DOT, LINEDDA, LINEBRESENHAM, CIRCLEBRESENHAM, TRIANGLEEDGEWALKING, TRIANGLEEDGEEQUATIONS, CUBE, SURROUNDING};
	DRAW_TYPE type = FILL;
	int transtype = 0;
	ImGui_ImplGlfwGL3_Init(window, true);
	vertices[0] -= 0.1f;

	type = SURROUNDING;
	BALL earth(20, 20, 0.376f, "earth.jpg");
	BALL moon(20, 20, 0.1f, "moon.jpg");

	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glBindVertexArray(VAO);
		program.use();

		glEnable(GL_DEPTH_TEST);

		glm::mat4 trans(1.0f);
		program.setMat4("trans", glm::value_ptr(trans));

		switch (type)
		{
			case FILL:
				glDrawArrays(GL_TRIANGLE_STRIP, 0, count);
				break;
			case LINE:
				glDrawElements(GL_LINE_LOOP, count, GL_UNSIGNED_INT, 0);
				break;
			case DOT:
				glDrawArrays(GL_POINTS, 0, count);
				break;
			case CUBE:
				if (!useTransShader) {
					switch (transtype)
					{
					case 1:
						trans = glm::translate(trans, glm::vec3(0.0f, cos(glfwGetTime()), 0.0f));
						break;
					case 2:
						trans = glm::rotate(trans, (float)glfwGetTime(), glm::vec3(0.0f, 1.0f, 0.0f));
						break;
					case 3:
						trans = glm::scale(trans, glm::vec3(cos(glfwGetTime()) / 3 + 0.5f, cos(glfwGetTime()) / 3 + 0.5f, cos(glfwGetTime()) / 3 + 0.5f));
						break;
					default:
						break;
					}
					trans = glm::rotate(trans, 37.0f, glm::vec3(0.5f, 0.4f, 0.7f));
					program.setMat4("trans", glm::value_ptr(trans));
				}
				else {
					switch (transtype)
					{
					case 1:
						programTranslate.use();
						programTranslate.setFloat("uColor", color[0], color[1], color[2], 1.0f);
						programTranslate.setFloat("v", (float)cos(glfwGetTime()), 0.0f, 0.0f);
						break;
					case 2:
						programRotate.use();
						programRotate.setFloat("uColor", color[0], color[1], color[2], 1.0f);
						programRotate.setFloat("angle", (float)glfwGetTime());
						programRotate.setFloat("axis", 1.0f, 1.0f, 1.0f);
						break;
					case 3:
						programScale.use();
						programScale.setFloat("uColor", color[0], color[1], color[2], 1.0f);
						programScale.setFloat("v", cos(glfwGetTime()) / 3 + 0.5f, cos(glfwGetTime()) / 3 + 0.5f, cos(glfwGetTime()) / 3 + 0.5f);
						break;
					default:
						break;
					}
				}

				glDrawElements(GL_LINE_LOOP, 4, GL_UNSIGNED_INT, 0);
				glDrawElements(GL_LINE_LOOP, 4, GL_UNSIGNED_INT, (void*)(4 * sizeof(GLuint)));
				glDrawElements(GL_LINE_LOOP, 4, GL_UNSIGNED_INT, (void*)(8 * sizeof(GLuint)));
				glDrawElements(GL_LINE_LOOP, 4, GL_UNSIGNED_INT, (void*)(12 * sizeof(GLuint)));
				program.setFloat("uColor", 1.0f, 0.0f, 0.0f, 1.0f);
				programRotate.setFloat("uColor", 1.0f, 0.0f, 0.0f, 1.0f);
				programScale.setFloat("uColor", 1.0f, 0.0f, 0.0f, 1.0f);
				programTranslate.setFloat("uColor", 1.0f, 0.0f, 0.0f, 1.0f);
				glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
				program.setFloat("uColor", 0.0f, 1.0f, 0.0f, 1.0f);
				programRotate.setFloat("uColor", 0.0f, 1.0f, 0.0f, 1.0f);
				programScale.setFloat("uColor", 0.0f, 1.0f, 0.0f, 1.0f);
				programTranslate.setFloat("uColor", 0.0f, 1.0f, 0.0f, 1.0f);
				glDrawArrays(GL_TRIANGLE_STRIP, 4, 4);
				program.setFloat("uColor", color[0], color[1], color[2], 1.0f);

				break;
			case SURROUNDING:
				program2.use();
				trans = glm::rotate(trans, glm::radians(-23.26f), glm::vec3(0.0f, 0.0f, 1.0f));
				trans = glm::rotate(trans, (float)glfwGetTime(), glm::vec3(0.0f, 1.0f, 0.0f));
				trans = glm::rotate(trans, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
				program2.setMat4("trans", glm::value_ptr(trans));
				earth.Draw();
				program2.use();
				trans = glm::rotate(trans, (float)glfwGetTime() / 3.0f, glm::vec3(0.0f, 0.0f, 1.0f));
				trans = glm::translate(trans, glm::vec3(0.0f, 0.7f, 0.0f));
				trans = glm::rotate(trans, (float)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));
				program2.setMat4("trans", glm::value_ptr(trans));
				moon.Draw();

				break;
			case LINEDDA:
				rasterization::LineDDA(-9, 8, 8, 6, 10);
				rasterization::LineDDA(-90, 60, 80, 40, 100);
				rasterization::LineDDA(-900, 400, 800, 200, 1000);
				rasterization::LineDDA(-9000, 2000, 8000, 0, 10000);
				rasterization::LineDDA(-90000, -90000, -90000, 10000, 100000);
				rasterization::LineDDA(-80000, -40000, 90000, -40000, 100000);
				break;
			case LINEBRESENHAM:
				rasterization::LineBresenham(-9, 8, 8, 6, 10);
				rasterization::LineBresenham(-90, 60, 80, 40, 100);
				rasterization::LineBresenham(-900, 400, 800, 200, 1000);
				rasterization::LineBresenham(-9000, 2000, 8000, 0, 10000);
				rasterization::LineBresenham(-90000, -90000, -90000, 10000, 100000);
				rasterization::LineBresenham(-80000, -40000, 90000, -40000, 100000);
				break;
			case CIRCLEBRESENHAM:
				rasterization::CircleBresenham(-15, 15, 15, 30);
				rasterization::CircleBresenham(150, 150, 150, 300);
				rasterization::CircleBresenham(-1500, -1500, 1500, 3000);
				rasterization::CircleBresenham(15000, -15000, 15000, 30000);
				break;
			case TRIANGLEEDGEWALKING:
				rasterization::EdgeWalking(-30, 8, -4, 16, -20, 26, 33);
				rasterization::EdgeWalking(40, 40, 150, 40, 40, 170, 190);
				rasterization::EdgeWalking(-80, -80, -300, -80, -80, -340, 350);
				rasterization::EdgeWalking(400, -1000, 1000, -300, 600, -1800, 2000);
				break;
			case TRIANGLEEDGEEQUATIONS:
				rasterization::EdgeEquations(-30, 8, -4, 16, -20, 26, 33);
				rasterization::EdgeEquations(40, 40, 150, 40, 40, 170, 190);
				rasterization::EdgeEquations(-80, -80, -300, -80, -80, -340, 350);
				rasterization::EdgeEquations(400, -1000, 1000, -300, 600, -1800, 2000);
				break;
			default:
				break;
		}

		ImGui_ImplGlfwGL3_NewFrame();
		if (ImGui::BeginMainMenuBar()) {
			if (ImGui::BeginMenu("Homework3&4")) {
				if (ImGui::BeginMenu("Change Primitives")) {
					if (ImGui::MenuItem("Triangle")) {
						type = FILL;
						count = 3;
					}
					if (ImGui::MenuItem("Rectangle")) {
						type = FILL;
						count = 4;
					}
					ImGui::EndMenu();
				}
				if (ImGui::BeginMenu("Type")) {
					if (ImGui::MenuItem("Fill")) type = FILL;
					if (ImGui::MenuItem("line")) type = LINE;
					if (ImGui::MenuItem("Dot")) type = DOT;
					if (ImGui::MenuItem("LineDDA")) type = LINEDDA;
					if (ImGui::MenuItem("LineBresenham")) type = LINEBRESENHAM;
					if (ImGui::MenuItem("CircleBresenham")) type = CIRCLEBRESENHAM;
					if (ImGui::MenuItem("TriangleEdgeWalking")) type = TRIANGLEEDGEWALKING;
					if (ImGui::MenuItem("TriangleEdgeEquations")) type = TRIANGLEEDGEEQUATIONS;
					ImGui::EndMenu();
				}
				if (ImGui::BeginMenu("Color")) {
					ImGui::MenuItem("Show color picker", NULL, &show_color_picker);
					ImGui::EndMenu();
				}
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Homework5")) {
				if (ImGui::MenuItem("Cube")) {
					type = CUBE;
					transtype = 0;
				}
				if (ImGui::MenuItem("Translate Cube")) {
					type = CUBE;
					transtype = 1;
				}
				if (ImGui::MenuItem("Rotate Cube")) {
					type = CUBE;
					transtype = 2;
				}
				if (ImGui::MenuItem("Scale Cube")) {
					type = CUBE;
					transtype = 3;
				}
				if (ImGui::MenuItem("Surrounding")) type = SURROUNDING;
				ImGui::MenuItem("Ues Transformatino shader", NULL, &useTransShader);
				ImGui::EndMenu();
			}
			ImGui::EndMainMenuBar();
		}
		if (show_color_picker) {
			ImGui::Begin("Colro Picker", &show_color_picker, ImGuiWindowFlags_AlwaysAutoResize);
			ImGui::ColorEdit3("", color);
			program.setFloat("uColor", color[0], color[1], color[2], 1.0f);
			ImGui::End();
		}
		ImGui::Render();
		glfwSwapBuffers(window);
	}

	glfwDestroyWindow(window);
	glfwTerminate();
}