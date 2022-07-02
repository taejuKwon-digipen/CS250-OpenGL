#pragma once
#include <cmath> 
#include <cstdio> 
#include <random> 
#include <functional> 
#include <iostream> 
#include <fstream> 
#include <algorithm> 
#include <glm/glm.hpp>

#include "math.hpp"
#include "noisemath.h"

#define ANALYTICAL_NORMALS 1 

class ValueNoise
{
public:
    struct Vertex
    {
        Vertex() {}
        Vertex(Vec3 pos, Vec3 nrm, Vec2 uv)
            : position(pos), normalVector(nrm), upVector(uv) {}

        Vec3 position;
        Vec3 normalVector;
        Vec2 upVector;
    };
	
    ValueNoise(const char* vert, const char* frag, unsigned seed);
    float eval(glm::vec2& p) const;
    static const unsigned kMaxTableSize = 256;
    static const unsigned kMaxTableSizeMask = kMaxTableSize - 1;
    float r[kMaxTableSize];
    unsigned permutationTable[kMaxTableSize * 2];

    void Init(int stacks, int slice);
    void Draw();

    void initializing();
    void addIndex(int index);
    void addVertex(Vertex& v);
    void BuildIndexBuffer(int stacks, int slices);

private:
    const char* mVertexShaderPath = nullptr;
    const char* mFragShaderpath = nullptr;

    std::vector<Vertex> vertexBuffer;
    std::vector<int> indexBuffer;

    unsigned int VAO, VBO, EBO = 0;
    int Stack, Slice;
    int numVertices = 0;
    int numTris = 0;
    int numIndices = 0;
	
    int NoiseStacks, NoiseSlices;

    unsigned imageWidth;
    unsigned imageHeight;
    float* noiseMap;
    unsigned int texture;
};