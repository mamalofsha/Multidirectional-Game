#version 330 core
in vec3 vertexColor; // Input from vertex shader
out vec4 FragColor;  // Output fragment color

void main() {
    FragColor = vec4(vertexColor, 1.0); // RGBA color
}
