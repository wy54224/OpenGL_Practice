#pragma once
#ifndef CAMERA_H
#define CAMERA_H
#include <glm\glm.hpp>

enum Camera_Movement
{
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT
};

const float SPEED = 2.5f;
const float SENSITIVITY = 0.05f;
const float ZOOM = 45.0f;

class Camera {
	inline void UpdateCameraVectors();
	glm::vec4 q;
	//glm::vec3 quatRotate(const float &theta, const glm::vec3 &v, const glm::vec3 &p);
	inline glm::vec4 multiQuat(const glm::vec4 &v1, const glm::vec4 &v2);
public:
	glm::vec3 Position;
	float Yaw, Pitch, MovementSpeed, MouseSensitivity, Zoom;
	Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), float yaw = 0, float pitch = 0);

	glm::vec4 GetQuat();
	void CameraPositionMove(Camera_Movement direction, float deltaTime);
	void CameraDirectionChange(float xOffset, float yOffset, const bool constrainPitch = true);
	void CameraZoomChange(float yOffset);
	glm::vec3 &direction();
};

#endif // !CAMERA_H