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

using namespace std;

class PerlinNoise
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
	
    PerlinNoise(const char* vert, const char* frag, const unsigned& seed);
    virtual ~PerlinNoise();
    float eval(const glm::vec3& p, glm::vec3& derivs) const;
    float eval(const glm::vec3& p) const;

    void Init(int stacks, int slice);
    void Draw();

    void initializing();
    void addIndex(int index);
    void addVertex(Vertex& v);
    void BuildIndexBuffer(int stacks, int slices);


private:
    const char* PmVertexShaderPath = nullptr;
    const char* PmFragShaderpath = nullptr;
	
    int NoiseStacks, NoiseSlices;

    /* inline */
    uint8_t hash(const int& x, const int& y, const int& z) const;
    float gradientDotV(
        uint8_t perm,  //a value between 0 and 255 
        float x, float y, float z) const;

    std::vector<Vertex> PvertexBuffer;
    std::vector<int> PindexBuffer;
	
    static const unsigned tableSize = 256;
    static const unsigned tableSizeMask = tableSize - 1;
    glm::vec3 gradients[tableSize];
    unsigned permutationTable[tableSize * 2];

    unsigned int texture;
    unsigned imageWidth = 0;
    unsigned imageHeight = 0;
    float* noiseMap;

    unsigned int PerlinNoiseVAO = 0;
    unsigned int PerlinNoiseVBO = 0;
	unsigned int PerlinNoiseEBO = 0;
    int PStack, PSlice;
    int PnumVertices = 0;
    int PnumTris = 0;
    int PnumIndices = 0;
};


class PolyMesh
{
public:
    PolyMesh();
    ~PolyMesh();
    glm::vec2* st;
    glm::vec3* vertices;
    glm::vec3* normals;
    uint32_t* faceArray = 0;
    uint32_t* verticesArray = 0;
    uint32_t numVertices = 0;
    uint32_t numFaces = 0;
    void exportToObj();
    glm::vec3& normalize();
};

PolyMesh* createPolyMesh(uint32_t width, uint32_t height, uint32_t subdivisionWidth, uint32_t subdivisionHeight);
