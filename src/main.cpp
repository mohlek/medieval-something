#include <stdio.h>
#include <stdlib.h>

#include <glm/glm.hpp>
#include <memory>

#include "engine/Window.h"
#include "engine/Camera.h"

#include "engine/model/3ds/Model3DS.h"
#include "engine/VertexArrayObject.h"

#include "shader/MainShader.h"

using namespace Engine;

int main(int argc, char** argv) {

    Window* win = new Window();

    win->create();

    Game::Shader::MainShader mainShader;
    
    mainShader.link();

    Model3DS dragon("resource/dragon.3ds");
   
    float points[] = {
         0.0f,  0.5f, 0.0f,
         0.5f, -0.5f, 0.0f,
        -0.5f, -0.5f, 0.0f
    };

    VertexArrayObject vao;
    std::shared_ptr<Buffer> vbo = std::make_shared<Buffer>();
    vbo->pushData(points, 9 * sizeof(float));
    vbo->stride = 3 * sizeof(float);

    vao.addBuffer(vbo);

    while (win->loop()) {
        mainShader.use();

        vao.bind();
        glDrawArrays(GL_TRIANGLES, 0, 3);
    }

    win->close();

    return 0;
}
