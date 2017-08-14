#ifndef SHADERPROGRAM_H
#define SHADERPROGRAM_H

#include <GL/glew.h>

#include <string>
#include <vector>
#include <map>
#include <memory>

#include "Shader.h"
#include "ShaderLoader.h"

namespace Engine {
    
    class ShaderProgram {
        private:
            GLuint programId = 0;
        
        public:
            std::vector<std::shared_ptr<Shader>> shaders;
            std::map<std::string, GLuint> uniforms;

            ShaderProgram();
            virtual ~ShaderProgram();

            GLint getUniformLocation(std::string&& name);

            void link();
            void use();
            void end();
    };
}

#endif /* SHADERPROGRAM_H */
