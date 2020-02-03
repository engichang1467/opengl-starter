#pragma once

#include "entity.hpp"
#include "math.hpp"
#include "types.hpp"

#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"

#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <math.h>
#include <fstream>
#include <iostream>
#include <random>
#include <sstream>
#include <string>
#include <vector>

struct Vertex
{
    V3 position;
    V3 barycentric;
    V3 normal;
    V3 color;
};

struct Mesh : Entity
{
    std::vector<Vertex> vertices;
    std::vector<U32>    indices;
    U32                 vertexArray, vertexBuffer, elementBuffer;

    Mesh(std::vector<Vertex> vertices, std::vector<U32> indices)
        : vertices(vertices), indices(indices)
    {
        load();
    }

    Mesh(std::string path, bool b)
    {
        U32                i = 0;
        std::ifstream      file = std::ifstream(path);
        std::istringstream stream;
        std::string        line;
        std::string        value;
        std::string        token;
        std::string        lastToken = "";
        if (file.is_open())
        {
            while (getline(file, line))
            {
                i++;
                if (i > 25) break;
                stream = std::istringstream(line);
                stream >> token;
                if (token != lastToken) i = 0;
                lastToken = token;

                if (token == "v")
                {
                    vertices.push_back(Vertex());
                    stream >> value;
                    vertices[i].position.x = std::stof(value);
                    stream >> value;
                    vertices[i].position.y = std::stof(value);
                    stream >> value;
                    vertices[i].position.z = std::stof(value);
                }
                else if (token == "vn")
                {
                    stream >> value;
                    vertices[i].normal.x = std::stof(value);
                    stream >> value;
                    vertices[i].normal.y = std::stof(value);
                    stream >> value;
                    vertices[i].normal.z = std::stof(value);
                }
                else if (token == "f")
                {
                }
            }
            file.close();
        }
        else
        {
            std::cout << "file not read." << std::endl;
        }
    }

    Mesh(std::string path)
    {
        tinyobj::attrib_t                attrib;
        std::vector<tinyobj::shape_t>    shapes;
        std::vector<tinyobj::material_t> materials;
        std::string                      warn, err;

        if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, path.c_str()))
        {
            throw std::runtime_error(warn + err);
        }

        for (const auto &shape : shapes)
        {
            for (const auto &index : shape.mesh.indices)
            {
                Vertex vertex = {};

                vertex.position = V3(
                    attrib.vertices[3 * index.vertex_index + 0],
                    attrib.vertices[3 * index.vertex_index + 1],
                    attrib.vertices[3 * index.vertex_index + 2]);

                vertex.barycentric = V3(
                    index.vertex_index % 3 == 0 ? 1 : 0,
                    index.vertex_index % 3 == 1 ? 1 : 0,
                    index.vertex_index % 3 == 2 ? 1 : 0);

                vertex.normal = V3(
                    attrib.normals[3 * index.normal_index + 0],
                    attrib.normals[3 * index.normal_index + 1],
                    attrib.normals[3 * index.normal_index + 2]);

                vertex.color = V3(0.5f, 0.5f, 0.5f);

                vertices.push_back(vertex);
                indices.push_back(indices.size());
            }
        }
    }

    void flattenNormals()
    {
        for (U32 index = 0; index < indices.size(); index += 3)
        {
            V3 normal = normalize(cross(
                vertices[indices[index]].position - vertices[indices[index + 1]].position,
                vertices[indices[index]].position - vertices[indices[index + 2]].position));

            vertices[indices[index + 0]].normal = normal;
            vertices[indices[index + 1]].normal = normal;
            vertices[indices[index + 2]].normal = normal;
        }
    }

    void draw(bool wireFrame = false)
    {
        glBindVertexArray(vertexArray);

        if (wireFrame)
            glDrawElements(GL_LINES, indices.size(), GL_UNSIGNED_INT, 0);
        else
            glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);

        glBindVertexArray(0);
    }

    void load()
    {
        // Vertex Array Object
        glGenVertexArrays(1, &vertexArray);
        glBindVertexArray(vertexArray);

        // Vertex Buffer Object
        glGenBuffers(1, &vertexBuffer);
        glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

        // Element Buffer Object
        glGenBuffers(1, &elementBuffer);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBuffer);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(U32), &indices[0], GL_STATIC_DRAW);

        // Vertex Positions
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, position));

        // Vertex Positions
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, barycentric));

        // Vertex Normals
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, normal));

        // Vertex Colors
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, color));
    }
};