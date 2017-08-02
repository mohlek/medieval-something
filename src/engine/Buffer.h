#include <GL/glew.h>

namespace Engine {

    class Buffer {
        private:
            GLuint bufferId = 0;
        public:
            const GLenum bufferType;
           
            GLint size;
            GLenum dataType;
            GLsizei stride;

            Buffer(GLenum bufferType);
            Buffer() : Buffer(GL_ARRAY_BUFFER) {};
            virtual ~Buffer();

            void* map();
            bool  unmap();
    };
}
