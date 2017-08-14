#ifndef SKYBOX_H
#define SKYBOX_H

#include <GL/glew.h>

#include <memory>

#include "../engine/Texture.h"
#include "shader/SkyBoxShader.h"

namespace Game {
    
    class SkyBox {
        private:
            std::shared_ptr<Engine::Texture> texture;

            Shader::SkyBoxShader shader;

        public:
            SkyBox();
            ~SkyBox();

            void render();
    };
}

#endif /* SKYBOX_H */
