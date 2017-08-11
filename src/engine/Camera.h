#include <GL/glew.h>

#include <glm/glm.hpp>

namespace Engine {
    
    class Camera {
        private:
            GLuint cameraBuffer;
        public:
            Camera();
            ~Camera();

            void setCamera(glm::vec3 cameraPos, glm::vec3 lookAt);
    };
}
