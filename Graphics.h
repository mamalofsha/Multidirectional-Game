#pragma once
#include "Shader.h"
#include <GLFW/glfw3.h>

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;
const double PI = 3.14159265358979323846;

class Graphics
{
public:
	static GLFWwindow* InitWindow();
	static Shader DrawTexture(const char* InFileName);
	static void DrawShape(class GameObject& InObject);
	static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
};