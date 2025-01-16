#include "Graphics.h"
#include "stb_image.h"
#include "XMLParser.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <ft2build.h>
#include FT_FREETYPE_H

GLFWwindow* Graphics::InitWindow(const GLuint InWidth, const GLuint InHeight)
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

RenderData Graphics::DrawTexture(std::vector<float> InVertices, std::vector<GLuint> InIndices, VertexAttribute InAttribute, const char* InFileName)
{
	GLuint VAO, VBO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, InVertices.size() * sizeof(float), InVertices.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, InIndices.size() * sizeof(GLuint), InIndices.data(), GL_STATIC_DRAW);

	size_t Sum = 0;
	for (size_t i = 0; i < InAttribute.Length.size(); i++)
	{
		glVertexAttribPointer(i, InAttribute.Length[i], GL_FLOAT, GL_FALSE, InAttribute.Stride * sizeof(float), (void*)(Sum * sizeof(float)));
		glEnableVertexAttribArray(i);
		Sum += InAttribute.Length[i];
	}

	glBindVertexArray(0);

	GLuint Texture;
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

RenderData Graphics::DrawGrid(std::weak_ptr<Shader> InShaderProgram,const GridConfig InGridConfig, int InWindowWidth, int InWindowHeight)
{
	std::vector<float> GridVertices = CreateGridVertices(static_cast<float>(InGridConfig.Width), static_cast<float>(InGridConfig.Height),
	InGridConfig.StartOffsetX, InGridConfig.StartOffsetY);
	int GridVerticesSize = GridVertices.size();
	GLuint VBO, VAO;
	glGenBuffers(1, &VBO);
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, GridVertices.size() * sizeof(float), GridVertices.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	if (auto SharedObjectShader = InShaderProgram.lock())
	{
		SharedObjectShader->use();
		SharedObjectShader->setUniform2f("TileSize", InGridConfig.TileWidth, InGridConfig.TileHeight);
		SharedObjectShader->setUniform2f("ScreenSize", InWindowWidth, InWindowHeight);
	}
	RenderData OutData = { VAO, VBO, GridVerticesSize, 0 };
	return OutData;
}

Shader Graphics::InitTextRender(std::map<GLchar, Character>& InMap, float InWindowWidth, float InWindowHeight, GLuint& InVAO, GLuint& InVBO)
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

			GLuint Texture;
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
				static_cast<GLuint>(Face->glyph->advance.x)
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
	float IsoScreenX = (InGridX - InGridY + InOffsetX - InOffsetY) * (InTileWidth );
	float IsoScreenY = (InGridX + InGridY + InOffsetY + InOffsetX) * (InTileHeight / 2.0f);
	// for that extra push up\offset
	float IsoScreenYY = (InGridX + InGridY + 1 + InOffsetY + InOffsetX) * (InTileHeight / 2.0f);
	float AdjustedScreenYY = (IsoScreenYY + InPanY) * (InZoom * (InLevelHeight / InWindowHeight));
	float ScreenYY = (1.0f - AdjustedScreenYY) / 2.0f * InWindowHeight;
	//
	float AdjustedScreenX = (IsoScreenX + InPanX) * (InZoom * (InLevelWidth / InWindowWidth));
	float AdjustedScreenY = (IsoScreenY + InPanY) * (InZoom * (InLevelHeight / InWindowHeight));
	// reverse ndc 
	float ScreenX = (AdjustedScreenX + 1.0f) / 2.0f * InWindowWidth;
	float ScreenY = (1.0f - AdjustedScreenY) / 2.0f * InWindowHeight;
	// that extra push
	float OffsetY = (ScreenY - ScreenYY) * 3 / 2;
	ScreenY -= OffsetY;
	return { ScreenX, ScreenY };
}

std::pair<int, int> Graphics::ScreenToGrid(double InScreenX, double InScreenY, float InTileWidth, float InTileHeight, float InOffsetX, float InOffsetY, float InZoom, float InPanX, float InPanY, int InWindowWidth, int InWindowHeight, float InLevelWidth, float InLevelHeight)
{
	float WindowAspectRatio = static_cast<float>(InWindowWidth) / static_cast<float>(InWindowHeight);
	float BgAspectRatio = InLevelWidth / InLevelHeight;
	float ScaleX = InLevelWidth / static_cast<float>(InWindowWidth);
	float ScaleY = InLevelHeight / static_cast<float>(InWindowHeight);
	float NdcX = (((((InScreenX) / InWindowWidth) * 2.0f - 1.0f) / (InZoom * ScaleX)) - InPanX) / InTileWidth;
	float NdcY = (((1.0f - ((InScreenY) / InWindowHeight) * 2.0f) / (InZoom * ScaleY)) - InPanY) / InTileWidth * 2.0f;
	// Convert NDC to approximate grid coordinates for isometric projection
	float ApproxGridX = ((NdcX + NdcY) - (2.0f * InOffsetX)) / 2;
	float ApproxGridY = ((NdcY - NdcX) - (2.0f * InOffsetY)) / 2;
	int GridX = static_cast<int>(std::floor(ApproxGridX));
	int GridY = static_cast<int>(std::floor(ApproxGridY));
	return { GridX, GridY };
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

	GLuint Indices[] = {
		0, 1, 2, // First triangle
		2, 3, 0  // Second triangle
	};

	GLuint VAO, VBO, EBO, Texture;

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
		Vertices.push_back(Y + InOffsetY);
		Vertices.push_back(InGridWidth + InOffsetX);
		Vertices.push_back(Y + InOffsetY);
	}
	
	for (int X = 0; X <= InGridWidth; ++X) {
		Vertices.push_back(X + InOffsetX);
		Vertices.push_back(0.0f + InOffsetY);
		Vertices.push_back(X + InOffsetX);
		Vertices.push_back(InGridHeight + InOffsetY);
	}
	
	return Vertices;
}

void Graphics::RenderText(Shader& InShader, const GLuint InVAO, const GLuint InVBO, std::string InText, float InX, float InY, float InScale, glm::vec3 InColor, std::map<GLchar, Character>& InMap)
{
	InShader.use();
	glUniform3f(glGetUniformLocation(InShader.ID, "textColor"), InColor.x, InColor.y, InColor.z);
	glActiveTexture(GL_TEXTURE0);
	glBindVertexArray(InVAO);

	// Iterate through characters in the text
	for (const auto& C : InText)
	{
		Character Ch = InMap[C];

		float XPos = InX + Ch.Bearing.x * InScale;
		float YPos = InY - (Ch.Size.y - Ch.Bearing.y) * InScale;

		float W = Ch.Size.x * InScale;
		float H = Ch.Size.y * InScale;

		// Create vertex array for the character quad
		float Vertices[6][4] = {
			{ XPos,     YPos + H,   0.0f, 0.0f },
			{ XPos,     YPos,       0.0f, 1.0f },
			{ XPos + W, YPos,       1.0f, 1.0f },

			{ XPos,     YPos + H,   0.0f, 0.0f },
			{ XPos + W, YPos,       1.0f, 1.0f },
			{ XPos + W, YPos + H,   1.0f, 0.0f }
		};

		// Render the character
		glBindTexture(GL_TEXTURE_2D, Ch.TextureID);
		glBindBuffer(GL_ARRAY_BUFFER, InVBO);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(Vertices), Vertices);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glDrawArrays(GL_TRIANGLES, 0, 6);

		// Advance cursor to the next character
		InX += (Ch.Advance >> 6) * InScale;
	}

	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Graphics::FramebufferSizeCallback(GLFWwindow* InWindow, int InWidth, int InHeight)
{
	glViewport(0, 0, InWidth, InHeight);
}
