#include "Mesh.h"
#include "GLShader.h"
#include "Window.h"
#include "IMGUI.h"
#include <iostream>
#include "ValueNoise.h"
#include "Camera.h"

GLFWwindow* Window::ptr_window;

void Window::windowInit(int width, int height)
{
	if (!glfwInit())
	{
		std::cout << "GLFW init has failed." << std::endl;
		std::exit(EXIT_FAILURE);
	}
	glfwSetErrorCallback(error_callback);

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RED_BITS, 8);
	glfwWindowHint(GLFW_GREEN_BITS, 8);
	glfwWindowHint(GLFW_BLUE_BITS, 8);
	glfwWindowHint(GLFW_ALPHA_BITS, 8);
	glfwWindowHint(GLFW_DOUBLEBUFFER, GLFW_TRUE);
	
	ptr_window = glfwCreateWindow(width, height, "KaN-Window", NULL, NULL);
	windowWidth = width;
	windowHeight = height;
	
	if (ptr_window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		std::exit(EXIT_FAILURE);
	}

	glfwMakeContextCurrent(ptr_window);

	glfwSetFramebufferSizeCallback(ptr_window, fbsize_callback); // error
	glfwSetKeyCallback(ptr_window, key_callback);
	glfwSetMouseButtonCallback(ptr_window, mousebutton_callback);
	glfwSetCursorPosCallback(ptr_window, mousepos_callback);
	glfwSetScrollCallback(ptr_window, mousescroll_callback);
	processInput(ptr_window);

	glfwSetInputMode(ptr_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

	glViewport(0, 0, windowWidth, windowHeight);
}

void Window::fbsize_callback([[maybe_unused]] GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void Window::key_callback(GLFWwindow* pwin, int key, int scancode, int action, int mod)
{
	if (GLFW_KEY_ESCAPE == key && GLFW_PRESS == action) {
		glfwSetWindowShouldClose(pwin, GLFW_TRUE);
	}
}

void Window::mousebutton_callback(GLFWwindow* pwin, int button, int action, int mod)
{
	switch (button)
	{
	case GLFW_MOUSE_BUTTON_LEFT:
		break;
	case GLFW_MOUSE_BUTTON_RIGHT:
		break;
	}
	switch (action)
	{
	case GLFW_PRESS:
		break;
	case GLFW_RELEASE:
		break;
	}
}

void Window::mousepos_callback(GLFWwindow* pwin, double xposIn, double yposIn)
{
	float xpos = static_cast<float>(xposIn);
	float ypos = static_cast<float>(yposIn);

	if (glfwGetKey(pwin, GLFW_KEY_SPACE) == GLFW_PRESS) {
		if (firstMouse)
		{
			lastX = xpos;
			lastY = ypos;
			firstMouse = false;
		}

		float xoffset = xpos - lastX;
		float yoffset = lastY - ypos;

		lastX = xpos;
		lastY = ypos;

		Camera::ProcessMouseMovement(xoffset, yoffset, firstMouse);
	}
}

void Window::mousescroll_callback(GLFWwindow* pwin, double xoffset, double yoffset)
{
	Camera::ProcessMouseScroll(static_cast<float>(yoffset));
}

void Window::error_callback(int error, char const* description)
{
	std::cerr << "GLFW error: " << description << std::endl;
}

void Window::processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		Camera::ProcessKeyboard(CameraMovement::FORWARD, deltaTime);
		
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		Camera::ProcessKeyboard(CameraMovement::BACKWARD, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		Camera::ProcessKeyboard(CameraMovement::LEFT, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		Camera::ProcessKeyboard(CameraMovement::RIGHT, deltaTime);
	}
}

double Window::update_time(double fps_calc_interval)
{
	static double prev_time = glfwGetTime();
	double curr_time = glfwGetTime();
	double delta_time = curr_time - prev_time;
	prev_time = curr_time;

	static double count = 0.0; // number of game loop iterations
	static double start_time = glfwGetTime();
	double elapsed_time = curr_time - start_time;

	++count;

	fps_calc_interval = (fps_calc_interval < 0.0) ? 0.0 : fps_calc_interval;
	fps_calc_interval = (fps_calc_interval > 10.0) ? 10.0 : fps_calc_interval;
	if (elapsed_time > fps_calc_interval) {
		fps = count / elapsed_time;
		start_time = curr_time;
		count = 0.0;
	}

	return delta_time;
}
