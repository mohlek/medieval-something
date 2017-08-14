#include "SkyBoxShader.h"

using namespace Game::Shader;

SkyBoxShader::SkyBoxShader() {
    Engine::ShaderLoader loadskyboxFragment(GL_FRAGMENT_SHADER, "shader/skybox.frag");
    std::shared_ptr<Engine::Shader> skyboxFragment = std::make_shared<Engine::Shader>(Engine::Shader(loadskyboxFragment));
    this->shaders.push_back(skyboxFragment);

    Engine::ShaderLoader loadskyboxVertex(GL_VERTEX_SHADER, "shader/skybox.vert");
    std::shared_ptr<Engine::Shader> skyboxVertex = std::make_shared<Engine::Shader>(Engine::Shader(loadskyboxVertex));
    this->shaders.push_back(skyboxVertex);
}
