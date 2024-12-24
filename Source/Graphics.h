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
struct VertexAttribute {
    int Stride;
    std::vector<int> Length;
};

class Graphics {
public:
    static GLFWwindow* InitWindow(const unsigned int InWidth, const unsigned int InHeight);
    static RenderData DrawTexture(std::vector<float> InVertices, std::vector<unsigned int> InIndices, VertexAttribute InAttribute, const char* InFileName);
    static Shader DrawGrid(const GridConfig InGridConfig, int InWindowWidth, int InWindowHeight);
    static RenderData DrawUIElement(std::vector<float> InPosition, std::vector<float> InSize, const char* InTextureFilePath);
    static Shader InitTextRender(std::map<GLchar, Character>& InMap, float InWindowWidth, float InWindowHeight, unsigned int& InVAO, unsigned int& InVBO);
    static std::pair<int, int> GridToWorldPosition(int InGridX, int InGridY, float InTileWidth, float InTileHeight, float InOffsetX, float InOffsetY, float InPanX, float InPanY, float InItemScale, float InZoom, float InWindowWidth, float InWindowHeight, float InLevelWidth, float InLevelHeight);
    static std::vector<float> CreateGridVertices(float InGridWidth, float InGridHeight, float InOffsetX, float InOffsetY);
    static void RenderText(Shader& InShader, const unsigned int InVAO, const unsigned int InVBO, std::string InText, float InX, float InY, float InScale, glm::vec3 InColor, std::map<GLchar, Character>& InMap);
    static void FramebufferSizeCallback(GLFWwindow* InWindow, int InWidth, int InHeight);
};