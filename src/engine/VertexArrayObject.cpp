#include "VertexArrayObject.h"

using namespace Engine;

VertexArrayObject::VertexArrayObject() {
    if (glCreateVertexArrays) {
        glCreateVertexArrays(1, &this->vaoId);
        return;
    }
    glGenVertexArrays(1, &this->vaoId);
}

VertexArrayObject::~VertexArrayObject() {
    this->buffers.clear();
    glDeleteVertexArrays(1, &this->vaoId); 
}

void VertexArrayObject::addBuffer(std::shared_ptr<Buffer>& buffer) {
    const int index = this->buffers.size();

    if (glEnableVertexArrayAttrib) {
        glEnableVertexArrayAttrib(this->vaoId, index);
    } else {
        bind();
        glEnableVertexAttribArray(index);
    }

    buffer->bind();
    glVertexAttribPointer(index, 3, buffer->dataType, GL_FALSE, buffer->stride, 0);
    this->buffers.push_back(buffer);
}

void VertexArrayObject::bind() {
    glBindVertexArray(this->vaoId);
}
