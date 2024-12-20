#version 330 core
layout(location = 0) in vec2 aGridPos;

uniform vec2 tileSize;
uniform vec2 panOffset;
uniform float zoom;

void main() {
    float screenX = ((aGridPos.x - aGridPos.y) * tileSize.x * 0.5 + panOffset.x)*zoom;
    float screenY = ((aGridPos.x + aGridPos.y) * tileSize.y * 0.5 + panOffset.y)*zoom;
    gl_Position = vec4(screenX, screenY, 0.0, 1.0);
}