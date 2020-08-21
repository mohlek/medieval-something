#ifndef H_SHADERS
#define H_SHADERS

#include <string>

namespace Shaders {
    std::string mainVert = R"(
        #version 410 core

        layout(location = 0) in vec3 position;

        uniform mat4 model;
        uniform mat4 view;
        uniform mat4 projection;

        void main() {
            gl_Position = projection * view * model * vec4(position, 1.0);
            //gl_Position = vec4(position, 1.0);
        }
    )";

    std::string mainFrag = R"(
        #version 410 core

        void main() {
            gl_FragColor = vec4(1.0, 0.0, 0.0, 1.0);
        }
    )";
}

#endif