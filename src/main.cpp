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
#include <engine/Terrain.h>
#include <engine/Camera.h>

#include <engine/PerlinNoise.hpp>

#include "../include/shader.h"

#include <GL/gl.h>

#include <engine/object/Object.h>

#include <cmath>
#include <cfenv>

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
    // feenableexcept(FE_DIVBYZERO | FE_INVALID | FE_OVERFLOW);
    Engine::Window* win = new Engine::Window("Medieval something", 800, 600);

    if (!win->create()) {
        return 1;
    }

    Engine::Camera camera;


    std::ifstream in("Medieval_House2.obj", std::ios::in);
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
    for (int i = 0; i < faceIndex.size(); i+=1) {
        /*
          example:
            i    i1 i2 i3
            0 =>  0  1  2   (i+0, i+1, i+2)
            1 =>  0  1  2   (i-1, i+0, i+1)
            2 =>  0  1  2   (i-2, i-1, i+0)
            3 =>  3  4  5   (i+0, i+1, i+2)
            4 =>  3  4  5   (i-1, i+0, i+1)
            5 =>  3  4  5   (i-2, i-1, i+0)
            ....

          get the coorect index:
        */
        const auto ii = std::array{
          i + 0 - (i % 3), // i1
          i + 1 - (i % 3), // i2
          i + 2 - (i % 3)  // i3
        };

        /*
          example:
            i    i1 i2 i3
            0 =>  0  1  2
            1 =>  1  2  0
            2 =>  2  0  1
            3 =>  3  4  5
            4 =>  4  5  3
            5 =>  5  3  4
            ....

          select the coorect index for iX
        */
        const auto i1 = ii[(i+0)%3];
        const auto i2 = ii[(i+1)%3];
        const auto i3 = ii[(i+2)%3];

        glm::vec3 pos1 = verticies[faceIndex[i1]];
        glm::vec3 pos2 = verticies[faceIndex[i2]];
        glm::vec3 pos3 = verticies[faceIndex[i3]];
        glm::vec2 uv1 = texture[textureIndex[i1]];
        glm::vec2 uv2 = texture[textureIndex[i2]];
        glm::vec2 uv3 = texture[textureIndex[i3]];

        glm::vec3 edge1 = pos2 - pos1;
        glm::vec3 edge2 = pos3 - pos1;
        glm::vec2 deltaUV1 = uv2 - uv1;
        glm::vec2 deltaUV2 = uv3 - uv1;

        constexpr auto epsilon = std::numeric_limits<float>::epsilon();
        float fd = (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);
        float f = 1.0f / (fd + std::copysign(epsilon, fd)); // add epsilon .. fixed division by zero problem..

        glm::vec3 t;
        t.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
        t.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
        t.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);

        glm::vec3 b;
        b.x = f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x);
        b.y = f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y);
        b.z = f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z);

        // todo make sure .. t and b is not lenght == 0 ... 

        tangent[i]   = glm::normalize(t); // normalize t
        bitangent[i] = glm::normalize(b); // normalze b
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
    vao.setBuffer(3, tangent);
    vao.setBuffer(4, bitangent);
    vao.bind();

    Engine::Texture textureDiffHouse("Medieval_House_Diff.png", GL_TEXTURE0);
    Engine::Texture textureNormalHouse("Medieval_House_Nor.png", GL_TEXTURE1);
    textureDiffHouse.bind();
    textureNormalHouse.bind();

    void* testTexture = malloc(250 * 250 * 4);
    float* testTexturePtr = (float*)testTexture;
    siv::PerlinNoise p;
    for (int y = 0; y < 250; ++y) {
        for (int x = 0; x < 250; ++x) {
            *testTexturePtr = p.accumulatedOctaveNoise2D_0_1(x/100.0, y/100.0, 8);
            testTexturePtr++;
        }
    }
    Engine::Texture testText(GL_TEXTURE0);
    testText.pushData(GL_R32F, GL_RED, GL_FLOAT, 250, 250, testTexture);
    testText.bind();

    Engine::Texture grassTexture("grass.jpg", GL_TEXTURE1);
    grassTexture.bind();


    Engine::ShaderProgram program;
    Engine::Shader vertexShader(GL_VERTEX_SHADER, Shader::mainVert);
    Engine::Shader fragmentShader(GL_FRAGMENT_SHADER, Shader::mainFrag);
    program.shaders.push_back(vertexShader);
    program.shaders.push_back(fragmentShader);
    program.link();

    program.use();
    
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)win->getWidth() / (float)win->getHeight(), 0.1f, 10000.0f);
    glm::mat4 modelHouse = glm::mat4(1.0f);
    modelHouse = glm::scale(modelHouse, glm::vec3(0.01));
    modelHouse = glm::translate(modelHouse, glm::vec3(50000, 23450, 80065));

    GLint uniformModelHouse = program.getUniformLocation("model");
    GLint uniformViewHouse = program.getUniformLocation("view");
    GLint uniformProjHouse = program.getUniformLocation("projection");
    GLint uniformTime = program.getUniformLocation("time");

    glUniformMatrix4fv(uniformProjHouse, 1, GL_FALSE, glm::value_ptr(projection));
    glUniformMatrix4fv(uniformModelHouse, 1, GL_FALSE, glm::value_ptr(modelHouse));

    Engine::ShaderProgram displayNormalShader;
    Engine::Shader displayNormalVertex(GL_VERTEX_SHADER, Shader::normalDisplayVert);
    Engine::Shader displayNormalGeometry(GL_GEOMETRY_SHADER, Shader::normalDisplayGeo);
    Engine::Shader displayNormalFragment(GL_FRAGMENT_SHADER, Shader::normalDisplayFrag);
    displayNormalShader.shaders.push_back(displayNormalVertex);
    displayNormalShader.shaders.push_back(displayNormalGeometry);
    displayNormalShader.shaders.push_back(displayNormalFragment);
    displayNormalShader.link();
    displayNormalShader.use();

    GLint uniformModelDisplayNormal = displayNormalShader.getUniformLocation("model");
    GLint uniformViewDisplayNormal = displayNormalShader.getUniformLocation("view");
    GLint uniformProjDisplayNormal = displayNormalShader.getUniformLocation("projection");
    glUniformMatrix4fv(uniformProjDisplayNormal, 1, GL_FALSE, glm::value_ptr(projection));
    glUniformMatrix4fv(uniformModelDisplayNormal, 1, GL_FALSE, glm::value_ptr(modelHouse));

    Engine::ShaderProgram programTerrain;
    Engine::Shader vertexShaderTerrain(GL_VERTEX_SHADER, Shader::terrainVertex);
    Engine::Shader fragmentShaderTerrain(GL_FRAGMENT_SHADER, Shader::terrainFragment);
    programTerrain.shaders.push_back(vertexShaderTerrain);
    programTerrain.shaders.push_back(fragmentShaderTerrain);
    programTerrain.link();

    programTerrain.use();

    glm::mat4 modelTerrain = glm::mat4(1.0f);
    modelTerrain = glm::scale(modelTerrain, glm::vec3(5000.f));
    camera.position.x += 200;
    camera.position.z += 1000;
    camera.position.y += 200;
    glm::mat4 viewTerrain = camera.getView();

    GLint uniformModelTerrain = programTerrain.getUniformLocation("model");
    GLint uniformViewTerrain = programTerrain.getUniformLocation("view");
    GLint uniformProjTerrain = programTerrain.getUniformLocation("projection");

    glUniformMatrix4fv(uniformProjTerrain, 1, GL_FALSE, glm::value_ptr(projection));
    glUniformMatrix4fv(uniformModelTerrain, 1, GL_FALSE, glm::value_ptr(modelTerrain));
    
    double factor = 0.14;
    Engine::Terrain terrain(500, 500, projection, programTerrain);
    terrain.setFactor(factor);
    
    Clock clock;

    bool KEYS[322];
    memset(KEYS, 0, 322);

    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    //SDL_ShowCursor(false);
    SDL_SetRelativeMouseMode(SDL_TRUE);
    float yaw, pitch;
    float lastX = 400, lastY = 300;

    glClearColor(1.0, 1.0, 1.0, 1.0);
    do {
        SDL_Event event;
        while (SDL_PollEvent( & event)) {
            switch (event.type) {
                case SDL_QUIT: return 0;
                case SDL_KEYUP:
                    if (event.key.keysym.sym <= 322)
                        KEYS[event.key.keysym.sym] = false; 
                    break;
                case SDL_KEYDOWN:
                    fprintf(stdout, "%d\n", event.key.keysym.sym);
                    if (event.key.keysym.sym <= 322)
                        KEYS[event.key.keysym.sym] = true; 
                    break;
                default: break;
            }
        }

        int mouseX, mouseY;
        SDL_GetRelativeMouseState(&mouseX, &mouseY);

        const float sensitivity = 0.05f;
        mouseX *= sensitivity;
        mouseY *= sensitivity;

        yaw   += mouseX;
        pitch += -mouseY;  

        if(pitch > 89.0f)
          pitch =  89.0f;
        if(pitch < -89.0f)
          pitch = -89.0f;

        camera.direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        camera.direction.y = sin(glm::radians(pitch));
        camera.direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

        float cameraSpeed = (float)(clock.delta / 10.0f);
        if (KEYS[SDLK_w]) {
            camera.position += camera.direction * cameraSpeed;
        }
        if (KEYS[SDLK_s]) {
            camera.position -= camera.direction * cameraSpeed;
        }
        if (KEYS[SDLK_d]) {
            camera.position += glm::normalize(glm::cross(camera.direction, camera.up)) * cameraSpeed;
        }
        if (KEYS[SDLK_a]) {
            camera.position -= glm::normalize(glm::cross(camera.direction, camera.up)) * cameraSpeed;
        }
        if (KEYS[SDLK_SPACE]) {
            camera.position.y += 1.0f * cameraSpeed;
        }
        if (KEYS[SDLK_x]) {
            camera.position.y -= 1.0f * cameraSpeed;
        }
        if (KEYS[SDLK_h]) {
            factor += 0.005;
            terrain.setFactor(factor);
            fprintf(stdout, "factor %f\n", factor);
        }
        if (KEYS[SDLK_j]) {
            factor -= 0.005;
            terrain.setFactor(factor);
            fprintf(stdout, "factor %f\n", factor);
        }
        if (KEYS[SDLK_q]) {
            win->close();
        }

        clock.tick();
        
        viewTerrain = camera.getView();
    
        programTerrain.use();
        testText.bind();
        grassTexture.bind();
        glUniformMatrix4fv(uniformViewTerrain, 1, GL_FALSE, glm::value_ptr(viewTerrain));
        terrain.render();

        program.use();
        vao.bind();
        textureDiffHouse.bind();
        textureNormalHouse.bind();
        glUniform1ui(uniformTime, SDL_GetTicks());
        glUniformMatrix4fv(uniformViewHouse, 1, GL_FALSE, glm::value_ptr(viewTerrain));
        glDrawElements(GL_TRIANGLES, faceIndex.size(), GL_UNSIGNED_INT, 0);
    } while (win->loop());

    return 0;
}
