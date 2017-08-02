#include <stdio.h>
#include <stdlib.h>

#include "engine/Window.h"
#include "engine/Object.h"

void render() {
    static const GLfloat color[] = {.0f, .0f, .5f, 1.0f};
    glClearBufferfv(GL_COLOR, 0, color);
}

int main(int argc, char** argv) {

    Engine::Window* win = new Engine::Window();

    win->create();
    
    Engine::Object* o = new Engine::Object();
    
    while(win->loop());
    win->close();

    return 0;
}
