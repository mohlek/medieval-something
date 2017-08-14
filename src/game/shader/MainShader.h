#ifndef MAINSHADER_H
#define MAINSHADER_H

#include "../../engine/shader/ShaderProgram.h"

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

#endif /* MAINSHADER_H */
