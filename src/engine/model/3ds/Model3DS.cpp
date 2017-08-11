#include "Model3DS.h"

#include <lib3ds/mesh.h>
#include <lib3ds/vector.h>

#include <assert.h>
#include <cstring>

using namespace Engine;

Model3DS::Model3DS(std::string&& filename) {
    this->vertexVBO = std::make_shared<Buffer>();
    this->normalsVBO = std::make_shared<Buffer>();
    this->model = lib3ds_file_load(filename.c_str());

    if (!model) {
        throw "Unable to load model";
    }
}

Model3DS::~Model3DS() {
    
}

unsigned int Model3DS::getTotalFaces() {
    assert(model != NULL);

    unsigned int totalFaces = 0;
    Lib3dsMesh* mesh;
    for (mesh = this->model->meshes; mesh != NULL; mesh = mesh->next) {
        totalFaces += mesh->faces;
    }

    return totalFaces;
}

void Model3DS::createVBO() {
    unsigned int totalFaces = getTotalFaces();

    Lib3dsVector* vertices = new Lib3dsVector[totalFaces * 3];
    Lib3dsVector* normals = new Lib3dsVector[totalFaces * 3];
    Lib3dsMesh* mesh;

    unsigned int finishedFaces = 0;

    for (mesh = this->model->meshes; mesh != NULL; mesh = mesh->next) {
        lib3ds_mesh_calculate_normals(mesh, &normals[finishedFaces * 3]);

        for (unsigned int curFace = 0; curFace < mesh->faces; ++curFace) {
            Lib3dsFace* face = &mesh->faceL[curFace];

            for (unsigned int i = 0; i < 3; ++i) {
                std::memcpy(&vertices[finishedFaces * 3 + 1], mesh->pointL[face->points[i]].pos, sizeof(Lib3dsVector));
            }
            finishedFaces++;
        }
    }

    unsigned int size = sizeof(Lib3dsVector) * 3 * totalFaces;

    this->vertexVBO->stride = sizeof(Lib3dsVector) * 3;
    this->vertexVBO->pushData(vertices, size);

    this->normalsVBO->stride = sizeof(Lib3dsVector) * 3;
    this->normalsVBO->pushData(normals, size);

    delete vertices;
    delete normals;

    lib3ds_file_free(this->model);
    this->model = NULL;
}

void Model3DS::render() {
}

