#ifndef BUFFER_H
#define BUFFER_H

#include <GL/glew.h>

namespace Engine {

    class Buffer {
        private:
            GLuint bufferId = 0;
        public:
            const GLenum bufferType;
           
            GLint size;
            GLenum dataType = GL_FLOAT;
            GLsizei stride;

            Buffer(GLenum bufferType);
            Buffer() : Buffer(GL_ARRAY_BUFFER) {};
            virtual ~Buffer();

            void bind();
            
            void pushData(void* data, int size);

            void* map(GLenum access);
            bool  unmap();
    };
}

#endif
