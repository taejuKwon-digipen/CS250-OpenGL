#include "PerlinNoise.h"
#include "GLShader.h"
#include <cmath>
#include <stb_image.h>

#define M_PI 3.14159265358979323846

GLShader shaderPerlin;

PerlinNoise::PerlinNoise(const char* vert, const char* frag, const unsigned& seed = 2016)
{
    PmVertexShaderPath = vert;
    PmFragShaderpath = frag;

    std::mt19937 generator(seed);
    std::uniform_real_distribution<float> distribution;
    auto dice = std::bind(distribution, generator);
    for (unsigned i = 0; i < tableSize; ++i) {
#if 0 
        // bad
        float gradientLen2;
        do {
            gradients[i] = glm::vec3(2 * dice() - 1, 2 * dice() - 1, 2 * dice() - 1);
            gradientLen2 = gradients[i].length2();
        } while (gradientLen2 > 1);
        gradients[i].normalize();
#else 
        // better
        float theta = acos(2 * dice() - 1);
        float phi = 2 * dice() * M_PI;

        float x = cos(phi) * sin(theta);
        float y = sin(phi) * sin(theta);
        float z = cos(theta);
        gradients[i] = glm::vec3(x, y, z);
#endif 
        permutationTable[i] = i;
    }

    std::uniform_int_distribution<unsigned> distributionInt;
    auto diceInt = std::bind(distributionInt, generator);
    // create permutation table
    for (unsigned i = 0; i < tableSize; ++i)
        std::swap(permutationTable[i], permutationTable[diceInt() & tableSizeMask]);
    // extend the permutation table in the index range [256:512]
    for (unsigned i = 0; i < tableSize; ++i) {
        permutationTable[tableSize + i] = permutationTable[i];
    }
}

PerlinNoise::~PerlinNoise()
{

}

float PerlinNoise::eval(const glm::vec3& p, glm::vec3& derivs) const
{
    int xi0 = ((int)std::floor(p.x)) & tableSizeMask;
    int yi0 = ((int)std::floor(p.y)) & tableSizeMask;
    int zi0 = ((int)std::floor(p.z)) & tableSizeMask;

    int xi1 = (xi0 + 1) & tableSizeMask;
    int yi1 = (yi0 + 1) & tableSizeMask;
    int zi1 = (zi0 + 1) & tableSizeMask;

    float tx = p.x - ((int)std::floor(p.x));
    float ty = p.y - ((int)std::floor(p.y));
    float tz = p.z - ((int)std::floor(p.z));

    float u = quintic(tx);
    float v = quintic(ty);
    float w = quintic(tz);

    // generate vectors going from the grid points to p
    float x0 = tx, x1 = tx - 1;
    float y0 = ty, y1 = ty - 1;
    float z0 = tz, z1 = tz - 1;

    float a = gradientDotV(hash(xi0, yi0, zi0), x0, y0, z0);
    float b = gradientDotV(hash(xi1, yi0, zi0), x1, y0, z0);
    float c = gradientDotV(hash(xi0, yi1, zi0), x0, y1, z0);
    float d = gradientDotV(hash(xi1, yi1, zi0), x1, y1, z0);
    float e = gradientDotV(hash(xi0, yi0, zi1), x0, y0, z1);
    float f = gradientDotV(hash(xi1, yi0, zi1), x1, y0, z1);
    float g = gradientDotV(hash(xi0, yi1, zi1), x0, y1, z1);
    float h = gradientDotV(hash(xi1, yi1, zi1), x1, y1, z1);

    float du = quinticDeriv(tx);
    float dv = quinticDeriv(ty);
    float dw = quinticDeriv(tz);

    float k0 = a;
    float k1 = (b - a);
    float k2 = (c - a);
    float k3 = (e - a);
    float k4 = (a + d - b - c);
    float k5 = (a + f - b - e);
    float k6 = (a + g - c - e);
    float k7 = (b + c + e + h - a - d - f - g);

    derivs.x = du * (k1 + k4 * v + k5 * w + k7 * v * w);
    derivs.y = dv * (k2 + k4 * u + k6 * w + k7 * v * w);
    derivs.z = dw * (k3 + k5 * u + k6 * v + k7 * v * w);

    return k0 + k1 * u + k2 * v + k3 * w + k4 * u * v + k5 * u * w + k6 * v * w + k7 * u * v * w;
}

float PerlinNoise::eval(const glm::vec3& p) const
{
    int xi0 = ((int)std::floor(p.x)) & tableSizeMask;
    int yi0 = ((int)std::floor(p.y)) & tableSizeMask;
    int zi0 = ((int)std::floor(p.z)) & tableSizeMask;

    int xi1 = (xi0 + 1) & tableSizeMask;
    int yi1 = (yi0 + 1) & tableSizeMask;
    int zi1 = (zi0 + 1) & tableSizeMask;

    float tx = p.x - ((int)std::floor(p.x));
    float ty = p.y - ((int)std::floor(p.y));
    float tz = p.z - ((int)std::floor(p.z));

    float u = smoothstep2(tx);
    float v = smoothstep2(ty);
    float w = smoothstep2(tz);


    // gradients at the corner of the cell
    const glm::vec3& c000 = gradients[hash(xi0, yi0, zi0)];
    const glm::vec3& c100 = gradients[hash(xi1, yi0, zi0)];
    const glm::vec3& c010 = gradients[hash(xi0, yi1, zi0)];
    const glm::vec3& c110 = gradients[hash(xi1, yi1, zi0)];

    const glm::vec3& c001 = gradients[hash(xi0, yi0, zi1)];
    const glm::vec3& c101 = gradients[hash(xi1, yi0, zi1)];
    const glm::vec3& c011 = gradients[hash(xi0, yi1, zi1)];
    const glm::vec3& c111 = gradients[hash(xi1, yi1, zi1)];

    // generate vectors going from the grid points to p
    float x0 = tx, x1 = tx - 1;
    float y0 = ty, y1 = ty - 1;
    float z0 = tz, z1 = tz - 1;

    glm::vec3 p000 = glm::vec3(x0, y0, z0);
    glm::vec3 p100 = glm::vec3(x1, y0, z0);
    glm::vec3 p010 = glm::vec3(x0, y1, z0);
    glm::vec3 p110 = glm::vec3(x1, y1, z0);

    glm::vec3 p001 = glm::vec3(x0, y0, z1);
    glm::vec3 p101 = glm::vec3(x1, y0, z1);
    glm::vec3 p011 = glm::vec3(x0, y1, z1);
    glm::vec3 p111 = glm::vec3(x1, y1, z1);

    // linear interpolation
    float a = lerp(dot(c000, p000), dot(c100, p100), u);
    float b = lerp(dot(c010, p010), dot(c110, p110), u);
    float c = lerp(dot(c001, p001), dot(c101, p101), u);
    float d = lerp(dot(c011, p011), dot(c111, p111), u);

    float e = lerp(a, b, v);
    float f = lerp(c, d, v);

    return std::lerp(e, f, w);  //g 
}

uint8_t PerlinNoise::hash(const int& x, const int& y, const int& z) const
{
    return permutationTable[permutationTable[permutationTable[x] + y] + z];
}

float PerlinNoise::gradientDotV(uint8_t perm, float x, float y, float z) const
{
    switch (perm & 15) {
    case  0: return  x + y;  //(1,1,0) 
    case  1: return -x + y;  //(-1,1,0) 
    case  2: return  x - y;  //(1,-1,0) 
    case  3: return -x - y;  //(-1,-1,0) 
    case  4: return  x + z;  //(1,0,1) 
    case  5: return -x + z;  //(-1,0,1) 
    case  6: return  x - z;  //(1,0,-1) 
    case  7: return -x - z;  //(-1,0,-1) 
    case  8: return  y + z;  //(0,1,1), 
    case  9: return -y + z;  //(0,-1,1), 
    case 10: return  y - z;  //(0,1,-1), 
    case 11: return -y - z;  //(0,-1,-1) 
    case 12: return  y + x;  //(1,1,0) 
    case 13: return -x + y;  //(-1,1,0) 
    case 14: return -y + z;  //(0,-1,1) 
    case 15: return -y - z;  //(0,-1,-1) 
    }
}

PolyMesh::PolyMesh() : vertices(nullptr), st(nullptr), normals(nullptr) {

}

PolyMesh::~PolyMesh()
{
    if (vertices) delete[] vertices;
    if (st) delete[] st;
    if (normals) delete[] normals;
}

void PolyMesh::exportToObj()
{
    std::ofstream ofs;
    ofs.open("./polyMesh.obj", std::ios_base::out);

    for (uint32_t i = 0; i < numVertices; ++i) {
        ofs << "v " << vertices[i].x << " " << vertices[i].y << " " << vertices[i].z << std::endl;
    }

    for (uint32_t i = 0; i < numVertices; ++i) {
        ofs << "vt " << st[i].x << " " << st[i].y << std::endl;
    }

    for (uint32_t i = 0; i < numVertices; ++i) {
        ofs << "vn " << normals[i].x << " " << normals[i].y << " " << normals[i].z << std::endl;
    }

    for (uint32_t i = 0, k = 0; i < numFaces; ++i) {
        ofs << "f ";
        for (uint32_t j = 0; j < faceArray[i]; ++j) {
            uint32_t objIndex = verticesArray[k + j] + 1;
            ofs << objIndex << "/" << objIndex << "/" << objIndex << ((j == (faceArray[i] - 1)) ? "" : " ");
        }
        ofs << std::endl;
        k += faceArray[i];
    }

    ofs.close();
}

PolyMesh* createPolyMesh(uint32_t width = 1, uint32_t height = 1, uint32_t subdivisionWidth = 40, uint32_t subdivisionHeight = 40)
{
    PolyMesh* poly = new PolyMesh;
    poly->numVertices = 0;
    poly->vertices = 0;
    poly->normals = 0;
    poly->faceArray = 0;
    poly->verticesArray = 0;
    poly->numVertices = 0;
    poly->numFaces = 0;
	
    poly->numVertices = (subdivisionWidth + 1) * (subdivisionHeight + 1);
    std::cerr << poly->numVertices << std::endl;
    poly->vertices = new glm::vec3[poly->numVertices];
    poly->normals = new glm::vec3[poly->numVertices];
    poly->st = new glm::vec2[poly->numVertices];
    float invSubdivisionWidth = 1.f / subdivisionWidth;
    float invSubdivisionHeight = 1.f / subdivisionHeight;
    for (uint32_t j = 0; j <= subdivisionHeight; ++j) {
        for (uint32_t i = 0; i <= subdivisionWidth; ++i) {
            poly->vertices[j * (subdivisionWidth + 1) + i] = glm::vec3(width * (i * invSubdivisionWidth - 0.5), 0, height * (j * invSubdivisionHeight - 0.5));
            poly->st[j * (subdivisionWidth + 1) + i] = glm::vec2(i * invSubdivisionWidth, j * invSubdivisionHeight);
        }
        std::cerr << std::endl;
    }

    poly->numFaces = subdivisionWidth * subdivisionHeight;
    poly->faceArray = new uint32_t[poly->numFaces];
    for (uint32_t i = 0; i < poly->numFaces; ++i)
        poly->faceArray[i] = 4;

    poly->verticesArray = new uint32_t[4 * poly->numFaces];
    for (uint32_t j = 0, k = 0; j < subdivisionHeight; ++j) {
        for (uint32_t i = 0; i < subdivisionWidth; ++i) {
            poly->verticesArray[k] = j * (subdivisionWidth + 1) + i;
            poly->verticesArray[k + 1] = j * (subdivisionWidth + 1) + i + 1;
            poly->verticesArray[k + 2] = (j + 1) * (subdivisionWidth + 1) + i + 1;
            poly->verticesArray[k + 3] = (j + 1) * (subdivisionWidth + 1) + i;
            k += 4;
        }
    }

    return poly;
}

void PerlinNoise::Init(int stacks, int slice)
{
    shaderPerlin.makeProgram(PmVertexShaderPath, PmFragShaderpath);
    shaderPerlin.Link();

    NoiseStacks = stacks;
    NoiseSlices = slice;

    PolyMesh* poly = createPolyMesh(3, 3, 30, 30);
    Vertex v;

    for (int i = 0; i <= NoiseStacks; ++i)
    {
        float row = (float)i / NoiseStacks;
        for (int j = 0; j <= NoiseSlices; ++j)
        {
            float col = (float)j / NoiseSlices;

            v.position = Vec3(col - 0.5f, 0.5f - row, 0.f);
            v.normalVector = Vec3(0.f, 0.f, 1.f);
            v.upVector = Vec2(col, row);
            addVertex(v);
        }
    }
    //stbi_image_free(data)

    // displace and compute analytical normal using noise function partial derivatives
    glm::vec3 derivs;
    for (uint32_t i = 0; i < poly->numVertices; ++i) {
        glm::vec3 p((poly->vertices[i].x /*+ 0.5*/), 0, (poly->vertices[i].z + 0.5));
        p.y = eval(poly->vertices[i], derivs);
#if ANALYTICAL_NORMALS 
        glm::vec3 tangent(1, derivs.x, 0);  //tangent 
        glm::vec3 bitangent(0, derivs.z, 1);  //bitangent 
        // equivalent to bitangent.cross(tangent)
       /* poly->normals[i] = glm::vec3(-derivs.x, 1, -derivs.z);
        normalize(poly->normals[i]);*/
        Vertex v = Vertex{ p,bitangent ,glm::vec3(0,0,0) };
        addVertex(v);
#endif 
    }

#if !ANALYTICAL_NORMALS 
    // compute face normal if you want
    for (uint32_t k = 0, off = 0; k < poly->numFaces; ++k) {
        uint32_t nverts = poly->faceArray[k];
        const glm::vec3& va = poly->vertices[poly->verticesArray[off]];
        const glm::vec3& vb = poly->vertices[poly->verticesArray[off + 1]];
        const glm::vec3& vc = poly->vertices[poly->verticesArray[off + nverts - 1]];

        glm::vec3 tangent = vb - va;
        glm::vec3 bitangent = vc - va;

        glm::vec3 cvalue = glm::cross(bitangent, tangent);

        poly->normals[poly->verticesArray[off]] = glm::normalize(cvalue);

        off += nverts;
    }
#endif 
    int width{}, height{}, nrChannels{};
    unsigned char* data = stbi_load("shaders/noise.ppm", &width, &height, &nrChannels, 0);
    imageWidth = width;
    imageHeight = height;
    noiseMap = new float[static_cast<float>(imageWidth) * static_cast<float>(imageHeight)];

    std::ofstream ofs;
    ofs.open("shaders/noise.ppm", std::ios::out | std::ios::binary);
    ofs << "P6\n" << width << " " << height << "\n255\n";
    for (unsigned k = 0; k < width * height; ++k) {
        unsigned char n = static_cast<unsigned char>(noiseMap[k] * 255);
        ofs << n << n << n;
    }
    ofs.close();
   // makePlane(NoiseStacks, NoiseSlices);

    float frequency = 0.01f;
    //output noise map to PPM
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    // 텍스처 wrapping/filtering 옵션 설정(현재 바인딩된 텍스처 객체에 대해)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // 텍스처 로드 및 생성

    poly->exportToObj();
    delete poly;

    // output noise map to PPM
    float* noiseMap = new float[width * height];

    for (uint32_t j = 0; j < height; ++j) {
        for (uint32_t i = 0; i < width; ++i) {
            noiseMap[j * width + i] = (eval(glm::vec3(i, 0, j) * static_cast<float>(1 / 64.), derivs) + 1) * 0.5;
        }
    }

    if (data)
    {
        for (int j = 0; j < height; j++)
        {
            for (int i = 0; i < width; i++)
            {
                glm::vec2 foreval = glm::vec2(i, j) * frequency;
                float g = eval(glm::vec3(foreval, 0)) * 10;
                noiseMap[j * width + i] = g - (int)g;
            }
        }
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, noiseMap);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    BuildIndexBuffer(NoiseStacks, NoiseSlices);
    initializing();
}


void PerlinNoise::Draw()
{
    shaderPerlin.Use();
    glBindTexture(GL_TEXTURE_2D, texture);
    glBindVertexArray(PerlinNoiseVAO);
    glDrawElements(GL_TRIANGLES, PnumIndices, GL_UNSIGNED_INT, 0);
    shaderPerlin.Unuse();
}

void PerlinNoise::initializing()
{
    glGenVertexArrays(1, &PerlinNoiseVAO);
    glGenBuffers(1, &PerlinNoiseVBO);
    glGenBuffers(1, &PerlinNoiseEBO);

    glBindVertexArray(PerlinNoiseVAO);
    glBindBuffer(GL_ARRAY_BUFFER, PerlinNoiseVBO);

    glBufferData(GL_ARRAY_BUFFER, (PnumVertices * sizeof(Vertex)), &PvertexBuffer[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, PerlinNoiseEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, (PnumIndices * sizeof(unsigned int)), &PindexBuffer[0], GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normalVector));

    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, upVector));

    glBindVertexArray(0);
}

void PerlinNoise::addIndex(int index)
{
    PindexBuffer.push_back(index);
    ++PnumIndices;

    if (PnumIndices % 3 == 0)
    {
        ++PnumTris;
    }
}

void PerlinNoise::addVertex(Vertex& v)
{
    PvertexBuffer.push_back(v);
    ++PnumVertices;
}

void PerlinNoise::BuildIndexBuffer(int stacks, int slices)
{
    int p0{ 0 }, p1{ 0 }, p2{ 0 }, p3{ 0 }, p4{ 0 }, p5{ 0 };
    int stride = slices + 1;

    for (int i = 0; i < stacks; ++i)
    {
        int currNow = i * stride;
        for (int j = 0; j < slices; ++j)
        {
            p0 = static_cast<int>(currNow + j);
            p1 = p0 + 1;
            p2 = p1 + stride;

            if (!DegenerateTri(PvertexBuffer[p0].position, PvertexBuffer[p1].position, PvertexBuffer[p2].position))
            {
                addIndex(p0);
                addIndex(p1);
                addIndex(p2);
            }

            p3 = p2;
            p4 = p3 - 1;
            p5 = p0;

            if (!DegenerateTri(PvertexBuffer[p3].position, PvertexBuffer[p4].position, PvertexBuffer[p5].position))
            {
                addIndex(p3);
                addIndex(p4);
                addIndex(p5);
            }
        }
    }
}
