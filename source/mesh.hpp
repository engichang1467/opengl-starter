#pragma once

#include "entity.hpp"
#include "math.hpp"
#include "types.hpp"

#include <glad/glad.h>

#include <math.h>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

struct Face;
struct Vertex;

struct Edge
{
    Vertex* start;
    Vertex* end;
    Face*   face;
    Edge*   next;
    Edge*   previous;
    Edge*   symmetric;
};

struct Face
{
    Edge* edge;  // always the counter-clockwise egde
    V3    normal;
};

struct Vertex
{
    Vertex() {}
    Vertex(V3 position) : position(position) {}

    V3    position;
    V3    barycentric;
    V3    normal;
    Edge* edge;  // always the counter-clockwise egde
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

    static V3_T<U32> processIndices(std::string string)
    {
        U32 dividerIndex1 = string.find("/");
        U32 dividerIndex2 = string.rfind("/");

        return V3_T<U32>(std::stoi(string.substr(0, dividerIndex1)), std::stoi(string.substr(dividerIndex1 + 1, dividerIndex2)), std::stoi(string.substr(dividerIndex2 + 1)));
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
    U32 facesLength;
    U32 edgesLength;
    U32 orderedVerticesLength;

    Face*   faces;
    Edge*   edges;
    Vertex* orderedVertices;

    WingedEdgeMesh() : Mesh() {}

    WingedEdgeMesh(std::string path) : Mesh(path)
    {
        createWingedEdgeMesh();
    }

    void createWingedEdgeMesh()
    {
        facesLength = indices.size() / 3;
        edgesLength = indices.size();

        faces = new Face[facesLength];
        edges = new Edge[edgesLength];

        std::unordered_map<std::string, U32> edgeIndexMap;

        for (U32 faceIndex = 0; faceIndex * 3 < indices.size(); faceIndex += 1)
        {
            // face
            faces[faceIndex] = Face();
            Face* face = &faces[faceIndex];

            // edge
            U32 edgeIndex1 = faceIndex * 3 + 0;
            edges[edgeIndex1] = Edge();
            Edge* edge1 = &edges[edgeIndex1];

            U32 edgeIndex2 = faceIndex * 3 + 1;
            edges[edgeIndex2] = Edge();
            Edge* edge2 = &edges[edgeIndex2];

            U32 edgeIndex3 = faceIndex * 3 + 2;
            edges[edgeIndex3] = Edge();
            Edge* edge3 = &edges[edgeIndex3];

            // vertex
            Vertex* vertex1 = &vertices[indices[faceIndex * 3 + 0]];
            Vertex* vertex2 = &vertices[indices[faceIndex * 3 + 1]];
            Vertex* vertex3 = &vertices[indices[faceIndex * 3 + 2]];

            // face
            face->edge = edge1;
            face->normal = normalize(cross(vertex1->position - vertex2->position, vertex1->position - vertex3->position));

            // edge
            edge1->next = edge2;
            edge1->previous = edge3;
            edge1->start = vertex1;
            edge1->end = vertex2;
            edge1->face = face;
            edgeIndexMap[vertex1->position.string() + vertex2->position.string()] = edgeIndex1;

            edge2->next = edge3;
            edge2->previous = edge1;
            edge2->start = vertex2;
            edge2->end = vertex3;
            edge2->face = face;
            edgeIndexMap[vertex2->position.string() + vertex3->position.string()] = edgeIndex2;

            edge3->next = edge1;
            edge3->previous = edge2;
            edge3->start = vertex3;
            edge3->end = vertex1;
            edge3->face = face;
            edgeIndexMap[vertex3->position.string() + vertex1->position.string()] = edgeIndex3;

            // vertex
            vertex1->edge = edge1;
            vertex2->edge = edge2;
            vertex3->edge = edge3;
        }

        for (U32 edgeIndex = 0; edgeIndex < edgesLength; edgeIndex += 1)
        {
            Edge* edge = &edges[edgeIndex];
            edge->symmetric = &edges[edgeIndexMap[edge->end->position.string() + edge->start->position.string()]];
        }

        for (U32 vertexIndex = 0; vertexIndex < vertices.size(); vertexIndex += 1)
        {
            Vertex* vertex = &vertices[vertexIndex];
            vertex->normal = getMeanNormal(*vertex);
        }
    }

    V3 getMeanNormal(Vertex& vertex)
    {
        V3    normalSum = vertex.edge->face->normal;
        Edge* startingEdge = vertex.edge;
        Edge* currentEdge = startingEdge->symmetric->next;
        while (!(startingEdge->end->position == currentEdge->end->position))
        {
            normalSum = normalSum + currentEdge->face->normal;
            currentEdge = currentEdge->symmetric->next;
        }
        return normalize(normalSum);
    }

    void load(bool isSmooth = true)
    {
        orderedVerticesLength = indices.size();
        orderedVertices = new Vertex[orderedVerticesLength];
        OUT("start load");

        for (U32 faceIndex = 0; faceIndex < facesLength; faceIndex += 1)
        {
            orderedVertices[faceIndex * 3 + 0] = *faces[faceIndex].edge->start;
            orderedVertices[faceIndex * 3 + 1] = *faces[faceIndex].edge->next->start;
            orderedVertices[faceIndex * 3 + 2] = *faces[faceIndex].edge->next->next->start;

            orderedVertices[faceIndex * 3 + 0].barycentric = V3(1, 0, 0);
            orderedVertices[faceIndex * 3 + 1].barycentric = V3(0, 1, 0);
            orderedVertices[faceIndex * 3 + 2].barycentric = V3(0, 0, 1);

            if (!isSmooth)
            {
                orderedVertices[faceIndex * 3 + 0].normal = faces[faceIndex].normal;
                orderedVertices[faceIndex * 3 + 1].normal = faces[faceIndex].normal;
                orderedVertices[faceIndex * 3 + 2].normal = faces[faceIndex].normal;
            }
        }

        // vertex array object
        glGenVertexArrays(1, &vertexArray);
        glBindVertexArray(vertexArray);

        // vertex buffer object
        glGenBuffers(1, &vertexBuffer);
        glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
        glBufferData(GL_ARRAY_BUFFER, orderedVerticesLength * sizeof(Vertex), &orderedVertices[0], GL_STATIC_DRAW);

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

    U32 getDegree(Vertex& vertex)
    {
        U32   degree = 1;
        Edge* startingEdge = vertex.edge;
        Edge* currentEdge = startingEdge->symmetric->next;
        while (!(startingEdge->end->position == currentEdge->end->position))
        {
            degree++;
            currentEdge = currentEdge->symmetric->next;
        }
        return degree;
    }

    V3 sumNeighbours(Vertex& vertex)
    {
        V3    sum = vertex.edge->end->position;
        Edge* startingEdge = vertex.edge;
        Edge* currentEdge = startingEdge->symmetric->next;
        U32   i = 0;
        while (!(startingEdge->end->position == currentEdge->end->position))
        {
            i++;
            sum = sum + currentEdge->end->position;
            currentEdge = currentEdge->symmetric->next;
        }
        return sum;
    }

    void subdivide()
    {
        std::unordered_map<std::string, U32> vertexIndexMap;

        U32  oldVertexCount = vertices.size();
        auto newVertices = std::vector<Vertex>(vertices);
        auto newIndices = std::vector<U32>();

        for (U32 faceIndex = 0; faceIndex * 3 < indices.size(); faceIndex += 1)
        {
            Face* face = &faces[faceIndex];

            Edge* edge1 = face->edge;
            Edge* edge2 = face->edge->next;
            Edge* edge3 = face->edge->next->next;

            Vertex* vertex1 = edge1->start;
            U32     vertexIndex1 = indices[faceIndex * 3 + 0];

            Vertex* vertex2 = edge2->start;
            U32     vertexIndex2 = indices[faceIndex * 3 + 1];

            Vertex* vertex3 = edge3->start;
            U32     vertexIndex3 = indices[faceIndex * 3 + 2];

            V3 oppositePosition;
            V3 position;

            // new vertex 1
            U32 newVertexIndex1;
            oppositePosition = edge1->symmetric->next->end->position;
            position = vertex1->position * 3 / 8 + vertex2->position * 3 / 8 + vertex3->position * 1 / 8 + oppositePosition * 1 / 8;
            if (vertexIndexMap.count(position.string()) == 0)
            {
                newVertexIndex1 = newVertices.size();
                newVertices.push_back(Vertex(position));
                vertexIndexMap[position.string()] = newVertexIndex1;
            }
            else
            {
                newVertexIndex1 = vertexIndexMap[position.string()];
            }

            // new vertex 2
            U32 newVertexIndex2;
            oppositePosition = edge2->symmetric->next->end->position;
            position = vertex2->position * 3 / 8 + vertex3->position * 3 / 8 + vertex1->position * 1 / 8 + oppositePosition * 1 / 8;
            if (vertexIndexMap.count(position.string()) == 0)
            {
                newVertexIndex2 = newVertices.size();
                newVertices.push_back(Vertex(position));
                vertexIndexMap[position.string()] = newVertexIndex2;
            }
            else
            {
                newVertexIndex2 = vertexIndexMap[position.string()];
            }

            // new vertex 3
            U32 newVertexIndex3;
            oppositePosition = edge3->symmetric->next->end->position;
            position = vertex3->position * 3 / 8 + vertex1->position * 3 / 8 + vertex2->position * 1 / 8 + oppositePosition * 1 / 8;
            if (vertexIndexMap.count(position.string()) == 0)
            {
                newVertexIndex3 = newVertices.size();
                newVertices.push_back(Vertex(position));
                vertexIndexMap[position.string()] = newVertexIndex3;
            }
            else
            {
                newVertexIndex3 = vertexIndexMap[position.string()];
            }

            // face 1
            newIndices.push_back(vertexIndex1);
            newIndices.push_back(newVertexIndex1);
            newIndices.push_back(newVertexIndex3);

            // face 2
            newIndices.push_back(vertexIndex2);
            newIndices.push_back(newVertexIndex2);
            newIndices.push_back(newVertexIndex1);

            // face 3
            newIndices.push_back(vertexIndex3);
            newIndices.push_back(newVertexIndex3);
            newIndices.push_back(newVertexIndex2);

            // face 4 (center)
            newIndices.push_back(newVertexIndex1);
            newIndices.push_back(newVertexIndex2);
            newIndices.push_back(newVertexIndex3);
        }

        indices = newIndices;
        vertices = newVertices;

        createWingedEdgeMesh();

        // loop through all even vertices (old ones)
        for (U32 vertexIndex = 0; vertexIndex < oldVertexCount; vertexIndex += 1)
        {
            Vertex* vertex = &vertices[vertexIndex];
            F32     degree = getDegree(*vertex);

            // get weight using loop equation
            F32 x = (3.0f / 8.0f) + (1.0f / 4.0f) * cos((2.0f * PI) / degree);
            F32 weight = (1.0f / degree) * ((5.0f / 8.0f) - (x * x));

            vertex->position = sumNeighbours(*vertex) * weight + vertex->position * (1.0f - weight * degree);
        }
    }

    void draw()
    {
        glBindVertexArray(vertexArray);
        glDrawArrays(GL_TRIANGLES, 0, orderedVerticesLength);
        glBindVertexArray(0);
    }
};
