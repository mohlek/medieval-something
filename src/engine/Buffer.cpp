#include "Buffer.h"

#include <cstring>

using namespace Engine;

Buffer::Buffer(GLenum bufferType) : bufferType(bufferType) {
    glGenBuffers(1, &this->bufferId);
}

Buffer::~Buffer() {
    glDeleteBuffers(1, &this->bufferId);
}

void Buffer::bind() {
    glBindBuffer(this->bufferType, this->bufferId);
}

void* Buffer::map(GLenum access = GL_READ_WRITE) {
    if (glMapNamedBuffer) {
        if (access != GL_READ_ONLY) {
            glNamedBufferData(this->bufferId, this->size, NULL, GL_STATIC_DRAW);
        } 
        return glMapNamedBuffer(this->bufferId, access);
    }

    bind();
    if (access != GL_READ_ONLY) {
        glBufferData(this->bufferType, this->size, NULL, GL_STATIC_DRAW);
    }
    return glMapBuffer(this->bufferType, access);
}

void Buffer::pushData(void* data, int size) {
    this->size = size;
    void* addr = map(GL_WRITE_ONLY);

    std::memcpy(addr, data, this->size);
}

bool Buffer::unmap() {
    if (glUnmapNamedBuffer) {
        return glUnmapNamedBuffer(this->bufferId);
    }

    bind();
    return glUnmapBuffer(this->bufferId);
}
