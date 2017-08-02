#include "Shader.h"

using namespace Engine;

Shader::~Shader() {
    glDeleteShader(this->shaderId);
}

void Shader::compile() {
    if (shaderId) {
        return;
    }

    this->shaderId = glCreateShader(this->shaderType);
    const char* s = this->source.c_str();
    glShaderSource(this->shaderId, 1, &s, 0);
    glCompileShader(this->shaderId);
}
