#include "Graphics.h"
#include "stb_image.h"
#include "GameObject.h"
#include "XMLParser.h"
#include "Button.h"

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
	// build and compile our texture shader
		// ------------------------------------
	Shader ourShader("Texture.vert", "Texture.frag");
	// set up vertex data (and buffer(s)) and configure vertex attributes
	// ------------------------------------------------------------------
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
	unsigned int VBO, VAO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	glBindVertexArray(VAO);
	ourShader.VAO = VAO;
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	ourShader.VBO = VBO;
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	ourShader.EBO = EBO;
	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	// texture coord attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);
	// load and create a texture 
	// -------------------------
	unsigned int texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture); // all upcoming GL_TEXTURE_2D operations now have effect on this texture object
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// load image, create texture and generate mipmaps
	ourShader.Texture = texture;
	int width, height, nrChannels;
	// The FileSystem::getPath(...) is part of the GitHub repository so we can find files on any IDE/platform; replace it with your own image path.
	stbi_set_flip_vertically_on_load(1);
	unsigned char* data = stbi_load(InFileName, &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);
	return ourShader;
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
	ourShader.VAO = VAO;
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, gridVertices.size() * sizeof(float), gridVertices.data(), GL_STATIC_DRAW);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	ourShader.VBO = VBO;

	// Set uniform values
	ourShader.use();
	GLint tileSizeLocation = glGetUniformLocation(ourShader.ID, "tileSize");
	glUniform2f(tileSizeLocation, InGridConfig.tileWidth, InGridConfig.tileHeight);
	GLint screenSizeLocation = glGetUniformLocation(ourShader.ID, "screenSize");
	glUniform2f(screenSizeLocation, WindowsWidth, WindowsHeight);
	return ourShader;
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
		InObject.ObjectShader->VAO = VAO;
		InObject.ObjectShader->VBO = VBO;
		InObject.ObjectShader->use();
		glDrawArrays(GL_TRIANGLES, 0, 3);
}

void Graphics::DrawButton(Button& InObject, const char* textureFilePath)
{
	InObject.ObjectShader = std::make_unique<Shader>("UI.vert", "UI.frag");
	unsigned int VBO, VAO, EBO;

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
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
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
	InObject.ObjectShader->VAO = VAO;
	InObject.ObjectShader->VBO = VBO;

	// Load the texture
	unsigned int textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);

	// Load texture data
	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true); // Flip image vertically
	unsigned char* data = stbi_load(textureFilePath, &width, &height, &nrChannels, 0);
	if (data) {
		GLenum format = (nrChannels == 4) ? GL_RGBA : GL_RGB; // Detect alpha channel
		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		// Set texture parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		InObject.ObjectShader->Texture = textureID; // Assign the texture ID to the button
	}
	else {
		std::cerr << "Failed to load texture: " << textureFilePath << std::endl;
	}
	stbi_image_free(data);

	// Use the shader and set uniforms
	InObject.ObjectShader->use();
	glUniform1i(glGetUniformLocation(InObject.ObjectShader->ID, "texture1"), 0); // Texture unit 0

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
	InObject.ObjectShader->VAO = VAO;
	InObject.ObjectShader->VBO = VBO;
	InObject.ObjectShader->use();
	glDrawArrays(GL_TRIANGLES, 0, 3);
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

void Graphics::framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}
