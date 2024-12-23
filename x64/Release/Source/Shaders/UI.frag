#version 330 core
in vec2 TexCoord;           // Texture coordinates from vertex shader
out vec4 FragColor;         // Output fragment color

uniform sampler2D texture1; // Texture sampler
uniform bool IsHovered;     // Hover state
uniform bool IsHidden;     // Hover state

void main() {
    vec4 baseColor = texture(texture1, TexCoord); // Sample the texture
    if(IsHidden)
    {
     FragColor = vec4(0.0, 0.0, 0.0,0.0); // Slightly brighter
    }
    else
    {
    if (IsHovered) {
        // Apply a light tint when hovered
        FragColor = baseColor *vec4(1.3, 1.3, 1.3,1.0); // Slightly brighter
    } else {
        FragColor = baseColor;
    }
    }
}