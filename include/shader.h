#ifndef H_SHADERS
#define H_SHADERS

#include <string>

namespace Shader {
    const std::string mainVert = 
    #include "shader/main.vs"
    ;

    std::string mainFrag =
    #include "shader/main.fs"
    ;
    
    std::string normalDisplayVert =
    #include "shader/displayNormal.vs"
    ;

    std::string normalDisplayGeo =
    #include "shader/displayNormal.gs"
    ;

    std::string normalDisplayFrag =
    #include "shader/displayNormal.fs"
    ;

    std::string terrainVertex =
    #include "shader/terrain.vs"
    ;

    std::string terrainFragment =
    #include "shader/terrain.fs"
    ;
}

#endif
