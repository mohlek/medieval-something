R"(
        #version 420 core

        in vec2 texCoord;
        layout(binding = 1) uniform sampler2D groundTexture;
        
        out vec4 fragColor;

        void main() { 
            fragColor = texture(groundTexture, texCoord*500);    
        }
    )"