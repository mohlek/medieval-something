#include "ShaderLoader.h"

#include <iostream>
#include <fstream>

using namespace Engine;

ShaderLoader::ShaderLoader(GLenum shaderType, std::string&& path) : shaderType(shaderType) {

    std::ifstream fileStream(path.c_str(), std::ios::in);
    
    if (!fileStream.is_open()) {
        std::cerr << "Could not read shader " << path << std::endl;
        return;
    }

    std::string line;
    while (!fileStream.eof()) {
        std::getline(fileStream, line);
        this->source.append(line + "\n");
    }

    fileStream.close();
}
