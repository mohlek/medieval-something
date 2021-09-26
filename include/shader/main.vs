R"(
        #version 420 core

        layout(location = 0) in vec3 in_position;
        layout(location = 1) in vec3 in_normal;
        layout(location = 2) in vec2 in_uv;
        layout(location = 3) in vec3 in_tangent;
        layout(location = 4) in vec3 in_bitangent;

        uniform mat4 model;
        layout(std140) uniform camera {
            mat4 view;
            mat4 projection;
        };

        uniform uint time;

        out VS_OUT {
            vec3 fragPos;
            vec2 uv;
            mat3 TBN;
            vec3 tangentLightDir;
            vec3 tangentViewPos;
            vec3 tangentFragPos;
        } vs_out;

        void main() {
            vs_out.fragPos = vec3(model * vec4(in_position, 1.0));
            vs_out.uv = in_uv;

            mat3 normalMatrix = transpose(inverse(mat3(model)));
            vec3 T = normalize(normalMatrix * in_tangent);
            vec3 N = normalize(normalMatrix * in_normal);
            T = normalize(T - dot(T, N) * N);
            vec3 B = cross(N, T);
    
            mat3 TBN = transpose(mat3(T, B, N));
            vs_out.TBN = TBN;

            vec3 lightDir = normalize(vec3(sin(time * 0.001), 1.0, cos(time * 0.001)));
            vs_out.tangentLightDir = TBN * lightDir;
            vs_out.tangentViewPos  = TBN * transpose(view)[3].xyz;
            vs_out.tangentFragPos  = TBN * vs_out.fragPos;

            gl_Position = projection * view * model * vec4(in_position, 1.0);
        }
    )";