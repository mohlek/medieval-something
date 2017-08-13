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
        glVertexArrayVertexBuffer(this->vaoId, index, buffer->getId(), 0, buffer->stride);
        glVertexArrayAttribFormat(this->vaoId, index, buffer->valuesPerIndex, GL_FLOAT, GL_FALSE, 0);
    } else {
        glEnableVertexAttribArray(index);
        buffer->bind();
        bind();
        glVertexAttribPointer(index, buffer->valuesPerIndex, buffer->dataType, GL_FALSE, buffer->stride, 0);
    }

    this->buffers.push_back(buffer);
}

void VertexArrayObject::bind() {
    glBindVertexArray(this->vaoId);
}
