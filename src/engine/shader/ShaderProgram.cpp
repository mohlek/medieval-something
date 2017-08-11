#include "ShaderProgram.h"

#include <iostream>

using namespace Engine;

ShaderProgram::ShaderProgram() {
}

ShaderProgram::~ShaderProgram() {
    this->shaders.clear();
    glDeleteProgram(this->programId);
}

void printProgramInfoLog(GLuint obj) {
	if (obj == -1) return;
	int infologLength = 0;
	int charsWritten  = 0;
	char *infoLog;

	glGetProgramiv(obj, GL_INFO_LOG_LENGTH,&infologLength);

	if (infologLength > 0) {
		infoLog = (char *)malloc(infologLength);
		glGetProgramInfoLog(obj, infologLength, &charsWritten, infoLog);
		std::cout << infoLog << std::endl;
		free(infoLog);
	}
}

GLint ShaderProgram::getUniformLocation(std::string&& name) {
    return glGetUniformLocation(this->programId, name.c_str());
}

void ShaderProgram::link() {
    this->programId = glCreateProgram();

    for (auto& s : this->shaders) {
        s->compile();
        glAttachShader(this->programId, s->getShaderId());
    }

    glLinkProgram(this->programId);
    printProgramInfoLog(this->programId);
}

void ShaderProgram::use() {
    glUseProgram(this->programId);
}

void ShaderProgram::end() {
    glUseProgram(0);
}
