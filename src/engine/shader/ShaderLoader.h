#include <GL/glew.h>
#include <string>

namespace Engine {
    
    class ShaderLoader {
        private:
        public:
            const GLenum shaderType;
            std::string source;

            ShaderLoader(GLenum shaderType, std::string& path);
            ~ShaderLoader();
    };
}
