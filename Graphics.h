#pragma once
#include "Shader.h"
#include <GLFW/glfw3.h>
#include "vector"

const double PI = 3.14159265358979323846;

struct GridConfig {
	int width, height;
	float tileWidth, tileHeight;
	std::vector<std::vector<int>> tiles; // Stores the tile types (0 = empty, 1 = occupied, etc.)
};

class Graphics
{
public:
	static GLFWwindow* InitWindow(const unsigned int Width, const unsigned int Height);
	static Shader DrawTexture(const char* InFileName);
	static Shader DrawGrid(const std::string& filename);
	static void DrawShape(class GameObject& InObject);
	static void DrawShape2(class GameObject& InObject);
	static std::vector<float> createGridVertices(int gridWidth, int gridHeight);

	static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
};