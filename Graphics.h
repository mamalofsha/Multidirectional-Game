#pragma once
#include "Shader.h"
#include <GLFW/glfw3.h>
#include "vector"
#include <map>

const double PI = 3.14159265358979323846;
/// Holds all state information relevant to a character as loaded using FreeType
struct Character {
	unsigned int TextureID; // ID handle of the glyph texture
	glm::ivec2   Size;      // Size of glyph
	glm::ivec2   Bearing;   // Offset from baseline to left/top of glyph
	unsigned int Advance;   // Horizontal offset to advance to next glyph
};

struct GridConfig {
	int width, height;
	float tileWidth, tileHeight;
	float StartOffsetX, StartOffsetY;
	std::vector<std::vector<int>> tiles; // Stores the tile types (0 = empty, 1 = occupied, etc.)
};

struct TextureData {
	Shader OutShader;
	std::vector<float> texturedVertices;
	std::vector<unsigned int> indices;
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
	static Shader GenerateTextureData(const std::vector<float>& IntexturedVertices,const std::vector<float>& IngridVertices);
	static Shader DrawGrid(const GridConfig InGridConfig,int WindowsWidth , int WindowsHeight);
	static Shader InitTextRender(std::map<GLchar, Character>& InMap, float inWindowWidth, float inWindowHeight);
	static void DrawShape(class GameObject& InObject);
	static void DrawUIElement(class UIElement& InObject, const char* textureFilePath);
	static void DrawShape2(class GameObject& InObject);
	static std::vector<float> createGridVertices(float gridWidth, float gridHeight, float OffsetX, float OffsetY);
	static void RenderText(Shader& shader, std::string text, float x, float y, float scale, glm::vec3 color, std::map<GLchar, Character>& InMap);
	static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
};