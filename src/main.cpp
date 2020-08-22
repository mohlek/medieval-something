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
#include <engine/Texture.h>

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
    std::vector<glm::vec3> normals;
    std::vector<glm::vec2> texture;
    std::vector<unsigned int> faceIndex;
    std::vector<int> textureIndex;
    std::vector<int> normalIndex;

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
            double u, v ;
            l >> u; l >> v;
            texture.push_back(glm::vec2(u, v));
        } else if (prefix == "vn") { // texture coordinates
            std::istringstream l(line.substr(3));
            double x,y,z;
            l >> x; l >> y; l >> z;
            normals.push_back(glm::vec3(x, y, z));
        } else if (prefix == "f ") { // faces
            std::string l = line.substr(2);
            const char* c = l.c_str();
            unsigned int vA, vB, vC, vD, tA, tB, tC, tD, nA, nB, nC, nD;
            int count = sscanf(c, "%i/%i/%i %i/%i/%i %i/%i/%i %i/%i/%i", &vA, &tA, &nA, &vB, &tB, &nB, &vC, &tC, &nC, &vD, &tD, &nD);

            faceIndex.push_back(vA -1);
            faceIndex.push_back(vB -1);
            faceIndex.push_back(vC -1);
            textureIndex.push_back(tA -1);
            textureIndex.push_back(tB -1);
            textureIndex.push_back(tC -1);
            normalIndex.push_back(nA -1);
            normalIndex.push_back(nB -1);
            normalIndex.push_back(nC -1);

            if (count == 12) {
                faceIndex.push_back(vA -1);
                faceIndex.push_back(vC -1);
                faceIndex.push_back(vD -1);
                textureIndex.push_back(tA -1);
                textureIndex.push_back(tC -1);
                textureIndex.push_back(tD -1);
                normalIndex.push_back(nA -1);
                normalIndex.push_back(nC -1);
                normalIndex.push_back(nD -1);
            }
        }
    }

    Engine::Buffer<glm::vec3> vbo(faceIndex.size());
    Engine::Buffer<glm::vec3> norm(faceIndex.size());
    Engine::Buffer<glm::vec2> uvs(faceIndex.size());
    Engine::Buffer<GLuint> elements(faceIndex.size(), GL_ELEMENT_ARRAY_BUFFER);
    for (int i = 0; i < faceIndex.size(); ++i) {
        elements[i] = i;
        vbo[i] = verticies[faceIndex[i]];
        norm[i] = normals[normalIndex[i]];
        uvs[i] = texture[textureIndex[i]];
    }

    Engine::Buffer<glm::vec2> tangent(faceIndex.size());
    Engine::Buffer<glm::vec2> bitangent(faceIndex.size());
    for (int i = 0; i < faceIndex.size(); i+=3) {
        glm::vec3 pos1 = verticies[faceIndex[i]];
        glm::vec3 pos2 = verticies[faceIndex[i+1]];
        glm::vec3 pos3 = verticies[faceIndex[i+2]];
        glm::vec2 uv1 = texture[textureIndex[i]];
        glm::vec2 uv2 = texture[textureIndex[i+1]];
        glm::vec2 uv3 = texture[textureIndex[i+2]];

        glm::vec3 edge1 = pos2 - pos1;
        glm::vec3 edge2 = pos3 - pos1;
        glm::vec2 deltaUV1 = uv2 - uv1;
        glm::vec2 deltaUV2 = uv3 - uv1;

        float f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

        glm::vec3 t;
        t.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
        t.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
        t.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);

        glm::vec3 b;
        b.x = f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x);
        b.y = f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y);
        b.z = f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z);

        tangent[i] = t;
        bitangent[i] = b;
    }
    elements.flush();
    vbo.flush();
    norm.flush();
    uvs.flush();
    tangent.flush();
    bitangent.flush();

    Engine::VertexArrayObject vao;
    vao.setBuffer(0, vbo);
    vao.setBuffer(0, elements);
    vao.setBuffer(1, norm);
    vao.setBuffer(2, uvs);
    vao.bind();

    glActiveTexture(GL_TEXTURE0);
    Engine::Texture textureDiff("Medieval_House_Diff.png");
    textureDiff.bind();

    glActiveTexture(GL_TEXTURE1);
    Engine::Texture textureNormal("Medieval_House_Nor.png");
    textureNormal.bind();

    Engine::ShaderProgram program;
    Engine::Shader vertexShader(GL_VERTEX_SHADER, Shaders::mainVert);
    Engine::Shader fragmentShader(GL_FRAGMENT_SHADER, Shaders::mainFrag);
    program.shaders.push_back(vertexShader);
    program.shaders.push_back(fragmentShader);
    program.link();

    program.use();

    glm::mat4 proj = glm::perspective(glm::radians(45.0f), (float)win->getWidth() / (float)win->getHeight(), 0.1f, 1000.0f);
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::scale(model, glm::vec3(0.1f));
    glm::mat4 view = glm::mat4(1.0f);
    view = glm::translate(view, glm::vec3(0.0f, 0.0f, -100.0f))

    GLint uniformModel = program.getUniformLocation("model");
    GLint uniformView = program.getUniformLocation("view");
    GLint uniformProj = program.getUniformLocation("projection");

    glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(uniformProj, 1, GL_FALSE, glm::value_ptr(proj));
    
    //glUniform1i(program.getUniformLocation("texture1"), 0);
    //glUniform1i(program.getUniformLocation("normalMap"), 1);

    Clock clock;

    do {
        clock.tick();
        float angle = clock.delta / 5000.0f;
        model = glm::rotate(model, angle, glm::vec3(0.0f, 1.0f, 0.0f));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        glDrawElements(GL_TRIANGLES, faceIndex.size(), GL_UNSIGNED_INT, 0);
    } while (win->loop());

    return 0;
}
