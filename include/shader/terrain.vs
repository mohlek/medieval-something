R"(
        #version 420 core

        layout(location = 0) in vec2 in_position;
        layout(location = 1) in vec3 in_normal;
        layout(location = 2) in vec2 in_uv;
        layout(location = 3) in vec3 in_tangent;
        layout(location = 4) in vec3 in_bitangent;

        uniform mat4 model;
        layout(std140, binding = 0) uniform camera {
            mat4 view;
            mat4 projection;
        };

        layout(binding = 0) uniform sampler2D heightMap;

        uniform float factor = 1.0;

        out vec2 texCoord;

        void main() {
          float height = texture(heightMap, in_position).r * factor;
          gl_Position = projection * view * model * vec4(in_position.x, height, in_position.y, 1.0);
          texCoord = in_position;
        }
    )"