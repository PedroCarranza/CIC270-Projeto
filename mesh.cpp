#include "mesh.h"

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

void Mesh::Draw(Shader *shader)
{

    // draw mesh
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, vertices.size());
    glBindVertexArray(0);

    // always good practice to set everything back to defaults once configured.
    glActiveTexture(GL_TEXTURE0);
}

void Mesh::parseMesh(const std::string &path)
{
    std::ifstream me(path);
    std::string line;
    std::vector<glm::vec3> pos, norm;
    std::vector<glm::vec2> tex;
    while (getline(me, line))
    {
        std::string test;
        glm::vec3 v3;
        glm::vec2 v2;

        std::stringstream p(line);
        p >> test;
        if (test == "v")
        {
            p >> v3.x >> v3.y >> v3.z;
            pos.push_back(v3);
        }
        if (test == "vn")
        {
            p >> v3.x >> v3.y >> v3.z;
            norm.push_back(v3);
        }
        if (test == "vt")
        {
            p >> v2.x >> v2.y;
            tex.push_back(v2);
        }
        if (test == "f")
        {
            int po[3];
            int te[3];
            int nm[3];
            char lixo;

            p >> po[0] >> lixo >> te[0] >> lixo >> nm[0];
            p >> po[1] >> lixo >> te[1] >> lixo >> nm[1];
            p >> po[2] >> lixo >> te[2] >> lixo >> nm[2];

            vertices.push_back({pos[po[0]], norm[nm[0]], tex[te[0]]});
            vertices.push_back({pos[po[1]], norm[nm[1]], tex[te[1]]});
            vertices.push_back({pos[po[2]], norm[nm[2]], tex[te[2]]});
        }
    }
}

void Mesh::setupMesh()
{
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

    // vertex positions
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)0);
    // vertex normals
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)(sizeof(float) * 3));
    // vertex texture coords
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)(sizeof(float) * 6));

    if (offsetof(Vertex, TexCoords) != (sizeof(float) * 6))
    {
        std::cout << "Ihh rapaz" << std::endl;
    }

    glBindVertexArray(0);
}