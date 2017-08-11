#include "Camera.h"

using namespace Engine;

Camera::Camera() {
    glGenBuffers(1, &this->cameraBuffer);
}

Camera::~Camera() {
    glBindBuffer(GL_UNIFORM_BUFFER, this->cameraBuffer);
    glDeleteBuffers(1, &this->cameraBuffer);
}

void Camera::setCamera(glm::vec3 cameraPos, glm::vec3 lookAt) {
    if (!this->cameraBuffer) {
        return;
    }

    float realMatrix[] = {cameraPos.x, cameraPos.y, cameraPos.z, 0.0f,
                          lookAt.x, lookAt.y, lookAt.z, 0.0f};
    
    glBindBuffer(GL_UNIFORM_BUFFER, this->cameraBuffer);
    glBufferData(GL_UNIFORM_BUFFER, sizeof(float) * 8, realMatrix, GL_DYNAMIC_DRAW);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}
