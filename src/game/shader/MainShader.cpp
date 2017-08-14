#include "MainShader.h"

#include <memory>
#include "../../engine/shader/Shader.h"
#include "../../engine/shader/ShaderLoader.h"

using namespace Game::Shader;

MainShader::MainShader() {
    Engine::ShaderLoader loadMainFragment(GL_FRAGMENT_SHADER, "shader/main.frag");
    std::shared_ptr<Engine::Shader> mainFragment = std::make_shared<Engine::Shader>(Engine::Shader(loadMainFragment));
    this->shaders.push_back(mainFragment);

    Engine::ShaderLoader loadMainVertex(GL_VERTEX_SHADER, "shader/main.vert");
    std::shared_ptr<Engine::Shader> mainVertex = std::make_shared<Engine::Shader>(Engine::Shader(loadMainVertex));
    this->shaders.push_back(mainVertex);
}
