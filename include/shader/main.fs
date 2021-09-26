R"(
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