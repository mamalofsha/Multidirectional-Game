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

struct RenderData
{
	unsigned int VAO, VBO, EBO;
	unsigned int TextureID;
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
struct VertexAttribute
{
	int stride;
	std::vector<int> length;
};
class Graphics
{
public:
	static GLFWwindow* InitWindow(const unsigned int Width, const unsigned int Height);
	static RenderData DrawTexture(std::vector<float> vertices, std::vector<unsigned int> indices, VertexAttribute InAttribute, const char* InFileName);

	static Shader DrawGrid(const GridConfig InGridConfig,int WindowsWidth , int WindowsHeight);
	static RenderData DrawUIElement(std::vector<float> position, std::vector<float> size, const char* textureFilePath);
	static Shader InitTextRender(std::map<GLchar, Character>& InMap, float inWindowWidth, float inWindowHeight,unsigned int& VAO, unsigned int& VBO);
	static void InitDrawObject(class GameObject& InObject);
	//static void DrawUIElement(class UIElement& InObject, const char* textureFilePath);
	static std::pair<int, int> GridToWorldPosition(int gridX, int gridY, float tileWidth, float tileHeight, float offsetX, float offsetY, float panX, float panY,float itemscale, float zoom, float windowWidth, float windowHeight);

	static void DrawShape2(class GameObject& InObject);
	static std::vector<float> createGridVertices(float gridWidth, float gridHeight, float OffsetX, float OffsetY);
	static void RenderText(Shader& shader,const unsigned int VAO, const unsigned int VBO,std::string text, float x, float y, float scale, glm::vec3 color, std::map<GLchar, Character>& InMap);
	static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
};