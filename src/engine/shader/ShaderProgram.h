#include <GL/glew.h>

#include <vector>
#include <memory>

#include "Shader.h"

namespace Engine {
    
    class ShaderProgram {
        private:
            GLuint programId = 0;
        
        public:
            std::vector<std::shared_ptr<Shader>> shaders;
            
            ShaderProgram();
            virtual ~ShaderProgram();

            void link();
            void use();
            void end();
    };
}
