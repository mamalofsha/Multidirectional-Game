#pragma once
#include "Shader.h"
#include <GLFW/glfw3.h>
#include "vector"

const double PI = 3.14159265358979323846;

struct GridConfig {
	int width, height;
	float tileWidth, tileHeight;
	float StartOffsetX, StartOffsetY;
	std::vector<std::vector<int>> tiles; // Stores the tile types (0 = empty, 1 = occupied, etc.)
};

enum class ShaderType { Background, Grid };

struct ShaderEntry {
	Shader shader;
	ShaderType type;
};

class Graphics
{
public:
	static GLFWwindow* InitWindow(const unsigned int Width, const unsigned int Height);
	static Shader DrawTexture(const char* InFileName);
	static Shader DrawGrid(const GridConfig InGridConfig,int WindowsWidth , int WindowsHeight);
	static void DrawShape(class GameObject& InObject);
	static void DrawUIElement(class UIElement& InObject, const char* textureFilePath);
	static void DrawShape2(class GameObject& InObject);
	static std::vector<float> createGridVertices(float gridWidth, float gridHeight, float OffsetX, float OffsetY);

	static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
};