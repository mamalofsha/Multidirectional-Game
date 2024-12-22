#include "Graphics.h"
#include "stb_image.h"
#include "GameObject.h"
#include "XMLParser.h"
#include "UIButton.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "UIElement.h"
#include <ft2build.h>
#include FT_FREETYPE_H

GLFWwindow* Graphics::InitWindow(const unsigned int Width, const unsigned int Height)
{
	// glfw: initialize and configure
// ------------------------------
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	// glfw window creation
	// --------------------
	GLFWwindow* Window = glfwCreateWindow(Width, Height, "Ast", NULL, NULL);
	if (Window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return nullptr;
	}
	glfwMakeContextCurrent(Window);
	glfwSetFramebufferSizeCallback(Window, framebuffer_size_callback);

	// glad: load all OpenGL function pointers
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return nullptr;
	}
	return Window;
}

Shader Graphics::DrawTexture(const char* InFileName)
{
	Shader ourShader("Texture.vert", "Texture.frag");

	// Vertex and index data
	float vertices[] = {
		// positions          // colors           // texture coords
		 1.0f,  1.0f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
		 1.0f, -1.0f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right
		-1.0f, -1.0f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
		-1.0f,  1.0f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // top left 
	};
	unsigned int indices[] = {
		0, 1, 3, // first triangle
		1, 2, 3  // second triangle
	};

	// Create VAO, VBO, and EBO
	unsigned int VAO, VBO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// Set vertex attribute pointers
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	glBindVertexArray(0); // Unbind VAO

	// Load texture
	unsigned int texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	// Texture parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// Load image
	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true);
	unsigned char* data = stbi_load(InFileName, &width, &height, &nrChannels, 0);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else {
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);

	// Assign VAO and Texture to shader
	//ourShader.VAO = VAO;
	//ourShader.VBO = VBO;
	//ourShader.EBO = EBO;
	//ourShader.Texture = texture;

	return ourShader;
}

Shader Graphics::GenerateTextureData(const std::vector<float>& IntexturedVertices, const std::vector<float>& IngridVertices)
{
	return Shader();
}

Shader Graphics::DrawGrid(const GridConfig InGridConfig, int WindowsWidth, int WindowsHeight)
{
	Shader ourShader("Grid.vert", "Grid.frag");

	std::vector<float> gridVertices = createGridVertices(InGridConfig.width, InGridConfig.height,InGridConfig.StartOffsetX,InGridConfig.StartOffsetY);

	// Create vertex buffer and array objects
	GLuint VBO, VAO;
	glGenBuffers(1, &VBO);
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	//ourShader.VAO = VAO;
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, gridVertices.size() * sizeof(float), gridVertices.data(), GL_STATIC_DRAW);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	//ourShader.VBO = VBO;

	// Set uniform values
	ourShader.use();
	GLint tileSizeLocation = glGetUniformLocation(ourShader.ID, "tileSize");
	glUniform2f(tileSizeLocation, InGridConfig.tileWidth, InGridConfig.tileHeight);
	GLint screenSizeLocation = glGetUniformLocation(ourShader.ID, "screenSize");
	glUniform2f(screenSizeLocation, WindowsWidth, WindowsHeight);
	return ourShader;
}

Shader Graphics::InitTextRender(std::map<GLchar, Character>& InMap,float inWindowWidth,float inWindowHeight,unsigned int& VAO, unsigned int& VBO)
{
	// OpenGL state
	// ------------
	//glEnable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// compile and setup the shader
	// ----------------------------
	Shader shader("Text.vert", "Text.frag");
	glm::mat4 projection = glm::ortho(0.0f, inWindowWidth, 0.0f, inWindowHeight);
	shader.use();
	glUniformMatrix4fv(glGetUniformLocation(shader.ID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

	// FreeType
	// --------
	FT_Library ft;
	// All functions return a value different than 0 whenever an error occurred
	if (FT_Init_FreeType(&ft))
	{
		std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
		return Shader();
	}

	// find path to font


	// load font as face
	FT_Face face;
	if (FT_New_Face(ft, "C:/Users/A/Desktop/PVE/autobahn.ttf", 0, &face)) {
		std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;
		return Shader();
	}
	else {
		// set size to load glyphs as
		FT_Set_Pixel_Sizes(face, 0, 48);

		// disable byte-alignment restriction
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

		// load first 128 characters of ASCII set
		for (unsigned char c = 0; c < 128; c++)
		{
			// Load character glyph 
			if (FT_Load_Char(face, c, FT_LOAD_RENDER))
			{
				std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
				continue;
			}
			// generate texture
			unsigned int texture;
			glGenTextures(1, &texture);
			glBindTexture(GL_TEXTURE_2D, texture);
			glTexImage2D(
				GL_TEXTURE_2D,
				0,
				GL_RED,
				face->glyph->bitmap.width,
				face->glyph->bitmap.rows,
				0,
				GL_RED,
				GL_UNSIGNED_BYTE,
				face->glyph->bitmap.buffer
			);
			// set texture options
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			// now store character for later use
			Character character = {
				texture,
				glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
				glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
				static_cast<unsigned int>(face->glyph->advance.x)
			};
			InMap.insert(std::pair<char, Character>(c, character));
		}
		glBindTexture(GL_TEXTURE_2D, 0);
	}
	// destroy FreeType once we're finished
	FT_Done_Face(face);
	FT_Done_FreeType(ft);


	// configure VAO/VBO for texture quads
	// -----------------------------------
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	//shader.VAO = VAO;
	//shader.VBO = VBO;
	return shader;
}

void Graphics::DrawShape(GameObject& InObject)
{
		//Shader TriShader("Shader.vert", "Shader.frag"); // you can name your shader files however you like
		InObject.ObjectShader = std::make_unique<Shader>("Shader.vert", "Shader.frag");
		float vertices[] = {
			// positions                                                                                                                                                                                                                                                 // colors
			InObject.GetTransform().Location[0] + InObject.GetLength() * std::cos(InObject.GetTransform().Rotation - (3 * PI / -1.0f))    , InObject.GetTransform().Location[1] + InObject.GetLength() * std::sin(InObject.GetTransform().Rotation - (3 * PI / -1.0f))    , 0.0f,  1.0f, 0.0f, 0.0f,  // bottom right
			InObject.GetTransform().Location[0] + InObject.GetLength() * std::cos(InObject.GetTransform().Rotation + (3 * PI / -1.0f))    , InObject.GetTransform().Location[1] + InObject.GetLength() * std::sin(InObject.GetTransform().Rotation + (3 * PI / -1.0f))    , 0.0f,  0.0f, 1.0f, 0.0f,  // bottom left
			InObject.GetTransform().Location[0] + InObject.GetLength() * std::cos(InObject.GetTransform().Rotation)                      , InObject.GetTransform().Location[1] + InObject.GetLength() * std::sin(InObject.GetTransform().Rotation)                      , 0.0f,  0.0f, 0.0f, 1.0f   // top 
		};
		unsigned int VBO, VAO;
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
		// position attribute
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		// color attribute
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);
		glBindVertexArray(VAO);
		//InObject.ObjectShader->VAO = VAO;
		//InObject.ObjectShader->VBO = VBO;
		InObject.ObjectShader->use();
		glDrawArrays(GL_TRIANGLES, 0, 3);
}

void Graphics::DrawUIElement(std::weak_ptr<Shader> ShaderProgram, UIElement& InObject, const char* textureFilePath)
{

	// Define vertex data for the button (with texture coordinates)
	float vertices[] = {
		// Positions                       // Texture Coords
		InObject.x - (InObject.width / 2.0f), InObject.y + (InObject.height / 2.0f), 0.0f, 1.0f, // Top-left
		InObject.x + (InObject.width / 2.0f), InObject.y + (InObject.height / 2.0f), 1.0f, 1.0f, // Top-right
		InObject.x + (InObject.width / 2.0f), InObject.y - (InObject.height / 2.0f), 1.0f, 0.0f, // Bottom-right
		InObject.x - (InObject.width / 2.0f), InObject.y - (InObject.height / 2.0f), 0.0f, 0.0f  // Bottom-left
	};

	unsigned int indices[] = {
		0, 1, 2, // First triangle
		2, 3, 0  // Second triangle
	};

	// Generate VAO and VBO
	glGenVertexArrays(1, &InObject.VAO);
	glGenBuffers(1, &InObject.VBO);

	glBindVertexArray(InObject.VAO);

	glBindBuffer(GL_ARRAY_BUFFER, InObject.VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glGenBuffers(1, &InObject.EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, InObject.EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// Position attribute
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// Texture attribute
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	// Assign VAO and VBO to the button's shader
	//InObject.ObjectShader->VAO = VAO;
	//InObject.ObjectShader->VBO = VBO;
	// Load the texture
	glGenTextures(1, &InObject.Texture);
	glBindTexture(GL_TEXTURE_2D, InObject.Texture);

	// Load texture data
	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true); // Flip image vertically
	unsigned char* data = stbi_load(textureFilePath, &width, &height, &nrChannels, 0);
	if (data) {
		GLenum format = (nrChannels == 4) ? GL_RGBA : GL_RGB; // Detect alpha channel
		glBindTexture(GL_TEXTURE_2D, InObject.Texture);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		// Set texture parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		///InObject.ObjectShader->Texture = textureID; // Assign the texture ID to the button
	}
	else {
		std::cerr << "Failed to load texture: " << textureFilePath << std::endl;
	}
	stbi_image_free(data);

	// Use the shader and set uniforms

		if (auto sharedPtr = ShaderProgram.lock()) { // Check if the object is still valid
			glUniform1i(glGetUniformLocation(sharedPtr->ID, "texture1"), 0); // Texture unit 0
		}
		else {
			std::cout << "The object has already been destroyed." << std::endl;
		}
}


void Graphics::DrawShape2(GameObject& InObject)
{
	//Shader TriShader("Shader.vert", "Shader.frag"); // you can name your shader files however you like
	InObject.ObjectShader = std::make_unique<Shader>("Shader.vert", "Shader.frag");
	bool haz = InObject.GetIsHazard();
	if (haz)
	{

	}
	float vertices[] = {
		// positions																																																														 // colors
		InObject.GetTransform().Location[0] + InObject.GetLength() * std::cos(InObject.GetTransform().Rotation - (3 * PI / -1.0f))    , InObject.GetTransform().Location[1] + InObject.GetLength() * std::sin(InObject.GetTransform().Rotation - (3 * PI / -1.0f))    , 0.0f ,  haz ? 1.0f : 0.0f, haz ? 0.0f : 1.0f, 0.0f,  // bottom right
		InObject.GetTransform().Location[0] + InObject.GetLength() * std::cos(InObject.GetTransform().Rotation + (3 * PI / -1.0f))    , InObject.GetTransform().Location[1] + InObject.GetLength() * std::sin(InObject.GetTransform().Rotation + (3 * PI / -1.0f))    , 0.0f ,  haz ? 1.0f : 0.0f, haz ? 0.0f : 1.0f, 0.0f,  // bottom left
		InObject.GetTransform().Location[0] + InObject.GetLength() * std::cos(InObject.GetTransform().Rotation)                      , InObject.GetTransform().Location[1] + InObject.GetLength() * std::sin(InObject.GetTransform().Rotation)                      , 0.0f ,  haz ? 1.0f : 0.0f, haz ? 0.0f : 1.0f, 1.0f   // top 
	};
	unsigned int VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glBindVertexArray(VAO);
	//InObject.ObjectShader->VAO = VAO;
	//InObject.ObjectShader->VBO = VBO;
	//InObject.ObjectShader->use();
	//glDrawArrays(GL_TRIANGLES, 0, 3);
}

std::vector<float> Graphics::createGridVertices(float gridWidth, float gridHeight, float OffsetX, float OffsetY)
{
	std::vector<float> vertices;

	// Horizontal lines
	for (int y = 0; y <= gridHeight; ++y) {
		vertices.push_back(0.0f + OffsetX);             // Start x
		vertices.push_back(static_cast<float>(y)+ OffsetY); // Start y
		vertices.push_back(static_cast<float>(gridWidth)+ OffsetX); // End x
		vertices.push_back(static_cast<float>(y)+ OffsetY); // End y
	}

	// Vertical lines
	for (int x = 0; x <= gridWidth; ++x) {
		vertices.push_back(static_cast<float>(x)+ OffsetX); // Start x
		vertices.push_back(0.0f + OffsetY);             // Start y
		vertices.push_back(static_cast<float>(x) + OffsetX); // End x
		vertices.push_back(static_cast<float>(gridHeight) + OffsetY); // End y
	}

	return vertices;
}

void Graphics::RenderText(Shader& shader, const unsigned int VAO, const unsigned int VBO,std::string text, float x, float y, float scale, glm::vec3 color, std::map<GLchar, Character>& InMap)
{
	// activate corresponding render state	
	shader.use();
	glUniform3f(glGetUniformLocation(shader.ID, "textColor"), color.x, color.y, color.z);
	glActiveTexture(GL_TEXTURE0);
	glBindVertexArray(VAO);

	// iterate through all characters
	std::string::const_iterator c;
	for (c = text.begin(); c != text.end(); c++)
	{
		Character ch = InMap[*c];

		float xpos = x + ch.Bearing.x * scale;
		float ypos = y - (ch.Size.y - ch.Bearing.y) * scale;

		float w = ch.Size.x * scale;
		float h = ch.Size.y * scale;
		// update VBO for each character
		float vertices[6][4] = {
			{ xpos,     ypos + h,   0.0f, 0.0f },
			{ xpos,     ypos,       0.0f, 1.0f },
			{ xpos + w, ypos,       1.0f, 1.0f },

			{ xpos,     ypos + h,   0.0f, 0.0f },
			{ xpos + w, ypos,       1.0f, 1.0f },
			{ xpos + w, ypos + h,   1.0f, 0.0f }
		};
		// render glyph texture over quad
		glBindTexture(GL_TEXTURE_2D, ch.TextureID);
		// update content of VBO memory
		glBindBuffer(GL_ARRAY_BUFFER,VBO);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices); // be sure to use glBufferSubData and not glBufferData

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		// render quad
		glDrawArrays(GL_TRIANGLES, 0, 6);
		// now advance cursors for next glyph (note that advance is number of 1/64 pixels)
		x += (ch.Advance >> 6) * scale; // bitshift by 6 to get value in pixels (2^6 = 64 (divide amount of 1/64th pixels by 64 to get amount of pixels))
	}
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Graphics::framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}
