#include "SkyBox.h"

using namespace Game;
using namespace Engine;

SkyBox::SkyBox() {
    this->shader.link();
    this->texture = std::make_shared<Texture>(Texture(GL_TEXTURE_CUBE_MAP)); 
    this->texture->loadFromFile("resource/textures/skybox_texture.jpg");
}

SkyBox::~SkyBox() {
}

void SkyBox::render() {
   shader.use(); 
}
