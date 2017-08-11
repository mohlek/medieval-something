#include "../engine/shader/ShaderProgram.h"

#include <GL/glew.h>

namespace Game {
    namespace Shader {
        class MainShader : public Engine::ShaderProgram {
            
            public:
                GLint positionLocation = getUniformLocation("position");
                
                MainShader();
        };
    }
}
