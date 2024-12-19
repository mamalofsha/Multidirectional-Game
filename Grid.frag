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
        FragColor = vec4(1.0, 0.0, 0.0, 1.0); // Default gray for other tiles
    }
}