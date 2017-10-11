#include "Texture.h"

#include "SOIL.h"

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

void Texture::loadFromBuffer(GLvoid* data) {
    if (!data) return;
    // TODO
}

void Texture::loadFromFile(std::string&& filename) {
    if (this->type == GL_TEXTURE_CUBE_MAP) {
        SOIL_load_OGL_single_cubemap(
            filename.c_str(),
            "EWUDNS",
            SOIL_LOAD_AUTO,
            this->textureId,
            SOIL_FLAG_MIPMAPS
        );
        return;
    }
    SOIL_load_OGL_texture(
            filename.c_str(), 
            SOIL_LOAD_AUTO,
            this->textureId,
            SOIL_FLAG_MIPMAPS
    );
}
