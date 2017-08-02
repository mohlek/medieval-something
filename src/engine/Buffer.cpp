#include "Buffer.h"

using namespace Engine;

Buffer::Buffer(GLenum bufferType) : bufferType(bufferType) {
    glGenBuffers(1, &this->bufferId);
}

Buffer::~Buffer() {
    glDeleteBuffers(1, &this->bufferId);
}

void* Buffer::map() {
    if (glMapNamedBuffer) {
        return glMapNamedBuffer(this->bufferId, GL_READ_WRITE);
    }

    glBindBuffer(this->bufferType, this->bufferId);
    return glMapBuffer(this->bufferType, GL_READ_WRITE);
}

bool Buffer::unmap() {
    if (glUnmapNamedBuffer) {
        return glUnmapNamedBuffer(this->bufferId);
    }

    glBindBuffer(this->bufferType, this->bufferId);
    return glUnmapBuffer(this->bufferId);
}
