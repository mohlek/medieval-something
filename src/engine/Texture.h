#ifndef TEXTURE_H
#define TEXTURE_H

#include <GL/glew.h>

namespace Engine {

    class Texture {
        private:
            GLuint textureId;
            GLenum type;
            const GLsizei size = 1;
        public:
            Texture(GLenum type);
            ~Texture();
    };
}

#endif /* TEXTURE_H */
