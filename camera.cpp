#include "camera.h"
#include <glad\glad.h>
#include <glm\gtc\matrix_transform.hpp>
#include <iostream>

inline void Camera::UpdateCameraVectors()
{
	float _Yaw = -glm::radians(Yaw), _Pitch = -glm::radians(Pitch);
	q = multiQuat(glm::vec4(sin(_Pitch), 0, 0, cos(_Pitch)), glm::vec4(0, sin(_Yaw), 0, cos(_Yaw)));
}

/*glm::vec3 Camera::quatRotate(const float & theta, const glm::vec3 & v, const glm::vec3 & p)
{
	glm::vec3 v1 = glm::normalize(v);
	float halfTheta = theta / 2;
	float sinHalfTheta = sin(halfTheta), w;
	glm::vec3 q;
	w = cos(halfTheta);
	q.x = sinHalfTheta * v.x;
	q.y = sinHalfTheta * v.y;
	q.z = sinHalfTheta * v.z;
	return glm::vec3(multiQuat(multiQuat(glm::vec4(q, w), glm::vec4(p, 0)), glm::vec4(-q, w)));
}*/

inline glm::vec4 Camera::multiQuat(const glm::vec4 & v1, const glm::vec4 & v2)
{
	glm::vec4 v;
	v.w = v1.w * v2.w - v1.x * v2.x - v1.y * v2.y - v1.z * v2.z;
	v.x = v1.w * v2.x + v2.w * v1.x + v1.y * v2.z - v1.z * v2.y;
	v.y = v1.w * v2.y + v2.w * v1.y + v1.z * v2.x - v1.x * v2.z;
	v.z = v1.w * v2.z + v2.w * v1.z + v1.x * v2.y - v1.y * v2.x;
	return v;
}

Camera::Camera(glm::vec3 position, float yaw, float pitch)
{
	MovementSpeed = SPEED;
	MouseSensitivity = SENSITIVITY;
	Zoom = ZOOM;
	Position = position;
	Yaw = yaw;
	Pitch = pitch;
	UpdateCameraVectors();
}

glm::vec4 Camera::GetQuat(){
	return q;
}

void Camera::CameraPositionMove(Camera_Movement direction, float deltaTime)
{
	float velocity = MovementSpeed * deltaTime;
	glm::vec4 _q = glm::vec4(-q.x, -q.y, -q.z, q.w);
	if (direction == FORWARD) {
		glm::vec3 Front = glm::vec3(multiQuat(multiQuat(_q, glm::vec4(0, 0, -1, 0)), q));
		Position += velocity * Front;
	}
	if (direction == BACKWARD) {
		glm::vec3 Front = glm::vec3(multiQuat(multiQuat(_q, glm::vec4(0, 0, -1, 0)), q));
		Position -= velocity * Front;
	}
	if (direction == LEFT) {
		glm::vec3 Right = glm::vec3(multiQuat(multiQuat(_q, glm::vec4(1, 0, 0, 0)), q));
		Position -= velocity * Right;
	}
	if (direction == RIGHT) {
		glm::vec3 Right = glm::vec3(multiQuat(multiQuat(_q, glm::vec4(1, 0, 0, 0)), q));
		Position += velocity * Right;
	}
}

void Camera::CameraDirectionChange(float xOffset, float yOffset, const bool constrainPitch)
{
	xOffset *= MouseSensitivity;
	yOffset *= MouseSensitivity;
	Yaw -= xOffset;
	Pitch += yOffset;
	if (constrainPitch) {
		if (Pitch > 89.0f)
			Pitch = 89.0f;
		if (Pitch < -89.0f)
			Pitch = -89.0f;
	}
	UpdateCameraVectors();
	glm::vec4 _q = glm::vec4(-q.x, -q.y, -q.z, q.w);
	glm::vec3 Front = glm::vec3(multiQuat(multiQuat(_q, glm::vec4(0, 0, -1, 0)), q));
	//std::cout << Front.x << ' ' << Front.y << ' ' << Front.z << std::endl;
	//std::cout << Yaw << ' ' << Pitch << std::endl;
}

void Camera::CameraZoomChange(float yOffset)
{
	Zoom -= yOffset;
	if (Zoom < 1.0f)
		Zoom = 1.0f;
	if (Zoom > 45.0f)
		Zoom = 45.0f;
}

glm::vec3 & Camera::direction()
{
	glm::vec4 _q = glm::vec4(-q.x, -q.y, -q.z, q.w);
	return glm::vec3(multiQuat(multiQuat(_q, glm::vec4(0, 0, -1, 0)), q));
}
