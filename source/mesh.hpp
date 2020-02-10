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
    Vertex() {}
    Vertex(V3 position) : position(position) {}

    V3   position;
    V3   barycentric;
    V3   normal;
    U32  edgeIndex;  // always the counter-clockwise egde
    bool isNew;
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

    std::unordered_map<std::string, U32> edgeIndexMap;

    WingedEdgeMesh() : Mesh() {}

    WingedEdgeMesh(std::string path) : Mesh(path)
    {
        createWingedEdgeMesh();
    }

    void createWingedEdgeMesh()
    {
        faces.clear();
        edges.clear();
        orderedVertices.clear();
        edgeIndexMap.clear();

        for (U32 faceIndex = 0; faceIndex * 3 < indices.size(); faceIndex += 1)  // for each face
        {
            Vertex* vertex1 = &vertices[indices[faceIndex * 3 + 0]];
            Vertex* vertex2 = &vertices[indices[faceIndex * 3 + 1]];
            Vertex* vertex3 = &vertices[indices[faceIndex * 3 + 2]];

            faces.push_back(Face());
            Face* face = &faces.back();

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

    void subdivide()
    {
        std::vector<U32>                     newIndices;
        std::unordered_map<std::string, U32> positionIndexMap;

        U32 evenVertices = vertices.size();

        for (U32 faceIndex = 0; faceIndex * 3 < indices.size(); faceIndex += 1)
        {
            V3          position;
            V3          oppositePosition;
            std::string positionKey;

            U32 vertexIndex1 = indices[faceIndex * 3 + 0];
            vertices[vertexIndex1].isNew = false;
            U32 vertexIndex2 = indices[faceIndex * 3 + 1];
            vertices[vertexIndex2].isNew = false;
            U32 vertexIndex3 = indices[faceIndex * 3 + 2];
            vertices[vertexIndex3].isNew = false;

            // new vertex 1
            U32 newVertexIndex1;
            // oppositePosition = edges[edges[edges[edgeIndexMap[vertices[vertexIndex2].position.string() + vertices[vertexIndex1].position.string()]].nextIndex].nextIndex].startVertex->position;
            // OUT(oppositePosition.string());
            position = vertices[vertexIndex1].position * 1 / 2 + vertices[vertexIndex2].position * 1 / 2;
            positionKey = position.string();
            if (positionIndexMap.count(positionKey) == 0)  // vertex does not exist
            {
                newVertexIndex1 = vertices.size();
                vertices.push_back(Vertex(position));
                vertices.back().isNew = true;
                positionIndexMap[positionKey] = newVertexIndex1;
            }
            else  // vertex exists
            {
                newVertexIndex1 = positionIndexMap[positionKey];
            }

            // new vertex 2
            U32 newVertexIndex2;
            // oppositePosition = edges[edges[edges[edgeIndexMap[vertices[vertexIndex3].position.string() + vertices[vertexIndex2].position.string()]].nextIndex].nextIndex].startVertex->position;
            // OUT(edges[edges[edges[edgeIndexMap[vertices[vertexIndex3].position.string() + vertices[vertexIndex2].position.string()]].nextIndex].nextIndex].startVertex->position.string());

            OUT(oppositePosition.string());
            position = vertices[vertexIndex2].position * 1 / 2 + vertices[vertexIndex3].position * 1 / 2;
            positionKey = position.string();
            if (positionIndexMap.count(positionKey) == 0)  // vertex does not exist
            {
                newVertexIndex2 = vertices.size();
                vertices.push_back(Vertex(position));
                vertices.back().isNew = true;
                positionIndexMap[positionKey] = newVertexIndex2;
            }
            else  // vertex exists
            {
                newVertexIndex2 = positionIndexMap[positionKey];
            }

            // new vertex 3
            U32 newVertexIndex3;
            // oppositePosition = edges[edges[edges[edgeIndexMap[vertices[vertexIndex1].position.string() + vertices[vertexIndex3].position.string()]].nextIndex].nextIndex].startVertex->position;
            // OUT(oppositePosition.string());
            position = vertices[vertexIndex3].position * 1 / 2 + vertices[vertexIndex1].position * 1 / 2;
            positionKey = position.string();
            if (positionIndexMap.count(positionKey) == 0)  // vertex does not exist
            {
                newVertexIndex3 = vertices.size();
                vertices.push_back(Vertex(position));
                vertices.back().isNew = true;
                positionIndexMap[positionKey] = newVertexIndex3;
            }
            else  // vertex exists
            {
                newVertexIndex3 = positionIndexMap[positionKey];
            }

            // new face 1
            newIndices.push_back(vertexIndex1);
            newIndices.push_back(newVertexIndex1);
            newIndices.push_back(newVertexIndex3);

            // new face 2
            newIndices.push_back(vertexIndex2);
            newIndices.push_back(newVertexIndex2);
            newIndices.push_back(newVertexIndex1);

            // new face 3
            newIndices.push_back(vertexIndex3);
            newIndices.push_back(newVertexIndex3);
            newIndices.push_back(newVertexIndex2);

            // new face 4 (center)
            newIndices.push_back(newVertexIndex1);
            newIndices.push_back(newVertexIndex2);
            newIndices.push_back(newVertexIndex3);
        }

        indices.clear();

        indices = newIndices;

        createWingedEdgeMesh();

        // for all odd vertices (new)
        for (U32 vertexIndex = evenVertices; vertexIndex < vertices.size(); vertexIndex += 1)
        {
        }

        // for all even vertices (old)
        for (U32 vertexIndex = 0; vertexIndex < evenVertices; vertexIndex += 1)
        {
        }
    }

    void load(bool isSmooth = true)
    {
        orderedVertices.clear();
        orderedVertices.resize(faces.size() * 3);
        OUT("start load");

        for (U32 faceIndex = 0; faceIndex * 3 < indices.size(); faceIndex += 1)
        {
            Vertex* vertex;

            // vertex 1
            orderedVertices.push_back(Vertex());
            vertex = &orderedVertices.back();
            vertex->position = vertices[indices[faceIndex * 3 + 0]].position;
            vertex->barycentric = V3(1, 0, 0);
            if (isSmooth)
                vertex->normal = vertices[indices[faceIndex * 3 + 0]].normal;
            else
                vertex->normal = faces[faceIndex].normal;

            // vertex 2
            orderedVertices.push_back(Vertex());
            vertex = &orderedVertices.back();
            vertex->position = vertices[indices[faceIndex * 3 + 1]].position;
            vertex->barycentric = V3(0, 1, 0);
            if (isSmooth)
                vertex->normal = vertices[indices[faceIndex * 3 + 1]].normal;
            else
                vertex->normal = faces[faceIndex].normal;

            // vertex 3
            orderedVertices.push_back(Vertex());
            vertex = &orderedVertices.back();
            vertex->position = vertices[indices[faceIndex * 3 + 2]].position;
            vertex->barycentric = V3(0, 0, 1);
            if (isSmooth)
                vertex->normal = vertices[indices[faceIndex * 3 + 2]].normal;
            else
                vertex->normal = faces[faceIndex].normal;
        }

        OUT("vertices        " + std::to_string(vertices.size()))
        OUT("orderedVertices " + std::to_string(orderedVertices.size()))
        OUT("indices         " + std::to_string(indices.size()))
        OUT("faces           " + std::to_string(faces.size()))

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
