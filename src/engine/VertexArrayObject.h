#include <GL/glew.h>
#include <vector>
#include <memory>

#include "Buffer.h"

namespace Engine {
    
    class VertexArrayObject {
        private:
            GLuint vaoId = 0;

            std::vector<std::shared_ptr<Buffer>> buffers;
        public:
            VertexArrayObject();
            virtual ~VertexArrayObject();

            void addBuffer(std::shared_ptr<Buffer>& buffer);
    };
}
