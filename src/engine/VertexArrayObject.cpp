#include "VertexArrayObject.h"

using namespace Engine;

VertexArrayObject::VertexArrayObject() {
    glGenVertexArrays(1, &this->vaoId);
}

VertexArrayObject::~VertexArrayObject() {
    this->buffers.clear();
    glDeleteVertexArrays(1, &this->vaoId); 
}

void VertexArrayObject::addBuffer(std::shared_ptr<Buffer>& buffer) {
    const int index = this->buffers.size();
    glEnableVertexAttribArray(index);
    glVertexAttribPointer(index, buffer->size, buffer->dataType, GL_FALSE, buffer->stride, 0);
    this->buffers.push_back(buffer);
}
