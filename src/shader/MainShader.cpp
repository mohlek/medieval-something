#include "MainShader.h"

#include <memory>
#include "../engine/shader/Shader.h"
#include "../engine/shader/ShaderLoader.h"

using namespace Game::Shader;

MainShader::MainShader() {
    Engine::ShaderLoader loadMainFragment(GL_FRAGMENT_SHADER, "shader/main.frag");
    Engine::Shader mainFragment(loadMainFragment);
    this->shaders.push_back(std::make_shared<Engine::Shader>(mainFragment));

    Engine::ShaderLoader loadMainVertex(GL_VERTEX_SHADER, "shader/main.vert");
    Engine::Shader mainVertex(loadMainVertex);
    this->shaders.push_back(std::make_shared<Engine::Shader>(mainVertex));
}
