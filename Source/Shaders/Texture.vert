#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTexCoord;
out vec3 ourColor;
out vec2 TexCoord;
uniform vec3 panOffset;
uniform float zoom;
uniform mat4 Transform;

void main()
{ 
ourColor = aColor;
TexCoord = aTexCoord;
    gl_Position = Transform * vec4(aPos, 1.0); // Apply transformation
}


