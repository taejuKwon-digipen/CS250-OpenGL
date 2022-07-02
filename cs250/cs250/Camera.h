#pragma once
#include "math.hpp"

enum class CameraMovement
{
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT,
};

class Camera
{
public:
	inline static Vec3 position;
	inline static Vec3 front;
	inline static Vec3 up;
	inline static Vec3 right;
	inline static Vec3 worldUp;

	inline static float yaw;
	inline static float pitch;

	inline static float movementSpeed;
	inline static float mouseSensitivity;
	inline static float Zoom;

	Camera();
	Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch);

	static Mat4 GetViewMatrix();
	static void ProcessKeyboard(CameraMovement direction, float deltaTime);
	static void ProcessMouseMovement(float xoffset, float yoffset, bool constrainPitch);
	static void ProcessMouseScroll(float yoffset);
	static void UpdateCameraVectors();
};