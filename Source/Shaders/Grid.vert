#version 330 core
layout(location = 0) in vec2 aGridPos;

uniform vec2 tileSize;
uniform vec2 panOffset;
uniform float zoom;
uniform mat4 transform;

void main() {
    float screenX = ((aGridPos.x - aGridPos.y) * tileSize.x * 0.5 );
    float screenY = ((aGridPos.x + aGridPos.y) * tileSize.y * 0.5 );
    gl_Position = transform *vec4(screenX, screenY, 0.0, 1.0);
}