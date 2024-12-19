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
uniform ivec2 tileCoor; // Hovered tile coordinate
uniform vec2 tileSize;  // Size of each tile
uniform vec2 screenSize; // Screen dimensions

void main() {
    vec2 screenCoord = (gl_FragCoord.xy / screenSize) * 2.0 - 1.0;
    float gridX = (2.0 * screenCoord.x / tileSize.x) + (2.0 * screenCoord.y / tileSize.y);
    float gridY = (2.0 * screenCoord.y / tileSize.y) - (2.0 * screenCoord.x / tileSize.x);

    if (int(gridX) == tileCoor.x && int(gridY) == tileCoor.y) {
        FragColor = vec4(1.0, 0.0, 0.0, 1.0); // Highlight the hovered tile in red
    } else {
        FragColor = vec4(0.8, 0.8, 0.8, 1.0); // Default gray for other tiles
    }
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

struct MouseState {
    double x, y;       // Current mouse position
    bool leftPressed;  // Is the left button pressed?
    bool rightPressed; // Is the right button pressed?
};

MouseState mouseState;

std::pair<int, int> screenToGrid(double screenX, double screenY, float tileWidth, float tileHeight, int windowWidth, int windowHeight) {
    // Step 1: Convert screen coordinates to Normalized Device Coordinates (NDC)
    float ndcX = (screenX / windowWidth) * 2.0f - 1.0f;
    float ndcY = 1.0f - (screenY / windowHeight) * 2.0f;

    // Debugging NDC values
  //  std::cout << "NDC Coordinates: (" << ndcX << ", " << ndcY << ")" << std::endl;

    // Step 2: Reverse isometric transformation to calculate grid indices
    float approxGridX = (ndcY / tileHeight) + (ndcX / tileWidth) / 2.0f;
    float approxGridY = (ndcY / tileHeight) - (ndcX / tileWidth) / 2.0f;

    // Debugging approximate grid indices before rounding
   // std::cout << "Approx Grid Coordinates: (" << approxGridX << ", " << approxGridY << ")" << std::endl;

    // Step 3: Round to the nearest integer grid index
    int gridX = static_cast<int>(round(approxGridX));
    int gridY = static_cast<int>(round(approxGridY));

    // Debugging final grid indices
    //std::cout << "Final Grid Coordinates: (" << gridX << ", " << gridY << ")" << std::endl;

    return { gridX, gridY };
}

void mousePositionCallback(GLFWwindow* window, double xpos, double ypos) {
    mouseState.x = xpos;
    mouseState.y = ypos;

    int windowWidth, windowHeight;
    glfwGetWindowSize(window, &windowWidth, &windowHeight);
    auto [gridX, gridY] = screenToGrid(xpos, ypos, TILE_WIDTH, TILE_HEIGHT, windowWidth, windowHeight);

    std::cout << "Mouse moved to grid: (" << gridX << ", " << gridY << ")" << std::endl;
}

void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        mouseState.leftPressed = true;

        int windowWidth, windowHeight;
        glfwGetWindowSize(window, &windowWidth, &windowHeight);
        auto [gridX, gridY] = screenToGrid(mouseState.x, mouseState.y, TILE_WIDTH, TILE_HEIGHT, windowWidth, windowHeight);

        std::cout << "Mouse clicked on grid: (" << gridX << ", " << gridY << ")" << std::endl;
    }
    else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE) {
        mouseState.leftPressed = false;
    }
}

int main() {
    // Initialize GLFW
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW\n";
        return -1;
    }

    GLFWwindow* window = glfwCreateWindow(800, 600, "Isometric Grid with Mouse Interaction", nullptr, nullptr);
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

    glfwSetCursorPosCallback(window, mousePositionCallback);
    glfwSetMouseButtonCallback(window, mouseButtonCallback);

    // Compile shaders and link program
    GLuint vertexShader = compileShader(GL_VERTEX_SHADER, vertexShaderSource);
    GLuint fragmentShader = compileShader(GL_FRAGMENT_SHADER, fragmentShaderSource);
    GLuint shaderProgram = linkProgram(vertexShader, fragmentShader);

    // Generate grid vertices
    std::vector<float> gridVertices = createGridVertices(GRID_WIDTH, GRID_HEIGHT);

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
    glUniform2f(tileSizeLocation, TILE_WIDTH, TILE_HEIGHT);
    GLint screenSizeLocation = glGetUniformLocation(shaderProgram, "screenSize");
    glUniform2f(screenSizeLocation, 800.0f, 600.0f);

    // Main render loop
    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT);

        int windowWidth, windowHeight;
        glfwGetWindowSize(window, &windowWidth, &windowHeight);

        auto [hoveredGridX, hoveredGridY] = screenToGrid(mouseState.x, mouseState.y, TILE_WIDTH, TILE_HEIGHT, windowWidth, windowHeight);

        glUseProgram(shaderProgram);
        GLint hoveredTileLocation = glGetUniformLocation(shaderProgram, "tileCoor");
        if (hoveredTileLocation == -1) {
            std::cerr << "Error: Uniform 'tileCoor' not found in the shader!" << std::endl;
        }
        else {
            glUniform2i(hoveredTileLocation, hoveredGridX, hoveredGridY);
        }
        float ndcX = (mouseState.x / windowWidth) * 2.0f - 1.0f;
        float ndcY = 1.0f - (mouseState.y / windowHeight) * 2.0f;
        int gridX = static_cast<int>(ceil(((2.0 * ndcX / 0.2) + (2.0 * ndcY / 0.1)) / 2));
        int gridY = static_cast<int>(ceil(((2.0 * ndcY / 0.1) - (2.0 * ndcX / 0.2))/2));


        std::cout << gridX << ",";
        std::cout << gridY << std::endl;

        glBindVertexArray(VAO);
        glDrawArrays(GL_POINTS, 0, gridVertices.size() / 2);

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
