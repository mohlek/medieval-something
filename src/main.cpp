#include <stdio.h>

#include <engine/Window.h>

int main(int argc, char** argv) {
    Engine::Window* win = new Engine::Window("Medieval something", 800, 600);

    if (!win->create()) {
        return 1;
    }

    do {

    } while (win->loop());

    return 0;
}
