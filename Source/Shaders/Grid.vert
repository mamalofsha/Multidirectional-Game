#version 330 core
layout(location = 0) in vec2 aGridPos;
uniform vec2 TileSize;
uniform mat4 Transform;
void main() {
    float screenX = ((aGridPos.x - aGridPos.y) * TileSize.x * 0.5 );
    float screenY = ((aGridPos.x + aGridPos.y) * TileSize.y * 0.5 );
    gl_Position = Transform *vec4(screenX, screenY, 0.0, 1.0);
}