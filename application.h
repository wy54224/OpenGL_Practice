#pragma once
#include "Shader\textureShader.h"
#include "Shader\SkyShader.h"
#include "Object\model.h"
#include "Object\rectangle.h"
#include "Object\ball.h"
#include "camera.h"
#include <glad\glad.h>
#include <GLFW\glfw3.h>
#include <imgui\imgui.h>
#include <imgui_impl_glfw_gl3.h>
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm\gtc\quaternion.hpp>
#include <glm\gtx\quaternion.hpp>
#ifndef MIN
#define MIN(x, y)((x) > (y) ? (y) : (x))
#endif // !MIN
#ifndef MAX
#define MAX(x, y)((x) > (y) ? (x) : (y))
#endif // !MAX