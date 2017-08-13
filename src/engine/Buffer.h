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
            GLenum valuesPerIndex = 3;

            Buffer(GLenum bufferType);
            Buffer() : Buffer(GL_ARRAY_BUFFER) {};
            virtual ~Buffer();

            void bind();
            void unbind();
            
            void pushData(void* data, int size);

            void* map(GLenum access);
            bool  unmap();
        
            GLuint getId() { return bufferId; }
    
    };
}

#endif
