#include "Camera.h"

#include <iostream>
const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 1.f;
const float SENSITIVITY = 0.1f;
const float ZOOM = 45.0f;

Camera::Camera()
{
	front = Vec3(0.f, 0.f, -1.f);
	movementSpeed = SPEED;
	mouseSensitivity = SENSITIVITY;
	Zoom = ZOOM;
	position = Vec3(0.f, 0.f,  3.f);
	worldUp = Vec3(0.f, 1.f, 0.f);
	yaw = YAW;
	pitch = PITCH;
	UpdateCameraVectors();
}

Camera::Camera(float PosX, float PosY, float PosZ, float UpX, float UpY, float UpZ, float Yaw, float Pitch)
{
	front = Vec3(0.f, 0.f, -1.f);
	movementSpeed = SPEED;
	mouseSensitivity = SENSITIVITY;
	Zoom = ZOOM;
	position = Vec3(PosX, PosY, PosZ);
	worldUp = Vec3(UpX, UpY, UpZ);
	yaw = YAW;
	pitch = PITCH;
	UpdateCameraVectors();
}

Mat4 Camera::GetViewMatrix()
{
	return LookAt(position, position + front, up);
}

void Camera::ProcessKeyboard(CameraMovement direction, float deltaTime)
{
	float velocity = movementSpeed * deltaTime;
	if (direction == CameraMovement::FORWARD)
	{
		position += front * velocity;
	}
	if (direction == CameraMovement::BACKWARD)
	{
		position -= front * velocity;
	}
	if (direction == CameraMovement::LEFT)
	{
		position -= right * velocity;
	}
	if (direction == CameraMovement::RIGHT)
	{
		position += right * velocity;
	}
}

void Camera::ProcessMouseMovement(float xoffset, float yoffset, bool constrainPitch)
{
	xoffset *= mouseSensitivity;
	yoffset *= mouseSensitivity;

	yaw += xoffset;
	pitch += yoffset;

	if(constrainPitch)
	{
		if (pitch > 89.f)
		{
			pitch = 89.f;
		}
		if (pitch < -89.f)
		{
			pitch = -89.f;
		}
	}
	UpdateCameraVectors();
}

void Camera::ProcessMouseScroll(float yoffset)
{
	Zoom -= (float)yoffset;
	if (Zoom < 1.f)
	{
		Zoom = 1.f;
	}
	if (Zoom > 45.f)
	{
		Zoom = 45.f;
	}
}

void Camera::UpdateCameraVectors()
{
	Vec3 Front;
	Front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	Front.y = sin(glm::radians(pitch));
	Front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

	front = normalize(Front);
	right = normalize(cross(front, worldUp));
	up = normalize(cross(right, front));
}
