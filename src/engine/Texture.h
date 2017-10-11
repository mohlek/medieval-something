#ifndef TEXTURE_H
#define TEXTURE_H

#include <GL/glew.h>

#include <string>
#include <memory>

#include "Buffer.h"

namespace Engine {

    class Texture {
        private:
            GLuint textureId;
            GLenum type;
            const GLsizei size = 1;

        public:
            Texture(GLenum type);
            ~Texture();

            void loadFromBuffer(GLvoid* data);
            void loadFromFile(std::string&& filename);
    };
}

#endif /* TEXTURE_H */
