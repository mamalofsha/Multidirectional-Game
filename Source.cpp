#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <iostream>
#include <string>
#include <cmath>
#include "TinyXML/tinyxml2.h"
#include <sstream>
#include "Graphics.h"
#include "XMLParser.h"
#include <functional>
#include "MouseInteraction.h"



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
    float gridX = (screenCoord.x / tileSize.x) + ( screenCoord.y / tileSize.y);
    float gridY = ( screenCoord.y / tileSize.y) - ( screenCoord.x / tileSize.x);

    if (int(gridX)==tileCoor.x && int(gridY)==tileCoor.y || (int(gridX)==tileCoor.x-1 && int(gridY)==tileCoor.y-1)) {
        FragColor = vec4(1.0, 0.0, 0.0, 1.0); // Highlight the hovered tile in red
    } else {
        FragColor = vec4(0.8, 0.8, 0.8, 1.0); // Default gray for other tiles
    }
}
)";
//    if (int(gridX) == tileCoor.x && int(gridY) == tileCoor.y) {

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


MouseState mouseState;
GridConfig gridConfig;




// Define callback functions
void onHoverFunction(int gridX, int gridY) {
    std::cout << "Hovered over tile: (" << gridX << ", " << gridY << ")" << std::endl;
    mouseState.GridX = gridX;
    mouseState.GridY = gridY;
}

void onClickFunction(int gridX, int gridY) {
    std::cout << "Clicked on tile: (" << gridX << ", " << gridY << ")" << std::endl;
    // Additional logic for click behavior
}


///







///







int main() {
    // Initialize GLFW
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW\n";
        return -1;
    }

    GLFWwindow* window = glfwCreateWindow(800, 800, "Isometric Grid with Mouse Interaction", nullptr, nullptr);
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


    // Mouse Interaction API setup
    MouseInteractionAPI mouseAPI;
    mouseAPI.setHoverCallback(onHoverFunction); // Use regular function
    mouseAPI.setClickCallback(onClickFunction); // Use regular function
    gridConfig = XMLParser::ParseGridDataFromXML("grid_config.xml");

    glfwSetCursorPosCallback(window, [](GLFWwindow* window, double xpos, double ypos) {
        int windowWidth, windowHeight;
        glfwGetWindowSize(window, &windowWidth, &windowHeight);
        mouseState.x = xpos;
        mouseState.y = ypos;
        MouseInteractionAPI* api = static_cast<MouseInteractionAPI*>(glfwGetWindowUserPointer(window));
        if (api) {
            api->handleMouseMove(xpos, ypos, windowWidth, windowHeight, gridConfig.tileWidth, gridConfig.tileHeight);
        }
        });
    glfwSetMouseButtonCallback(window, [](GLFWwindow* window, int button, int action, int mods) {
        if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
            double xpos, ypos;
            glfwGetCursorPos(window, &xpos, &ypos);
            int windowWidth, windowHeight;
            glfwGetWindowSize(window, &windowWidth, &windowHeight);

            MouseInteractionAPI* api = static_cast<MouseInteractionAPI*>(glfwGetWindowUserPointer(window));
            if (api) {
                api->handleMouseClick(xpos, ypos, windowWidth, windowHeight, gridConfig.tileWidth, gridConfig.tileHeight);
            }
        }
        });

    // Compile shaders and link program
    GLuint vertexShader = compileShader(GL_VERTEX_SHADER, vertexShaderSource);
    GLuint fragmentShader = compileShader(GL_FRAGMENT_SHADER, fragmentShaderSource);
    GLuint shaderProgram = linkProgram(vertexShader, fragmentShader);

    // Load grid configuration from XML

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
    GLint screenSizeLocation = glGetUniformLocation(shaderProgram, "screenSize");
    glUniform2f(screenSizeLocation, 800.0f, 800.0f);


    glfwSetWindowUserPointer(window, &mouseAPI);

    // Main render loop
    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT);


        glUseProgram(shaderProgram);
        GLint hoveredTileLocation = glGetUniformLocation(shaderProgram, "tileCoor");
        if (hoveredTileLocation == -1) {
            std::cerr << "Error: Uniform 'tileCoor' not found in the shader!" << std::endl;
        }
        else {
            glUniform2i(hoveredTileLocation, mouseState.GridX, mouseState.GridY);
        }

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



