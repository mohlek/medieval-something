#ifndef H_SHADERS
#define H_SHADERS

#include <string>

namespace Shaders {
    std::string mainVert = R"(
        #version 420 core

        layout(location = 0) in vec3 in_position;
        layout(location = 1) in vec3 in_normal;
        layout(location = 2) in vec2 in_uv;
        layout(location = 3) in vec3 in_tangent;
        layout(location = 4) in vec3 in_bitangent;

        uniform mat4 model;
        uniform mat4 view;
        uniform mat4 projection;

        uniform vec3 lightPos = vec3(10.0, 300.0, 10.0);
        uniform vec3 viewPos = vec3(1.0, 1.0, 100.0);

        out VS_OUT {
            vec3 fragPos;
            vec2 uv;
            vec3 tangentLightPos;
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
            
            vs_out.tangentLightPos = TBN * lightPos;
            vs_out.tangentViewPos  = TBN * viewPos;
            vs_out.tangentFragPos  = TBN * vs_out.fragPos;
            
            gl_Position = projection * view * model * vec4(in_position, 1.0);
        }
    )";

    std::string mainFrag = R"(
        #version 420 core

        in VS_OUT {
            vec3 fragPos;
            vec2 uv;
            vec3 tangentLightPos;
            vec3 tangentViewPos;
            vec3 tangentFragPos;
        } fs_in;

        layout(binding=0) uniform sampler2D diffuseMap;
        layout(binding=1) uniform sampler2D normalMap;

        out vec4 fragColor;

        void main() {
            vec3 normal = texture(normalMap, fs_in.uv).rgb;
            normal = normalize(normal * 2.0 - 1.0);  // this normal is in tangent space
        
            vec3 color = texture(diffuseMap, fs_in.uv).rgb;
            vec3 ambient = 0.1 * color;
            vec3 lightDir = normalize(fs_in.tangentLightPos - fs_in.tangentFragPos);
            float diff = max(dot(lightDir, normal), 0.0);
            vec3 diffuse = diff * color;
            vec3 viewDir = normalize(fs_in.tangentViewPos - fs_in.tangentFragPos);
            vec3 reflectDir = reflect(-lightDir, normal);
            vec3 halfwayDir = normalize(lightDir + viewDir);  
            float spec = pow(max(dot(normal, halfwayDir), 0.0), 32.0);

            vec3 specular = vec3(0.2) * spec;
            fragColor = vec4(ambient + diffuse + specular, 1.0);
        }
    )";
}

#endif