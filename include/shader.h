#ifndef H_SHADERS
#define H_SHADERS

#include <string>

namespace Shader {
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

            vec3 lightDir = normalize(vec3(sin(time * 0.001), -1.0, cos(time * 0.001)));
            vs_out.tangentLightDir = lightDir;
            vs_out.tangentViewPos  = TBN * transpose(view)[3].xyz;
            vs_out.tangentFragPos  = TBN * vs_out.fragPos;

            gl_Position = projection * view * model * vec4(in_position, 1.0);
        }
    )";

    std::string mainFrag = R"(
        #version 420 core

        in VS_OUT {
            vec3 fragPos;
            vec2 uv;
            mat3 TBN;
            vec3 tangentLightDir;
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
            vec3 lightDir = fs_in.tangentLightDir;
            float diff = max(dot(lightDir, normal), 0.0);
            vec3 diffuse = diff * color;
            vec3 viewDir = normalize(fs_in.tangentViewPos - fs_in.tangentFragPos);
            vec3 reflectDir = reflect(-lightDir, normal);
            vec3 halfwayDir = normalize(lightDir + viewDir);  
            float spec = pow(max(dot(normal, halfwayDir), 0.0), 32.0);

            vec3 specular = vec3(0.2) * spec;
            fragColor = vec4(ambient + diffuse + specular, 1.0); // + specular, 1.0);
        }
    )";
    
    std::string normalDisplayVert = R"(
        #version 420 core
        layout(location = 0) in vec3 in_position;
        layout(location = 1) in vec3 in_normal;
        layout(location = 2) in vec2 in_uv;
        layout(location = 3) in vec3 in_tangent;
        layout(location = 4) in vec3 in_bitangent;
      
        out VS_OUT {
            vec3 normal;
        } vs_out;
        
        uniform mat4 view;
        uniform mat4 model;
        
        void main()
        {
            gl_Position = view * model * vec4(in_position, 1.0); 
            mat3 normalMatrix = mat3(transpose(inverse(view * model)));
            vs_out.normal = normalize(vec3(vec4(normalMatrix * in_normal, 0.0)));
        }
    )";

    std::string normalDisplayGeo = R"(
        #version 420 core
        layout (triangles) in;
        layout (line_strip, max_vertices = 6) out;
        
        in VS_OUT {
            vec3 normal;
        } gs_in[];
        
        const float MAGNITUDE = 3.0;
          
        uniform mat4 projection;
        
        void GenerateLine(int index)
        {
            gl_Position = projection * gl_in[index].gl_Position;
            EmitVertex();
            gl_Position = projection * (gl_in[index].gl_Position + 
                                        vec4(gs_in[index].normal, 0.0) * MAGNITUDE);
            EmitVertex();
            EndPrimitive();
        }
        
        void main()
        {
            GenerateLine(0); // first vertex normal
            GenerateLine(1); // second vertex normal
            GenerateLine(2); // third vertex normal
        } 
    )";

    std::string normalDisplayFrag = R"(
        #version 420 core
        out vec4 FragColor;

        void main()
        {
            FragColor = vec4(1.0, 1.0, 0.0, 1.0);
        }   
    )";

    std::string terrainVertex = R"(
        #version 420 core

        layout(location = 0) in vec2 in_position;
        layout(location = 1) in vec3 in_normal;
        layout(location = 2) in vec2 in_uv;
        layout(location = 3) in vec3 in_tangent;
        layout(location = 4) in vec3 in_bitangent;

        layout(binding = 0) uniform sampler2D heightMap;

        uniform mat4 model;
        uniform mat4 view;
        uniform mat4 projection;

        uniform float factor = 1.0;

        out vec2 texCoord;

        void main() {
          float height = texture(heightMap, in_position).r * factor;
          gl_Position = projection * view * model * vec4(in_position.x, height, in_position.y, 1.0);
          texCoord = in_position;
        }
    )";

    std::string terrainFragment = R"(
        #version 420 core

        in vec2 texCoord;
        layout(binding = 1) uniform sampler2D groundTexture;
        
        out vec4 fragColor;

        void main() { 
            fragColor = texture(groundTexture, texCoord*500);    
        }
    )";
}

#endif
