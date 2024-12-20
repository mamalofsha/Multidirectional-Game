#version 330 core
layout(location = 0) in vec2 aPos; // Position attribute
layout(location = 1) in vec3 aColor; // Color attribute

out vec3 vertexColor; // Pass color to fragment shader

void main() {
    gl_Position = vec4(aPos, 0.0, 1.0); // Convert to clip space
    vertexColor = aColor;              // Pass color
}
