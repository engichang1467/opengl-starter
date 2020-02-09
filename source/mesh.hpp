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
    Vertex* startVertex;
    Vertex* endVertex;
    Face*   face;
    U32     nextIndex;
    U32     previousIndex;
    U32     symmetricIndex;
};

struct Face
{
    U32 edgeIndex;  // always the counter-clockwise egde
    V3  normal;
};

struct Vertex
{
    V3  position;
    V3  barycentric;
    V3  normal = V3(0, 0, 0);
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

        // scale vertices
        for (U32 vertexIndex = 0; vertexIndex < vertices.size(); vertexIndex++)
        {
            vertices[vertexIndex].position = vertices[vertexIndex].position * (1 / max);
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
        std::unordered_map<std::string, U32> edgeIndexMap;

        for (U32 faceIndex = 0; faceIndex * 3 < indices.size(); faceIndex += 1)  // for each face
        {
            faces.push_back(Face());
            Face* face = &faces.back();

            Vertex* vertex1 = &vertices[indices[faceIndex * 3 + 0]];
            Vertex* vertex2 = &vertices[indices[faceIndex * 3 + 1]];
            Vertex* vertex3 = &vertices[indices[faceIndex * 3 + 2]];

            V3 normal = normalize(cross(
                vertex1->position - vertex2->position,
                vertex1->position - vertex3->position));

            U32 edgeIndex1 = edges.size();
            edges.push_back(Edge());

            U32 edgeIndex2 = edges.size();
            edges.push_back(Edge());

            U32 edgeIndex3 = edges.size();
            edges.push_back(Edge());

            // faces
            face->edgeIndex = edgeIndex1;
            face->normal = normal;

            // vertices
            vertex1->edgeIndex = edgeIndex1;
            vertex1->normal = vertex1->normal + normal;

            vertex2->edgeIndex = edgeIndex2;
            vertex2->normal = vertex2->normal + normal;

            vertex3->edgeIndex = edgeIndex3;
            vertex3->normal = vertex3->normal + normal;

            // edges
            edges[edgeIndex1].startVertex = vertex1;
            edges[edgeIndex1].endVertex = vertex2;
            edges[edgeIndex1].face = face;
            edges[edgeIndex1].previousIndex = edgeIndex3;
            edges[edgeIndex1].nextIndex = edgeIndex2;
            edgeIndexMap[vertex1->position.string() + vertex2->position.string()] = edgeIndex1;

            edges[edgeIndex2].startVertex = vertex2;
            edges[edgeIndex2].endVertex = vertex3;
            edges[edgeIndex2].face = face;
            edges[edgeIndex2].previousIndex = edgeIndex1;
            edges[edgeIndex2].nextIndex = edgeIndex3;
            edgeIndexMap[vertex2->position.string() + vertex3->position.string()] = edgeIndex2;

            edges[edgeIndex3].startVertex = vertex3;
            edges[edgeIndex3].endVertex = vertex1;
            edges[edgeIndex3].face = face;
            edges[edgeIndex3].previousIndex = edgeIndex2;
            edges[edgeIndex3].nextIndex = edgeIndex1;
            edgeIndexMap[vertex3->position.string() + vertex1->position.string()] = edgeIndex3;
        }

        for (U32 vertexIndex = 0; vertexIndex < vertices.size(); vertexIndex += 1)  // for each vertex
        {
            vertices[vertexIndex].normal = normalize(vertices[vertexIndex].normal);
        }

        for (U32 edgeIndex = 0; edgeIndex < edges.size(); edgeIndex += 1)  // for each edge
        {
            edges[edgeIndex].symmetricIndex = edgeIndexMap[edges[edgeIndex].endVertex->position.string() + edges[edgeIndex].startVertex->position.string()];
        }
    }

    void load(bool isSmooth = true)
    {
        orderedVertices.clear();
        orderedVertices.resize(faces.size() * 3);
        OUT("start load");
        for (Face face : faces)
        {
            orderedVertices.push_back(*edges[face.edgeIndex].startVertex);
            Vertex* vertex1 = &orderedVertices.back();

            orderedVertices.push_back(*edges[edges[face.edgeIndex].nextIndex].startVertex);
            Vertex* vertex2 = &orderedVertices.back();

            orderedVertices.push_back(*edges[edges[edges[face.edgeIndex].nextIndex].nextIndex].startVertex);
            Vertex* vertex3 = &orderedVertices.back();

            if (!isSmooth)
            {
                vertex1->normal = face.normal;
                vertex2->normal = face.normal;
                vertex3->normal = face.normal;
            }

            vertex1->barycentric = V3(1, 0, 0);
            vertex2->barycentric = V3(0, 1, 0);
            vertex3->barycentric = V3(0, 0, 1);
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

        OUT("end load");
    }

    void draw()
    {
        glBindVertexArray(vertexArray);

        glDrawArrays(GL_TRIANGLES, 0, orderedVertices.size());

        glBindVertexArray(0);
    }
};
