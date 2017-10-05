#include "Shader.h"
#include <GLFW\glfw3.h>
#include <iostream>
#include <imgui\imgui.h>
#include <imgui_impl_glfw_gl3.h>
const GLsizei SRC_WIDTH = 800;
const GLsizei SRC_HEIGHT = 600;
const char* TITLE = "ImGUI";

void error_callback(int error, const char* description) {
	std::cout << "Error " << error << ": " << description << std::endl;
	system("pause");
}

void framebuffer_size_callback(GLFWwindow* window, GLsizei width, GLsizei height) {
	glViewport(0, 0, width, height);
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
		-0.3f, -0.5f,  0.0f,
		-0.3f,  0.5f,  0.0f,
		 0.3f, -0.5f,  0.0f,
		 0.3f,  0.5f,  0.0f
	};

	GLuint indices[] = {
		2, 0, 1, 3
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
	
	Shader program("simple.vert", "simple.frag");
	program.use();
	GLfloat color[3];
	color[0] = color[1] = color[2] = 0.0f;
	program.setFloat("uColor", color[0], color[1], color[2], 1.0f);
	int count = 3;
	bool show_color_picker = false;
	enum DRAW_TYPE{FILL, LINE, DOT};
	DRAW_TYPE type = FILL;
	ImGui_ImplGlfwGL3_Init(window, true);
	while (!glfwWindowShouldClose(window)) {
		glfwWaitEvents();
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

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
			default:
				break;
		}

		ImGui_ImplGlfwGL3_NewFrame();
		if (ImGui::BeginMainMenuBar()) {
			if (ImGui::BeginMenu("Change Primitives")) {
				if (ImGui::MenuItem("Triangle")) count = 3;
				if (ImGui::MenuItem("Rectangle")) count = 4;
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Type")) {
				if (ImGui::MenuItem("Fill")) type = FILL;
				if (ImGui::MenuItem("LINE")) type = LINE;
				if (ImGui::MenuItem("DOT")) type = DOT;
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Color")) {
				ImGui::MenuItem("Show color picker", NULL, &show_color_picker);
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