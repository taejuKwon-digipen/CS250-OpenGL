#pragma once
#include <GLFW/glfw3.h>
#include <vector>
#include "math.hpp"

class Mesh;
class Shader;
class PerlinNoise;

class Window
{
public:
	static void windowInit(int width, int height);
	static void fbsize_callback([[maybe_unused]] GLFWwindow* window, int width, int height);
	static void key_callback(GLFWwindow* pwin, int key, int scancode, int action, int mod);
	static void mousebutton_callback(GLFWwindow* pwin, int button, int action, int mod);
	static void mousescroll_callback(GLFWwindow* pwin, double xoffset, double yoffset);
	static void mousepos_callback(GLFWwindow* pwin, double xposIn, double yposIn);
	static void error_callback(int error, char const* description);
	static void processInput(GLFWwindow* window);
	
	static double update_time(double fpsCalcInt = 1.0);
	
	static GLFWwindow* ptr_window;
	inline static GLdouble fps = 30;
	inline static float deltaTime = 0.f;
	inline static float lastFrame = 0.f;
	inline static int windowWidth, windowHeight;
	inline static bool firstMouse = true;
	inline static float lastX = windowWidth / 2.f;
	inline static float lastY = windowHeight / 2.f;
private:
};