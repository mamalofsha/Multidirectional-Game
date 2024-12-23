#pragma once
#include "Shader.h"
#include <GLFW/glfw3.h>
#include "vector"
#include <map>

/// Holds all state information relevant to a character as loaded using FreeType
struct Character {
	unsigned int TextureID; // ID handle of the glyph texture
	glm::ivec2   Size;      // Size of glyph
	glm::ivec2   Bearing;   // Offset from baseline to left/top of glyph
	unsigned int Advance;   // Horizontal offset to advance to next glyph
};
/// Holds all state information relevant to grid data
struct GridConfig {
	int Width, Height;					 // Stores rows and columns count e.g 5x5
	float TileWidth, TileHeight;		 // Stores tile sizes (0.2 x 0.1 (diamond shaped - for isometric illusion)) 
	float StartOffsetX, StartOffsetY;    // Stores the grid origin offset
	std::vector<std::vector<int>> Tiles; // Stores the tile types (0 = empty, (!= 0) = occupied)
};
/// Holds all state information relevant to render an object
struct RenderData
{
	unsigned int VAO, VBO, EBO;
	unsigned int TextureID;
};
// for being able to do this 	glVertexAttribPointer(i, InAttribute.length[i], GL_FLOAT, GL_FALSE, InAttribute.stride * sizeof(float), (void*)(Sum * sizeof(float)));
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
	static std::pair<int, int> GridToWorldPosition(int gridX, int gridY, float tileWidth, float tileHeight, float offsetX, float offsetY, float panX, float panY,float itemscale, float zoom, float windowWidth, float windowHeight , float levelWidth, float levelHeight);

	static std::vector<float> createGridVertices(float gridWidth, float gridHeight, float OffsetX, float OffsetY);
	static void RenderText(Shader& shader,const unsigned int VAO, const unsigned int VBO,std::string text, float x, float y, float scale, glm::vec3 color, std::map<GLchar, Character>& InMap);
	static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
};