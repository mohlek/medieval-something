#include <stdio.h>
#include <stdlib.h>

#include "engine/Window.h"
#include "engine/Object.h"
#include "engine/shader/ShaderProgram.h"
#include "engine/shader/ShaderLoader.h"

using namespace Engine;

int main(int argc, char** argv) {

    Window* win = new Window();

    win->create();
   
    ShaderProgram p1;
    ShaderLoader sl(GL_FRAGMENT_SHADER, "shader/main.frag");
    Shader s(sl);
    p1.shaders.push_back(std::make_shared<Shader>(s));
    p1.link();
    
    while (win->loop()) {
        glClear(GL_COLOR_BUFFER_BIT || GL_DEPTH_BUFFER_BIT);

        p1.use();

        p1.end();
    }
    win->close();

    return 0;
}
