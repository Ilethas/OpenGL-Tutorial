#include "Mesh.h"
#include <QOpenGLShaderProgram>


Mesh::Mesh()
{
    openGL->initializeOpenGLFunctions();
}


Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures)
    : vertices(vertices)
    , indices(indices)
    , textures(textures)
{
    openGL->initializeOpenGLFunctions();
    setupMesh();
}


void Mesh::clear()
{
    openGL->glDeleteVertexArrays(1, &VAO);
    openGL->glDeleteBuffers(1, &VBO);
    openGL->glDeleteBuffers(1, &EBO);

    for(unsigned int i = 0; i < textures.size(); i++)
    {
        openGL->glDeleteTextures(1, &textures[i].id);
    }
}


void Mesh::draw(QOpenGLShaderProgram* program)
{
    unsigned int diffuseNr = 0;
    unsigned int specularNr = 0;
    unsigned int normalNr = 0;

    for(unsigned int i = 0; i < textures.size(); i++)
    {
        // Activate proper texture unit before binding
        openGL->glActiveTexture(GL_TEXTURE0 + i);

        // Retrieve texture number (the N in diffuse_textureN)
        std::string number;
        std::string name = textures[i].type;
        if(name == "diffuse")
        {
            number = std::to_string(diffuseNr++);
        }
        else if(name == "specular")
        {
            number = std::to_string(specularNr++);
        }
        else if(name == "normal")
        {
            number = std::to_string(normalNr++);
        }

        program->setUniformValue(("material." + name + '[' + number + ']').c_str(), i);
        openGL->glBindTexture(GL_TEXTURE_2D, textures[i].id);
    }
    openGL->glActiveTexture(GL_TEXTURE0);

    if (normalNr > 0)
    {
        program->setUniformValue("usingNormalMaps", true);
    }

    // Draw mesh
    openGL->glBindVertexArray(VAO);
    openGL->glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    openGL->glBindVertexArray(0);
}


void Mesh::setupMesh()
{
    openGL->glGenVertexArrays(1, &VAO);
    openGL->glGenBuffers(1, &VBO);
    openGL->glGenBuffers(1, &EBO);

    openGL->glBindVertexArray(VAO);
    openGL->glBindBuffer(GL_ARRAY_BUFFER, VBO);
    openGL->glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

    openGL->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    openGL->glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

    // Vertex positions
    openGL->glEnableVertexAttribArray(0);
    openGL->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

    // Vertex normals
    openGL->glEnableVertexAttribArray(1);
    openGL->glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));

    // Vertex texture coords
    openGL->glEnableVertexAttribArray(2);
    openGL->glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, textureCoordinates));

    // vertex tangent
    openGL->glEnableVertexAttribArray(3);
    openGL->glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, tangent));

    // vertex bitangent
    openGL->glEnableVertexAttribArray(4);
    openGL->glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, bitangent));

    openGL->glBindVertexArray(0);
}
