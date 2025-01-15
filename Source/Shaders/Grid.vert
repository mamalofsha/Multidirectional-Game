#version 330 core
layout(location = 0) in vec2 aGridPos;
uniform vec2 TileSize;
uniform mat4 Transform;
void main() {
/// 45 rotated grid for isometric feel
    float screenX = ((aGridPos.x - aGridPos.y) * TileSize.x );
    float screenY = ((aGridPos.x + aGridPos.y) * TileSize.y * 0.5 );
   /// straight grid
    //float screenX = aGridPos.x * TileSize.x;
    //float screenY = aGridPos.y * TileSize.y;
    gl_Position = Transform *vec4(screenX, screenY, 0.0, 1.0);
}