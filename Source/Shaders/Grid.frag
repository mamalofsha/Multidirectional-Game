#version 330 core
out vec4 FragColor;
uniform ivec2 TileCoor; // Hovered tile coordinate
uniform vec2 TileSize;  // Size of each tile
uniform vec2 ScreenSize; // Screen dimensions
uniform bool IsHidden; // Screen dimensions

void main() {
    if(IsHidden)
    {
    FragColor = vec4(1.0, 0.0, 0.0, 0.0);
    return;
    }

    vec2 screenCoord = (gl_FragCoord.xy / ScreenSize) * 2.0 - 1.0;
    float gridX = (screenCoord.x / TileSize.x) + ( screenCoord.y / TileSize.y);
    float gridY = ( screenCoord.y / TileSize.y) - ( screenCoord.x / TileSize.x);

    if (int(gridX)==TileCoor.x && int(gridY)==TileCoor.y || (int(gridX)==TileCoor.x-1 && int(gridY)==TileCoor.y-1)) {
        FragColor = vec4(1.0, 0.0, 0.0, 1.0); // Highlight the hovered tile in red
    } else {
        FragColor = vec4(1.0, 0.0, 0.0, 1.0); // Default gray for other tiles
    }
}