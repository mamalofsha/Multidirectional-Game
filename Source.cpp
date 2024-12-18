#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <iostream>
#include <string>
#include <cmath>
#include "TinyXML/tinyxml2.h"
#include <sstream>

// Grid settings
const int GRID_WIDTH = 10;
const int GRID_HEIGHT = 10;
const float TILE_WIDTH = 0.2f; // Tile width in normalized device coordinates
const float TILE_HEIGHT = 0.1f; // Tile height in normalized device coordinates

// Vertex shader source
const char* vertexShaderSource = R"(
#version 330 core
layout(location = 0) in vec2 aGridPos;

uniform vec2 tileSize;

void main() {
    float screenX = (aGridPos.x - aGridPos.y) * tileSize.x * 0.5;
    float screenY = (aGridPos.x + aGridPos.y) * tileSize.y * 0.5;
    gl_Position = vec4(screenX, screenY, 0.0, 1.0);
}
)";

// Fragment shader source
const char* fragmentShaderSource = R"(
#version 330 core
out vec4 FragColor;

void main() {
    FragColor = vec4(0.8, 0.8, 0.8, 1.0); // Light gray
}
)";

// Function to compile shader
GLuint compileShader(GLenum type, const char* source) {
    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &source, nullptr);
    glCompileShader(shader);

    int success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetShaderInfoLog(shader, 512, nullptr, infoLog);
        std::cerr << "ERROR::SHADER::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    return shader;
}

// Function to link shaders
GLuint linkProgram(GLuint vertexShader, GLuint fragmentShader) {
    GLuint program = glCreateProgram();
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);

    int success;
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetProgramInfoLog(program, 512, nullptr, infoLog);
        std::cerr << "ERROR::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
    return program;
}

// Function to create isometric grid vertices
std::vector<float> createGridVertices(int gridWidth, int gridHeight) {
    std::vector<float> vertices;

    for (int y = 0; y <= gridHeight; ++y) {
        for (int x = 0; x <= gridWidth; ++x) {
            vertices.push_back(static_cast<float>(x)); // Grid X
            vertices.push_back(static_cast<float>(y)); // Grid Y
        }
    }

    return vertices;
}




struct GridConfig {
    int width, height;
    float tileWidth, tileHeight;
    std::vector<std::vector<int>> tiles; // Stores the tile types (0 = empty, 1 = occupied, etc.)
};

GridConfig readGridFromXML(const std::string& filename) {
    GridConfig config;

    // Load XML file
    tinyxml2::XMLDocument doc;
    if (doc.LoadFile(filename.c_str()) != tinyxml2::XML_SUCCESS) {
        std::cerr << "Error loading XML file: " << filename << std::endl;
        return config;
    }

    // Parse dimensions
    auto* root = doc.FirstChildElement("grid");
    if (!root) {
        std::cerr << "Invalid XML structure: No <grid> element." << std::endl;
        return config;
    }

    auto* dimensions = root->FirstChildElement("dimensions");
    if (dimensions) {
        dimensions->FirstChildElement("width")->QueryIntText(&config.width);
        dimensions->FirstChildElement("height")->QueryIntText(&config.height);
    }

    // Parse tile size
    auto* tile = root->FirstChildElement("tile");
    if (tile) {
        tile->FirstChildElement("width")->QueryFloatText(&config.tileWidth);
        tile->FirstChildElement("height")->QueryFloatText(&config.tileHeight);
    }

    // Parse tiles
    auto* tilesElement = root->FirstChildElement("tiles");
    if (tilesElement) {
        for (auto* row = tilesElement->FirstChildElement("row"); row; row = row->NextSiblingElement("row")) {
            std::vector<int> tileRow;
            const char* rowText = row->GetText();
            std::stringstream ss(rowText); // Initialize with rowText
            int tileType;
            while (ss >> tileType) {
                tileRow.push_back(tileType);
            }
            config.tiles.push_back(tileRow);
        }
    }

    return config;
}



int main() {
    // Initialize GLFW
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW\n";
        return -1;
    }

    GLFWwindow* window = glfwCreateWindow(800, 600, "Isometric Grid with XML", nullptr, nullptr);
    if (!window) {
        std::cerr << "Failed to create GLFW window\n";
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // Load OpenGL functions using GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD\n";
        return -1;
    }

    // Compile shaders and link program
    GLuint vertexShader = compileShader(GL_VERTEX_SHADER, vertexShaderSource);
    GLuint fragmentShader = compileShader(GL_FRAGMENT_SHADER, fragmentShaderSource);
    GLuint shaderProgram = linkProgram(vertexShader, fragmentShader);

    // Load grid configuration from XML
    GridConfig gridConfig = readGridFromXML("grid_config.xml");

    // Generate grid vertices based on the XML data
    std::vector<float> gridVertices = createGridVertices(gridConfig.width, gridConfig.height);

    // Create vertex buffer and array objects
    GLuint VBO, VAO;
    glGenBuffers(1, &VBO);
    glGenVertexArrays(1, &VAO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, gridVertices.size() * sizeof(float), gridVertices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Set uniform values
    glUseProgram(shaderProgram);
    GLint tileSizeLocation = glGetUniformLocation(shaderProgram, "tileSize");
    glUniform2f(tileSizeLocation, gridConfig.tileWidth, gridConfig.tileHeight);

    // Main render loop
    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT);

        // Draw grid
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        glDrawArrays(GL_POINTS, 0, gridVertices.size() / 2); // Drawing each vertex

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Clean up
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);
    glfwTerminate();

    return 0;
}



