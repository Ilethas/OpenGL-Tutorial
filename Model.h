#ifndef MODEL_H
#define MODEL_H
#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLShaderProgram>
#include <vector>
#include <string>
#include "Importer.hpp"
#include "scene.h"
#include "postprocess.h"
#include "Mesh.h"


class Model
{
    public:
        std::vector<Texture> texturesLoaded;
        std::vector<Mesh> meshes;
        std::string directory;

        Model();
        Model(std::string path);
        void draw(QOpenGLShaderProgram* program);
        void clear();

    private:
        std::unique_ptr<QOpenGLFunctions_3_3_Core> openGL = std::make_unique<QOpenGLFunctions_3_3_Core>();

        void loadModel(std::string path);
        void processNode(aiNode* node, const aiScene* scene);
        Mesh processMesh(aiMesh* mesh, const aiScene* scene);
        std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);
        unsigned int textureFromFile(std::string path, std::string directory);
};

#endif // MODEL_H
