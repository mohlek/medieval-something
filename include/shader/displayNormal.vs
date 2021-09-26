R"(
        #version 420 core
        layout(location = 0) in vec3 in_position;
        layout(location = 1) in vec3 in_normal;
        layout(location = 2) in vec2 in_uv;
        layout(location = 3) in vec3 in_tangent;
        layout(location = 4) in vec3 in_bitangent;
      
        out VS_OUT {
            vec3 normal;
        } vs_out;
        
        uniform mat4 model;
        layout(std140) uniform camera {
            mat4 view;
            mat4 projection;
        };
        
        void main()
        {
            gl_Position = view * model * vec4(in_position, 1.0); 
            mat3 normalMatrix = mat3(transpose(inverse(view * model)));
            vs_out.normal = normalize(vec3(vec4(normalMatrix * in_normal, 0.0)));
        }
    )"