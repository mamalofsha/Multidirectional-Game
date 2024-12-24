#version 330 core
in vec2 TexCoord;           
out vec4 FragColor;        

uniform sampler2D texture1; 
uniform bool IsHovered;     
uniform bool IsHidden;     
uniform float Alpha =1.0;     
void main() {
    vec4 baseColor = texture(texture1, TexCoord); 
    if(IsHidden)
    {
     FragColor = vec4(0.0, 0.0, 0.0,0.0); 
    }
    else
    {
    if (IsHovered) {
        FragColor = baseColor *vec4(1.3, 1.3, 1.3,1.0); 
    } else {
        FragColor = baseColor *vec4(1.0, 1.0, 1.0,Alpha);
    }
    }
}