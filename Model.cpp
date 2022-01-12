#include "Model.h"
#include <QVector3D>
#include <QImage>
#include <QDir>


Model::Model()
{
    openGL->initializeOpenGLFunctions();
}


Model::Model(std::string path)
{
    openGL->initializeOpenGLFunctions();
    loadModel(path);
}


void Model::draw(QOpenGLShaderProgram* program)
{
    for(unsigned int i = 0; i < meshes.size(); i++)
    {
        meshes[i].draw(program);
    }
}


void Model::clear()
{
    for (auto& mesh : meshes)
    {
        mesh.clear();
    }
}


void Model::loadModel(std::string path)
{
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);

    if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        qDebug() << "ERROR::ASSIMP:: " << importer.GetErrorString();
        return;
    }
    unsigned int textureID;
    openGL->glGenTextures(1, &textureID);

    // Retrieve the directory path of the filepath and process nodes
    directory = path.substr(0, path.find_last_of('\\'));
    processNode(scene->mRootNode, scene);
}


void Model::processNode(aiNode* node, const aiScene* scene)
{
    for(unsigned int i = 0; i < node->mNumMeshes; i++)
    {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        meshes.push_back(processMesh(mesh, scene));
    }

    for(unsigned int i = 0; i < node->mNumChildren; i++)
    {
        processNode(node->mChildren[i], scene);
    }
}


Mesh Model::processMesh(aiMesh* mesh, const aiScene* scene)
{
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture> textures;

    // Walk through each of the mesh's vertices
    for(unsigned int i = 0; i < mesh->mNumVertices; i++)
    {
        Vertex vertex;
        vertex.position = QVector3D(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);
        vertex.normal = QVector3D(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);
        if (mesh->HasTangentsAndBitangents())
        {
            vertex.tangent = QVector3D(mesh->mTangents[i].x, mesh->mTangents[i].y, mesh->mTangents[i].z);
            vertex.bitangent = QVector3D(mesh->mBitangents[i].x, mesh->mBitangents[i].y, mesh->mBitangents[i].z);
        }

        // texture coordinates
        if(mesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
        {
            // a vertex can contain up to 8 different texture coordinates. We thus make the assumption that we won't
            // use models where a vertex can have multiple texture coordinates so we always take the first set (0).
            vertex.textureCoordinates = QVector2D(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);
        }
        vertices.push_back(vertex);
    }

    // Now wak through each of the mesh's faces and retrieve the corresponding vertex indices.
    for(unsigned int i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace face = mesh->mFaces[i];

        // Retrieve all indices of the face and store them in the indices vector
        for(unsigned int j = 0; j < face.mNumIndices; j++)
        {
            indices.push_back(face.mIndices[j]);
        }
    }

    // Process materials
    aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
    // 1. diffuse maps
    std::vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "diffuse");
    textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

    // 2. specular maps
    std::vector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "specular");
    textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());

    // 3. normal maps
    std::vector<Texture> normalMaps = loadMaterialTextures(material, aiTextureType_HEIGHT, "normal");
    textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());

    // return a mesh object created from the extracted mesh data
    return Mesh(vertices, indices, textures);
}


// checks all material textures of a given type and loads the textures if they're not loaded yet.
// the required info is returned as a Texture struct.
std::vector<Texture> Model::loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName)
{
    std::vector<Texture> textures;
    for(unsigned int i = 0; i < mat->GetTextureCount(type); i++)
    {
        aiString materialName;
        mat->Get(AI_MATKEY_NAME,materialName);

        aiString textureName;
        mat->GetTexture(type, i, &textureName);

        QDir materialPath = QDir::cleanPath(materialName.C_Str());
        materialPath = materialPath.path().left(materialPath.path().lastIndexOf('/'));
        materialPath = QDir::cleanPath(materialPath.path() + QDir::separator() + textureName.C_Str());
        std::string fullPath = materialPath.path().toLatin1();

        // check if texture was loaded before and if so, continue to next iteration: skip loading a new texture
        bool skip = false;
        for(unsigned int j = 0; j < texturesLoaded.size(); j++)
        {
            if(texturesLoaded[j].path == fullPath)
            {
                textures.push_back(texturesLoaded[j]);
                skip = true; // a texture with the same filepath has already been loaded, continue to next one. (optimization)
                break;
            }
        }

        if(!skip)
        {   // if texture hasn't been loaded already, load it

            Texture texture;
            texture.id = textureFromFile(fullPath, directory);
            texture.type = typeName;
            texture.path = fullPath;
            textures.push_back(texture);
            texturesLoaded.push_back(texture);  // store it as texture loaded for entire model, to ensure we won't unnecesery load duplicate textures.
        }
    }
    return textures;
}


unsigned int Model::textureFromFile(std::string path, std::string directory)
{
    QString relativeFilePath = path.c_str();
    QString directoryPath = directory.c_str();
    QDir filename = QDir::cleanPath(directoryPath + QDir::separator() + relativeFilePath);

    unsigned int textureID;
    openGL->glGenTextures(1, &textureID);

    QImage image(filename.path());
    //unsigned char *data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
    if (!image.isNull())
    {
        image = image.convertToFormat(QImage::Format_RGBA8888);

        openGL->glBindTexture(GL_TEXTURE_2D, textureID);
        openGL->glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image.width(), image.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, (void*)image.bits());
        openGL->glGenerateMipmap(GL_TEXTURE_2D);

        openGL->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        openGL->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        openGL->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        openGL->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    }
    else
    {
        qDebug() << "Texture failed to load at path: " << filename.path();
    }

    return textureID;
}
