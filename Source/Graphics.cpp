#include "Graphics.h"
#include "stb_image.h"
#include "XMLParser.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <ft2build.h>
#include FT_FREETYPE_H

GLFWwindow* Graphics::InitWindow(const unsigned int InWidth, const unsigned int InHeight)
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	GLFWwindow* Window = glfwCreateWindow(InWidth, InHeight, "Castle", NULL, NULL);
	if (Window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return nullptr;
	}
	glfwMakeContextCurrent(Window);
	glfwSetFramebufferSizeCallback(Window, FramebufferSizeCallback);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return nullptr;
	}
	return Window;
}

RenderData Graphics::DrawTexture(std::vector<float> InVertices, std::vector<unsigned int> InIndices, VertexAttribute InAttribute, const char* InFileName)
{
	unsigned int VAO, VBO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, InVertices.size() * sizeof(float), InVertices.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, InIndices.size() * sizeof(unsigned int), InIndices.data(), GL_STATIC_DRAW);

	size_t Sum = 0;
	for (size_t i = 0; i < InAttribute.Length.size(); i++)
	{
		glVertexAttribPointer(i, InAttribute.Length[i], GL_FLOAT, GL_FALSE, InAttribute.Stride * sizeof(float), (void*)(Sum * sizeof(float)));
		glEnableVertexAttribArray(i);
		Sum += InAttribute.Length[i];
	}

	glBindVertexArray(0);

	unsigned int Texture;
	glGenTextures(1, &Texture);
	glBindTexture(GL_TEXTURE_2D, Texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int Width, Height, NrChannels;
	stbi_set_flip_vertically_on_load(true);
	unsigned char* Data = stbi_load(InFileName, &Width, &Height, &NrChannels, 0);
	if (Data)
	{
		GLenum Format = (NrChannels == 4) ? GL_RGBA : GL_RGB;
		glTexImage2D(GL_TEXTURE_2D, 0, Format, Width, Height, 0, Format, GL_UNSIGNED_BYTE, Data);
	}
	else
	{
		std::cerr << "Failed to load texture" << std::endl;
	}
	stbi_image_free(Data);
	RenderData OutData = { VAO, VBO, EBO, Texture };
	return OutData;
}

Shader Graphics::DrawGrid(const GridConfig InGridConfig, int InWindowWidth, int InWindowHeight)
{
	Shader GridShader("Source/Shaders/Grid.vert", "Source/Shaders/Grid.frag");
	std::vector<float> GridVertices = CreateGridVertices(InGridConfig.Width, InGridConfig.Height, InGridConfig.StartOffsetX, InGridConfig.StartOffsetY);

	GLuint VBO, VAO;
	glGenBuffers(1, &VBO);
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, GridVertices.size() * sizeof(float), GridVertices.data(), GL_STATIC_DRAW);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	GridShader.use();
	GLint TileSizeLocation = glGetUniformLocation(GridShader.ID, "tileSize");
	glUniform2f(TileSizeLocation, InGridConfig.TileWidth, InGridConfig.TileHeight);
	GLint ScreenSizeLocation = glGetUniformLocation(GridShader.ID, "screenSize");
	glUniform2f(ScreenSizeLocation, InWindowWidth, InWindowHeight);
	return GridShader;
}

Shader Graphics::InitTextRender(std::map<GLchar, Character>& InMap, float InWindowWidth, float InWindowHeight, unsigned int& InVAO, unsigned int& InVBO)
{
	Shader TextShader("Source/Shaders/Text.vert", "Source/Shaders/Text.frag");
	glm::mat4 Projection = glm::ortho(0.0f, InWindowWidth, 0.0f, InWindowHeight);
	TextShader.use();
	glUniformMatrix4fv(glGetUniformLocation(TextShader.ID, "Projection"), 1, GL_FALSE, glm::value_ptr(Projection));

	// Initialize FreeType
	FT_Library FreeTypeLibrary;
	if (FT_Init_FreeType(&FreeTypeLibrary))
	{
		std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
		return Shader();
	}

	FT_Face Face;
	if (FT_New_Face(FreeTypeLibrary, "Assets/Fonts/autobahn.ttf", 0, &Face))
	{
		std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;
		return Shader();
	}
	else
	{
		FT_Set_Pixel_Sizes(Face, 0, 48);
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

		for (unsigned char c = 0; c < 128; c++)
		{
			if (FT_Load_Char(Face, c, FT_LOAD_RENDER))
			{
				std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
				continue;
			}

			unsigned int Texture;
			glGenTextures(1, &Texture);
			glBindTexture(GL_TEXTURE_2D, Texture);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, Face->glyph->bitmap.width, Face->glyph->bitmap.rows, 0, GL_RED, GL_UNSIGNED_BYTE, Face->glyph->bitmap.buffer);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			Character NewCharacter = {
				Texture,
				glm::ivec2(Face->glyph->bitmap.width, Face->glyph->bitmap.rows),
				glm::ivec2(Face->glyph->bitmap_left, Face->glyph->bitmap_top),
				static_cast<unsigned int>(Face->glyph->advance.x)
			};
			InMap.insert(std::pair<char, Character>(c, NewCharacter));
		}
		glBindTexture(GL_TEXTURE_2D, 0);
	}
	FT_Done_Face(Face);
	FT_Done_FreeType(FreeTypeLibrary);

	glGenVertexArrays(1, &InVAO);
	glGenBuffers(1, &InVBO);
	glBindVertexArray(InVAO);
	glBindBuffer(GL_ARRAY_BUFFER, InVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	return TextShader;
}

std::pair<int, int> Graphics::GridToWorldPosition(int InGridX, int InGridY, float InTileWidth, float InTileHeight, float InOffsetX, float InOffsetY, float InPanX, float InPanY, float InItemScale, float InZoom, float InWindowWidth, float InWindowHeight, float InLevelWidth, float InLevelHeight)
{
	float IsoScreenX = (InGridX - InGridY + InOffsetX - InOffsetY) * (InTileWidth / 2.0f);
	float IsoScreenY = (InGridX + InGridY + InOffsetY + InOffsetX) * (InTileHeight / 2.0f);

	float IsoScreenYY = (InGridX + InGridY + 1 + InOffsetY + InOffsetX) * (InTileHeight / 2.0f);
	float AdjustedScreenYY = (IsoScreenYY + InPanY) * (InZoom * (InLevelHeight / InWindowHeight));
	float ScreenYY = (1.0f - AdjustedScreenYY) / 2.0f * InWindowHeight;

	float AdjustedScreenX = (IsoScreenX + InPanX) * (InZoom * (InLevelWidth / InWindowWidth));
	float AdjustedScreenY = (IsoScreenY + InPanY) * (InZoom * (InLevelHeight / InWindowHeight));

	float ScreenX = (AdjustedScreenX + 1.0f) / 2.0f * InWindowWidth;
	float ScreenY = (1.0f - AdjustedScreenY) / 2.0f * InWindowHeight;

	float OffsetY = (ScreenY - ScreenYY) * 3 / 2;
	ScreenY -= OffsetY;

	return { ScreenX, ScreenY };
}

RenderData Graphics::DrawUIElement(std::vector<float> InPosition, std::vector<float> InSize, const char* InTextureFilePath)
{
	float Vertices[] = {
		// Positions                       // Texture Coords
		InPosition[0] - (InSize[0] / 2.0f), InPosition[1] + (InSize[1] / 2.0f), 0.0f, 1.0f, // Top-left
		InPosition[0] + (InSize[0] / 2.0f), InPosition[1] + (InSize[1] / 2.0f), 1.0f, 1.0f, // Top-right
		InPosition[0] + (InSize[0] / 2.0f), InPosition[1] - (InSize[1] / 2.0f), 1.0f, 0.0f, // Bottom-right
		InPosition[0] - (InSize[0] / 2.0f), InPosition[1] - (InSize[1] / 2.0f), 0.0f, 0.0f  // Bottom-left
	};

	unsigned int Indices[] = {
		0, 1, 2, // First triangle
		2, 3, 0  // Second triangle
	};

	unsigned int VAO, VBO, EBO, Texture;

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);

	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Indices), Indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	glGenTextures(1, &Texture);
	glBindTexture(GL_TEXTURE_2D, Texture);

	int Width, Height, NrChannels;
	stbi_set_flip_vertically_on_load(true);
	unsigned char* Data = stbi_load(InTextureFilePath, &Width, &Height, &NrChannels, 0);
	if (Data) {
		GLenum Format = (NrChannels == 4) ? GL_RGBA : GL_RGB;
		glBindTexture(GL_TEXTURE_2D, Texture);
		glTexImage2D(GL_TEXTURE_2D, 0, Format, Width, Height, 0, Format, GL_UNSIGNED_BYTE, Data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}
	else {
		std::cerr << "Failed to load texture: " << InTextureFilePath << std::endl;
	}
	stbi_image_free(Data);

	RenderData OutData = { VAO, VBO, EBO, Texture };
	return OutData;
}

std::vector<float> Graphics::CreateGridVertices(float InGridWidth, float InGridHeight, float InOffsetX, float InOffsetY)
{
	std::vector<float> Vertices;

	for (int Y = 0; Y <= InGridHeight; ++Y) {
		Vertices.push_back(0.0f + InOffsetX);
		Vertices.push_back(static_cast<float>(Y) + InOffsetY);
		Vertices.push_back(static_cast<float>(InGridWidth) + InOffsetX);
		Vertices.push_back(static_cast<float>(Y) + InOffsetY);
	}

	for (int X = 0; X <= InGridWidth; ++X) {
		Vertices.push_back(static_cast<float>(X) + InOffsetX);
		Vertices.push_back(0.0f + InOffsetY);
		Vertices.push_back(static_cast<float>(X) + InOffsetX);
		Vertices.push_back(static_cast<float>(InGridHeight) + InOffsetY);
	}

	return Vertices;
}

void Graphics::RenderText(Shader& InShader, const unsigned int InVAO, const unsigned int InVBO, std::string InText, float InX, float InY, float InScale, glm::vec3 InColor, std::map<GLchar, Character>& InMap)
{
	InShader.use();
	glUniform3f(glGetUniformLocation(InShader.ID, "textColor"), InColor.x, InColor.y, InColor.z);
	glActiveTexture(GL_TEXTURE0);
	glBindVertexArray(InVAO);

	for (const auto& C : InText)
	{
		Character Ch = InMap[C];

		float XPos = InX + Ch.Bearing.x * InScale;
		float YPos = InY - (Ch.Size.y - Ch.Bearing.y) * InScale;

		float W = Ch.Size.x * InScale;
		float H = Ch.Size.y * InScale;
		float Vertices[6][4] = {
			{ XPos,     YPos + H,   0.0f, 0.0f },
			{ XPos,     YPos,       0.0f, 1.0f },
			{ XPos + W, YPos,       1.0f, 1.0f },

			{ XPos,     YPos + H,   0.0f, 0.0f },
			{ XPos + W, YPos,       1.0f, 1.0f },
			{ XPos + W, YPos + H,   1.0f, 0.0f }
		};

		glBindTexture(GL_TEXTURE_2D, Ch.TextureID);
		glBindBuffer(GL_ARRAY_BUFFER, InVBO);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(Vertices), Vertices);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		InX += (Ch.Advance >> 6) * InScale;
	}
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Graphics::FramebufferSizeCallback(GLFWwindow* InWindow, int InWidth, int InHeight)
{
	glViewport(0, 0, InWidth, InHeight);
}
