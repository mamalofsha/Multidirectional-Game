        #version 330 core
        layout(location = 0) in vec2 aPos;
        layout(location = 1) in vec2 aTexCoord;
        out vec2 TexCoord;
        uniform vec2 mousePos;
        uniform float size;
        void main() {
            vec2 position = aPos * size + mousePos;
            gl_Position = vec4(position, 0.0, 1.0);
            TexCoord = aTexCoord;
        }