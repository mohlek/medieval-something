#include <stdio.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Buffer.h"

namespace Engine {
    
    class Object {
        private:
            Buffer buffer;
        public:
            Object();
            virtual ~Object();
    };
};
