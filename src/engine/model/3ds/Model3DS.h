#include <lib3ds/file.h>
#include <GL/glew.h>

#include "../../Buffer.h"

#include <string>

namespace Engine {
    class Model3DS {
        private:
            Lib3dsFile* model;

            unsigned int getTotalFaces();
        public:
            Engine::Buffer vertexVBO;
            Engine::Buffer normalsVBO;

            Model3DS(std::string&& filename);
            ~Model3DS();
            void render();
            void createVBO();
    };
}
