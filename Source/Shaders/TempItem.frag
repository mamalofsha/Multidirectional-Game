#version 330 core
out vec4 FragColor;
in vec2 TexCoord;
uniform sampler2D texture1;
uniform bool isOverlapping;    

void main() 
{
    if(isOverlapping)
    {
            FragColor = vec4(1.0, 0.0, 0.0, 1.0) *texture(texture1, TexCoord) ; // Solid red color
    }
    else
    {
            FragColor = texture(texture1, TexCoord);
    }
}