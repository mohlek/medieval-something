#ifndef MODEL3DS_H
#define MODEL3DS_H

#include <lib3ds/file.h>
#include <GL/glew.h>

#include <string>
#include <memory>

#include "../../Buffer.h"

namespace Engine {
    class Model3DS {
        private:
            Lib3dsFile* model;

            unsigned int getTotalFaces();
        public:
            std::shared_ptr<Buffer> vertexVBO;
            std::shared_ptr<Buffer> normalsVBO;

            Model3DS(std::string&& filename);
            ~Model3DS();
            void render();
            void createVBO();
    };
}

#endif
