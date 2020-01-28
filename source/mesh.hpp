#include <glad.h>

#include <vector>

#include "entity.hpp"
#include "shader.hpp"

struct Vertex
{
    V3 position;
    V3 normal;
    V2 texCoord;
};

struct Texture
{
    U32         id;
    std::string type;
};

struct Mesh
{
    U32                 vertexArray, vertexBuffer, elementBuffer;
    std::vector<Vertex> vertices;
    std::vector<U32>    indices;

    // Mesh(std::string modelPath, std::string texturePath)
    // {
    //     I32      width, height, channels;
    //     stbi_uc* pixels = stbi_load(texturePath.c_str(), &width, &height, &channels, STBI_rgb_alpha);

    //     // glGenVertexArrays(1, &vao);
    //     // glGenBuffers(1, &vbo);
    //     // glGenBuffers(1, &ebo);
    // }

    Mesh(std::vector<Vertex> vertices, std::vector<U32> indices)
    {
        this->vertices = vertices;
        this->indices = indices;

        for (int i = 0; i < this->vertices.size(); i += 1)
        {
            std::cout << "v: " << this->vertices[i].position.x << ", " << this->vertices[i].position.y << ", " << this->vertices[i].position.z << std::endl;
        }

        for (int i = 0; i < this->indices.size(); i += 1)
        {
            std::cout << "i: " << this->indices[i] << std::endl;
        }

        setup();
    }

    void setup()
    {
        glGenVertexArrays(1, &vertexArray);
        glGenBuffers(1, &vertexBuffer);
        glGenBuffers(1, &elementBuffer);

        glBindVertexArray(vertexArray);
        glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);

        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBuffer);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(U32), &indices[0], GL_STATIC_DRAW);

        // vertex positions
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
        // vertex normals
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
        // vertex texture coords
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoord));

        glBindVertexArray(0);
    }

    void draw()
    {
        glBindVertexArray(vertexArray);
        glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }
};