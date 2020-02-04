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
#include <unordered_map>
#include <vector>

struct Vertex
{
    V3  position;
    V3  barycentric;
    V3  normal;
    U32 edgeIndex;

    bool operator<(Vertex vertex)
    {
        if (position.x < vertex.position.x) return true;
        if (position.x > vertex.position.x) return false;
        if (position.y < vertex.position.y) return true;
        if (position.y > vertex.position.y) return false;
        if (position.z < vertex.position.z) return true;
        return false;
    }
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

    Mesh(std::string path)
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
                    stream >> value;
                    indices.push_back(std::stoi(value.substr(0, value.find("/"))) - 1);
                    stream >> value;
                    indices.push_back(std::stoi(value.substr(0, value.find("/"))) - 1);
                    stream >> value;
                    indices.push_back(std::stoi(value.substr(0, value.find("/"))) - 1);
                }
            }
            file.close();
        }
        else
        {
            std::cout << "file not read." << std::endl;
        }
    }

    Mesh(std::string path, bool b)
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

                // vertex.color = V3(0.5f, 0.5f, 0.5f);

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

    void draw()
    {
        glBindVertexArray(vertexArray);

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
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, barycentric));

        // Vertex Normals
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, normal));

        // // Vertex Colors
        // glEnableVertexAttribArray(2);
        // glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, color));
    }
};

struct Edge
{
    U32 origVertexIndex;
    U32 destVertexIndex;
    U32 leftFaceIndex;
    U32 rightFaceIndex;
    U32 cwLeftEdgeIndex;
    U32 ccwLeftEdgeIndex;
    U32 cwRightEdgeIndex;
    U32 ccwRightEdgeIndex;
};

struct Face
{
    U32 edgeIndex;
};

struct WingedEdgeMesh : Mesh
{
    std::vector<Face>   faces;
    std::vector<Edge>   edges;
    std::vector<Vertex> renderVertices;

    WingedEdgeMesh(std::string path) : Mesh(path)
    {
        std::unordered_map<std::string, U32> edgeMap;

        for (U32 faceIndex = 0; faceIndex * 3 < indices.size(); faceIndex += 1)  // for each face
        {
            Face face = Face();

            U32 ccwLastEdgeIndex, cwLastEdgeIndex, ccwCurrentEdgeIndex, cwCurrentEdgeIndex;
            for (U32 offset = 0; offset < 4; offset += 1)  // for each edge + 1
            {
                U32 firstVertexIndex = indices[faceIndex * 3 + offset];
                U32 secondVertexIndex = indices[faceIndex * 3 + ((offset + 1) % 3)];

                // counter clock-wise edges
                // ----------------------------------------------------------------
                std::string ccwEdgeKey = vertices[firstVertexIndex].position.string() + vertices[secondVertexIndex].position.string();

                // get edge index
                if (0 == edgeMap.count(ccwEdgeKey))  // if edge does not exist
                {
                    ccwCurrentEdgeIndex = edges.size();
                    edgeMap[ccwEdgeKey] = ccwCurrentEdgeIndex;

                    Edge edge = Edge();
                    edge.origVertexIndex = firstVertexIndex;
                    edge.destVertexIndex = secondVertexIndex;
                    edges.push_back(edge);
                }
                else  // if edge exists
                {
                    ccwCurrentEdgeIndex = edgeMap[ccwEdgeKey];
                }

                // add last edge as incedent
                if (ccwLastEdgeIndex)  // if not first iteration
                {
                    edges[ccwCurrentEdgeIndex].cwLeftEdgeIndex = ccwLastEdgeIndex;
                    edges[ccwLastEdgeIndex].ccwLeftEdgeIndex = ccwCurrentEdgeIndex;
                }

                ccwLastEdgeIndex = ccwCurrentEdgeIndex;

                edges[ccwCurrentEdgeIndex].leftFaceIndex = faceIndex;

                vertices[firstVertexIndex].edgeIndex = ccwCurrentEdgeIndex;

                // clock-wise edges (reverse)
                // ----------------------------------------------------------------
                std::string cwEdgeKey = vertices[secondVertexIndex].position.string() + vertices[firstVertexIndex].position.string();

                // get edge index
                if (0 < edgeMap.count(cwEdgeKey))  // if edge does not exist
                {
                    cwCurrentEdgeIndex = edges.size();
                    edgeMap[cwEdgeKey] = cwCurrentEdgeIndex;

                    Edge edge = Edge();
                    edge.origVertexIndex = secondVertexIndex;
                    edge.destVertexIndex = firstVertexIndex;
                    edges.push_back(edge);
                }
                else  // if edge exists
                {
                    cwCurrentEdgeIndex = edgeMap[cwEdgeKey];
                }

                // add last edge as incedent
                if (cwLastEdgeIndex)  // if not first iteration
                {
                    edges[cwCurrentEdgeIndex].cwRightEdgeIndex = cwLastEdgeIndex;
                    edges[cwLastEdgeIndex].ccwRightEdgeIndex = cwCurrentEdgeIndex;
                }

                cwLastEdgeIndex = cwCurrentEdgeIndex;

                edges[cwCurrentEdgeIndex].rightFaceIndex = faceIndex;

                vertices[secondVertexIndex].edgeIndex = cwCurrentEdgeIndex;
            }

            face.edgeIndex = ccwCurrentEdgeIndex;
            faces.push_back(face);
        }
    }

    void load()
    {
        renderVertices.clear();
        renderVertices.resize(faces.size() * 3);
        for (auto face : faces)
        {
            U32 firstEdgeIndex = face.edgeIndex;
            U32 secondEdgeIndex = edges[firstEdgeIndex].ccwLeftEdgeIndex;
            U32 thirdEdgeIndex = edges[thirdEdgeIndex].ccwLeftEdgeIndex;

            Vertex vertex;

            // first vertex
            vertex = vertices[edges[firstEdgeIndex].origVertexIndex];
            vertex.barycentric = V3(1, 0, 0);
            renderVertices.push_back(vertex);

            // second vertex
            vertex = vertices[edges[secondEdgeIndex].origVertexIndex];
            vertex.barycentric = V3(0, 1, 0);
            renderVertices.push_back(vertex);

            // third vertex
            vertex = vertices[edges[thirdEdgeIndex].origVertexIndex];
            vertex.barycentric = V3(0, 0, 1);
            renderVertices.push_back(vertex);
        }

        // vertex array object
        glGenVertexArrays(1, &vertexArray);
        glBindVertexArray(vertexArray);

        // vertex buffer object
        glGenBuffers(1, &vertexBuffer);
        glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
        glBufferData(GL_ARRAY_BUFFER, renderVertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

        // vertex positions
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, position));

        // vertex bary
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, barycentric));

        // vertex normals
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, normal));

        // // Vertex Colors
        // glEnableVertexAttribArray(2);
        // glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, color));
    }

    void draw()
    {
        glBindVertexArray(vertexArray);

        glDrawArrays(GL_TRIANGLES, 0, renderVertices.size());

        glBindVertexArray(0);
    }
};