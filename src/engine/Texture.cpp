#include "Texture.h"

using namespace Engine;

Texture::Texture(GLenum type = GL_TEXTURE_2D) : type(type) {
    if (glCreateTextures) {
        glCreateTextures(type, this->size, &this->textureId);
    } else {
        glGenTextures(this->size, &this->textureId);
        glBindTexture(type, this->textureId);
    }
}

Texture::~Texture() {
    glDeleteTextures(this->size, &this->textureId);
}
