#ifndef MESH_H
#define MESH_H
#include <QOpenGLShaderProgram>
#include <QOpenGLFunctions_3_3_Core>
#include <QVector3D>
#include <string>
#include <vector>
#include <memory>


struct Vertex
{
    QVector3D position;
    QVector3D normal;
    QVector3D tangent;
    QVector3D bitangent;
    QVector2D textureCoordinates;
};


struct Texture
{
    unsigned int id;
    std::string type;
    std::string path;
};


class Mesh
{
    public:
        std::vector<Vertex> vertices;
        std::vector<unsigned int> indices;
        std::vector<Texture> textures;

        Mesh();
        Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures);
        void clear();
        void draw(QOpenGLShaderProgram* program);

    private:
        std::unique_ptr<QOpenGLFunctions_3_3_Core> openGL = std::make_unique<QOpenGLFunctions_3_3_Core>();
        unsigned int VAO, VBO, EBO;
        void setupMesh();
};


#endif // MESH_H
