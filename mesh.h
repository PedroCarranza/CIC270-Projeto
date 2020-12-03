#ifndef MESH_H
#define MESH_H

#include <GL/glew.h> // holds all OpenGL type declarations

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <string>
#include <vector>

#include "lib/shader.h"

struct Vertex
{
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 TexCoords;
};

class Mesh
{
public:
    // mesh Data
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    unsigned int VAO;

    // constructor
    Mesh(const std::string &path)
    {

        parseMesh(path);
        setupMesh();
    }

    // render the mesh
    void Draw(Shader *shader);

private:
    unsigned int VBO, EBO;

    void parseMesh(const std::string &path);
    void setupMesh();
};
#endif