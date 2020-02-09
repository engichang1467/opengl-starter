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

struct Face;
struct Vertex;

V3_T<U32> processIndices(std::string string)
{
    U32 dividerIndex1 = string.find("/");
    U32 dividerIndex2 = string.rfind("/");

    return V3_T<U32>(std::stoi(string.substr(0, dividerIndex1)), std::stoi(string.substr(dividerIndex1 + 1, dividerIndex2)), std::stoi(string.substr(dividerIndex2 + 1)));
}

struct Edge
{
    U32 startVertexIndex;
    U32 endVertexIndex;
    U32 faceIndex;
    U32 nextIndex;
    U32 previousIndex;
    U32 symmetricIndex;
};

struct Face
{
    U32 edgeIndex;  // always the counter-clockwise egde
};

struct Vertex
{
    V3  position;
    V3  barycentric;
    V3  normal = V3(1, 0, 0);
    U32 edgeIndex;  // always the counter-clockwise egde
};

struct Mesh : Entity
{
    std::vector<Vertex> vertices;
    std::vector<U32>    indices;
    U32                 vertexArray, vertexBuffer, elementBuffer;

    Mesh(){};

    Mesh(std::string path)
    {
        U32                vIndex = 0, vnIndex = 0;
        std::ifstream      file = std::ifstream(path);
        std::istringstream stream;
        std::string        line;
        std::string        valueString;
        std::string        token;
        F32                value;
        F32                max = 0;
        V3_T<U32>          indexVector;
        if (file.is_open())
        {
            while (getline(file, line))
            {
                stream = std::istringstream(line);
                stream >> token;

                if (token == "v")
                {
                    vertices.push_back(Vertex());

                    stream >> valueString;
                    value = std::stof(valueString);
                    vertices[vIndex].position.x = value;
                    if (max < value) max = value;

                    stream >> valueString;
                    value = std::stof(valueString);
                    vertices[vIndex].position.y = value;
                    if (max < value) max = value;

                    stream >> valueString;
                    value = std::stof(valueString);
                    vertices[vIndex].position.z = value;
                    if (max < value) max = value;

                    vIndex++;
                }
                else if (token == "vn")
                {
                    stream >> valueString;
                    vertices[vnIndex].normal.x = std::stof(valueString);
                    stream >> valueString;
                    vertices[vnIndex].normal.y = std::stof(valueString);
                    stream >> valueString;
                    vertices[vnIndex].normal.z = std::stof(valueString);
                    vnIndex++;
                }
                else if (token == "f")
                {
                    stream >> valueString;
                    indexVector = processIndices(valueString);
                    indices.push_back(indexVector.x - 1);

                    stream >> valueString;
                    indexVector = processIndices(valueString);
                    indices.push_back(indexVector.x - 1);

                    stream >> valueString;
                    indexVector = processIndices(valueString);
                    indices.push_back(indexVector.x - 1);
                }
            }
            file.close();
        }
        else
        {
            std::cout << "file not read." << std::endl;
        }

        for (U32 vertexIndex = 0; vertexIndex < vertices.size(); vertexIndex++)
        {
            vertices[vertexIndex].position = vertices[vertexIndex].position * (1 / max);
        }

        OUT(vIndex)
        OUT(vnIndex)
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

        for (const auto& shape : shapes)
        {
            for (const auto& index : shape.mesh.indices)
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
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));

        // Vertex Positions
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, barycentric));

        // Vertex Normals
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));

        // // Vertex Colors
        // glEnableVertexAttribArray(2);
        // glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, color));
    }
};

struct WingedEdgeMesh : Mesh
{
    std::vector<Face>   faces;
    std::vector<Edge>   edges;
    std::vector<Vertex> orderedVertices;  // for rendering

    WingedEdgeMesh() : Mesh() {}

    WingedEdgeMesh(std::string path) : Mesh(path)
    {
        OUT("start constructor");

        std::unordered_map<std::string, U32> edgeIndexMap;

        for (U32 faceIndex = 0; faceIndex * 3 < indices.size(); faceIndex += 1)  // for each face
        {
            faces.push_back(Face());

            U32 vertexIndex1 = indices[faceIndex * 3 + 0];
            U32 vertexIndex2 = indices[faceIndex * 3 + 1];
            U32 vertexIndex3 = indices[faceIndex * 3 + 2];

            U32 edgeIndex1 = edges.size();
            edges.push_back(Edge());

            U32 edgeIndex2 = edges.size();
            edges.push_back(Edge());

            U32 edgeIndex3 = edges.size();
            edges.push_back(Edge());

            // faces
            faces[faceIndex].edgeIndex = edgeIndex1;

            // vertices
            vertices[vertexIndex1].edgeIndex = edgeIndex1;
            vertices[vertexIndex2].edgeIndex = edgeIndex2;
            vertices[vertexIndex3].edgeIndex = edgeIndex3;

            // edges
            edges[edgeIndex1].startVertexIndex = vertexIndex1;
            edges[edgeIndex1].endVertexIndex = vertexIndex2;
            edges[edgeIndex1].faceIndex = faceIndex;
            edges[edgeIndex1].previousIndex = edgeIndex3;
            edges[edgeIndex1].nextIndex = edgeIndex2;

            edges[edgeIndex2].startVertexIndex = vertexIndex2;
            edges[edgeIndex2].endVertexIndex = vertexIndex3;
            edges[edgeIndex2].faceIndex = faceIndex;
            edges[edgeIndex2].previousIndex = edgeIndex1;
            edges[edgeIndex2].nextIndex = edgeIndex3;

            edges[edgeIndex3].startVertexIndex = vertexIndex3;
            edges[edgeIndex3].endVertexIndex = vertexIndex1;
            edges[edgeIndex3].faceIndex = faceIndex;
            edges[edgeIndex3].previousIndex = edgeIndex2;
            edges[edgeIndex3].nextIndex = edgeIndex1;
        }

        // for (auto face : faces)
        // {
        //     U32 vertexIndex1 = edges[face.edgeIndex].startVertexIndex;
        //     U32 vertexIndex2 = edges[edges[face.edgeIndex].nextIndex].startVertexIndex;
        //     U32 vertexIndex3 = edges[edges[edges[face.edgeIndex].nextIndex].nextIndex].startVertexIndex;
        //     U32 vertexIndex4 = edges[edges[edges[edges[face.edgeIndex].nextIndex].nextIndex].nextIndex].startVertexIndex;
        //     // std::cout << vertices[vertexIndex1].position.string() << " -> " << vertices[vertexIndex2].position.string() << " -> " << vertices[vertexIndex3].position.string() << std::endl;
        //     std::cout << vertices[vertexIndex1].position.string() << " -> " << vertices[vertexIndex4].position.string() << std::endl;
        // }

        OUT("end constructor");
    }

    void load(bool isSmooth = true)
    {
        orderedVertices.clear();
        orderedVertices.resize(faces.size() * 3);
        OUT("start load");
        for (Face face : faces)
        {
            U32 vertexIndex1 = edges[face.edgeIndex].startVertexIndex;
            U32 vertexIndex2 = edges[edges[face.edgeIndex].nextIndex].startVertexIndex;
            U32 vertexIndex3 = edges[edges[edges[face.edgeIndex].nextIndex].nextIndex].startVertexIndex;

            if (!isSmooth)
            {
                V3 normal = normalize(cross(
                    vertices[vertexIndex1].position - vertices[vertexIndex2].position,
                    vertices[vertexIndex1].position - vertices[vertexIndex3].position));

                vertices[vertexIndex1].normal = normal;
                vertices[vertexIndex2].normal = normal;
                vertices[vertexIndex3].normal = normal;
            }

            orderedVertices.push_back(vertices[vertexIndex1]);
            orderedVertices.back().barycentric = V3(1, 0, 0);

            orderedVertices.push_back(vertices[vertexIndex2]);
            orderedVertices.back().barycentric = V3(0, 1, 0);

            orderedVertices.push_back(vertices[vertexIndex3]);
            orderedVertices.back().barycentric = V3(0, 0, 1);
        }

        // vertex array object
        glGenVertexArrays(1, &vertexArray);
        glBindVertexArray(vertexArray);

        // vertex buffer object
        glGenBuffers(1, &vertexBuffer);
        glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
        glBufferData(GL_ARRAY_BUFFER, orderedVertices.size() * sizeof(Vertex), &orderedVertices[0], GL_STATIC_DRAW);

        // vertex positions
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));

        // vertex barycentrics
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, barycentric));

        // vertex normals
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));

        // // Vertex Colors
        // glEnableVertexAttribArray(2);
        // glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, color));
        OUT("end load");
    }

    void draw()
    {
        glBindVertexArray(vertexArray);

        glDrawArrays(GL_TRIANGLES, 0, orderedVertices.size());

        glBindVertexArray(0);
    }
};
