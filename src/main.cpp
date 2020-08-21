#include <stdio.h>
#include <fstream>
#include <sstream>
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <engine/Window.h>
#include <engine/Buffer.h>
#include <engine/VertexArrayObject.h>
#include <engine/shader/ShaderLoader.h>
#include <engine/shader/ShaderProgram.h>

#include "shader.h"

#include <GL/gl.h>

struct Clock
{
    uint32_t last_tick_time = 0;
    uint32_t delta = 0;

    void tick()
    {
        uint32_t tick_time = SDL_GetTicks();
        delta = tick_time - last_tick_time;
        last_tick_time = tick_time;
    }
};

int main(int argc, char** argv) {
    Engine::Window* win = new Engine::Window("Medieval something", 800, 600);

    if (!win->create()) {
        return 1;
    }

    std::ifstream in("Medieval_House.obj", std::ios::in);
    if (!in) {
        fprintf(stderr, "Could not open file");
        exit(1);
    }

    std::vector<glm::vec3> verticies;
    std::vector<glm::vec2> texture;
    std::vector<int> faceIndex;
    std::vector<int> textureIndex;

    std::string line;
    while (std::getline(in, line)) {
        std::string prefix = line.substr(0, 2);
        if (prefix == "v ") { // verticies
          std::istringstream l(line.substr(2));
          double x,y,z;
          l >> x; l >> y; l >> z;
          verticies.push_back(glm::vec3(x, y, z));
        } else if (prefix == "vt") { // texture coordinates
            std::istringstream l(line.substr(3));
            int u, v ;
            l >> u; l >> v;
            texture.push_back(glm::vec2(u, v));
        } else if (prefix == "f ") { // faces
            const char* c = line.substr(2).c_str();
            int vA, vB, vC, tA, tB, tC;
            sscanf(c, "%i/%i %i/%i %i/%i", &vA, &tA, &vB, &tB, &vC, &tC);
            faceIndex.push_back(vA -1);
            faceIndex.push_back(vB -1);
            faceIndex.push_back(vC -1);
            textureIndex.push_back(tA -1);
            textureIndex.push_back(tB -1);
            textureIndex.push_back(tC -1);
        }
    }

    Engine::Buffer<glm::vec3> vbo(verticies.size());
    for (int i = 0; i < verticies.size(); ++i) {
        vbo[i] = verticies[i];
    }
    vbo.flush();

    Engine::Buffer<GLuint> elements(faceIndex.size(), GL_ELEMENT_ARRAY_BUFFER);
    for (int i = 0; i < faceIndex.size(); ++i) {
        elements[i] = faceIndex[i];
    }
    elements.flush();

    Engine::VertexArrayObject vao;
    vao.setBuffer(0, vbo);
    vao.setBuffer(0, elements);
    vao.bind();

    Engine::ShaderProgram program;
    Engine::Shader vertexShader(GL_VERTEX_SHADER, Shaders::mainVert);
    Engine::Shader fragmentShader(GL_FRAGMENT_SHADER, Shaders::mainFrag);
    program.shaders.push_back(vertexShader);
    program.shaders.push_back(fragmentShader);
    program.link();

    program.use();

    glm::mat4 proj = glm::perspective(glm::radians(45.0f), (float)win->getWidth() / (float)win->getHeight(), 0.1f, 100.0f);
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::scale(model, glm::vec3(0.3f));
    glm::mat4 view = glm::mat4(1.0f);
    view = glm::translate(view, glm::vec3(0.0f, 0.0f, -100.0f));

    GLint uniformModel = program.getUniformLocation("model");
    GLint uniformView = program.getUniformLocation("view");
    GLint uniformProj = program.getUniformLocation("projection");

    glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(uniformProj, 1, GL_FALSE, glm::value_ptr(proj));

    Clock clock;

    //elements.bind();
    do {
        clock.tick();
        float angle = clock.delta / 1000.0f;
        model = glm::rotate(model, angle, glm::vec3(1.0f));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        glDrawElements(GL_TRIANGLES, elements._size, GL_UNSIGNED_INT, 0);
    } while (win->loop());

    return 0;
}
