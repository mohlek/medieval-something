#include <stdio.h>
#include <stdlib.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "config.h"

namespace Engine {

    class Window {
        private:
            GLFWwindow* window;
            GLFWmonitor* monitor;
            const GLFWvidmode* mode;

            CONFIG::WINDOW_STATE windowState;

            static void errorCallback(int error, const char* desc);
            static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
            
            static void openGLDebugOutput(unsigned int source, unsigned int type, unsigned int id, unsigned int severity, int length, const char* message, const void* userParam);
        public:
            void create();
            bool loop();
            void close();

            Window();
    };
}
