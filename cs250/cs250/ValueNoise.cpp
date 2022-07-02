#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#include "ValueNoise.h"
#include "GLShader.h"
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#define M_PI 3.14159265358979323846

GLShader shaderValue;

ValueNoise::ValueNoise(const char* vert, const char* frag, unsigned seed)
{
    mVertexShaderPath = vert;
    mFragShaderpath = frag;

    std::mt19937 gen(seed);
    std::uniform_real_distribution<float> distrFloat;
    auto randFloat = std::bind(distrFloat, gen);

    // create an array of random values and initialize permutation table
    for (unsigned k = 0; k < kMaxTableSize; ++k) {
        r[k] = randFloat();
        permutationTable[k] = k;
    }

    // shuffle values of the permutation table
    std::uniform_int_distribution<unsigned> distrUInt;
    auto randUInt = std::bind(distrUInt, gen);
    for (unsigned k = 0; k < kMaxTableSize; ++k) {
        unsigned i = randUInt() & kMaxTableSizeMask;
        std::swap(permutationTable[k], permutationTable[i]);
        permutationTable[k + kMaxTableSize] = permutationTable[k];
    }
}

void ValueNoise::Init(int planeStack, int planeSlice)
{
    shaderValue.makeProgram(mVertexShaderPath, mFragShaderpath);
    shaderValue.Link();

    NoiseStacks = planeStack;
    NoiseSlices = planeSlice;

    Vertex v;

    for (int i = 0; i <= planeStack; ++i)
    {
        float row = (float)i / planeStack;
        for (int j = 0; j <= planeSlice; ++j)
        {
            float col = (float)j / planeSlice;

            v.position = Vec3(col - 0.5f, 0.5f - row, 0.f);
            v.normalVector = Vec3(0.f, 0.f, 1.f);
            v.upVector = Vec2(col, row);
            addVertex(v);
        }
    }

#if 0 
    unsigned seed = 2016;
    std::mt19937 gen(seed);
    std::uniform_real_distribution<float> distr;
    auto dice = std::bind(distr, gen);  //std::function<float()> 

    for (unsigned j = 0; j < imageHeight; ++j) {
        for (unsigned i = 0; i < imageWidth; ++i) {
            // generate a float in the range [0:1]
            noiseMap[j * imageWidth + i] = dice();
        }
    }
#elif 0 
    ValueNoise noise;
    float frequency = 0.05f;
    for (unsigned j = 0; j < imageHeight; ++j) {
        for (unsigned i = 0; i < imageWidth; ++i) {
            // generate a float in the range [0:1]
            noiseMap[j * imageWidth + i] = noise.eval(Vec2f(i, j) * frequency);
        }
    }
#elif 0 
    ValueNoise noise;
    float frequency = 0.02f;
    float frequencyMult = 1.8;
    float amplitudeMult = 0.35;
    unsigned numLayers = 5;
    float maxNoiseVal = 0;
    for (unsigned j = 0; j < imageHeight; ++j) {
        for (unsigned i = 0; i < imageWidth; ++i) {
            Vec2f pNoise = Vec2f(i, j) * frequency;
            float amplitude = 1;
            for (unsigned l = 0; l < numLayers; ++l) {
                noiseMap[j * imageWidth + i] += noise.eval(pNoise) * amplitude;
                pNoise *= frequencyMult;
                amplitude *= amplitudeMult;
            }
            if (noiseMap[j * imageWidth + i] > maxNoiseVal) maxNoiseVal = noiseMap[j * imageWidth + i];
        }
    }
    for (unsigned i = 0; i < imageWidth * imageHeight; ++i) noiseMap[i] /= maxNoiseVal;
#elif 0
    ValueNoise noise;
    float frequency = 0.02f;
    float frequencyMult = 1.8;
    float amplitudeMult = 0.35;
    unsigned numLayers = 5;
    float maxNoiseVal = 0;
    for (unsigned j = 0; j < imageHeight; ++j) {
        for (unsigned i = 0; i < imageWidth; ++i) {
            Vec2f pNoise = Vec2f(i, j) * frequency;
            float amplitude = 1;
            for (unsigned l = 0; l < numLayers; ++l) {
                noiseMap[j * imageWidth + i] += std::fabs(2 * noise.eval(pNoise) - 1) * amplitude;
                pNoise *= frequencyMult;
                amplitude *= amplitudeMult;
            }
            if (noiseMap[j * imageWidth + i] > maxNoiseVal) maxNoiseVal = noiseMap[j * imageWidth + i];
        }
    }
    for (unsigned i = 0; i < imageWidth * imageHeight; ++i) noiseMap[i] /= maxNoiseVal;
#elif 0 
    ValueNoise noise;
    float frequency = 0.02f;
    float frequencyMult = 1.8;
    float amplitudeMult = 0.35;
    unsigned numLayers = 5;
    for (unsigned j = 0; j < imageHeight; ++j) {
        for (unsigned i = 0; i < imageWidth; ++i) {
            Vec2f pNoise = Vec2f(i, j) * frequency;
            float amplitude = 1;
            float noiseValue = 0;
            // compute some fractal noise
            for (unsigned l = 0; l < numLayers; ++l) {
                noiseValue += noise.eval(pNoise) * amplitude;
                pNoise *= frequencyMult;
                amplitude *= amplitudeMult;
            }
            // we "displace" the value i used in the sin() expression by noiseValue * 100
            noiseMap[j * imageWidth + i] = (sin((i + noiseValue * 100) * 2 * M_PI / 200.f) + 1) / 2.f;
        }
    }
    //#else 1 
        /*for (unsigned j = 0; j < imageHeight; ++j) {
            for (unsigned i = 0; i < imageWidth; ++i) {
                glm::vec2 foreval = glm::vec2(i, j) * frequency;
                float g = eval(foreval) * 10;
                noiseMap[j * imageWidth + i] = g - (int)g;
            }
        }*/
#endif 
    float frequency = 0.01f;
    //output noise map to PPM
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int width{ 512 }, height{ 512 }, nrChannels{ 1 };
    //unsigned char* data = stbi_load("shaders/noise.ppm", &width, &height, &nrChannels, 0);
    imageWidth = width;
    imageHeight = height;
    imageHeight = imageWidth = 512;
    noiseMap = new float[static_cast<float>(imageWidth) * static_cast<float>(imageHeight)];
    //if (data)
    {
        for (int j = 0; j < height; j++)
        {
            for (int i = 0; i < width; i++)
            {
                glm::vec2 foreval = glm::vec2(i, j) * frequency;
                float g = eval(foreval) * 10;
                noiseMap[j * width + i] = g - (int)g;
            }
        }
        glTexImage2D(GL_TEXTURE_2D, 0, GL_R32F, width, height, 0, GL_RED, GL_FLOAT, noiseMap);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    //else
    //{
    //    std::cout << "Failed to load texture" << std::endl;
    //}
    //stbi_image_free(data)

    BuildIndexBuffer(NoiseStacks, NoiseSlices);
    initializing();
}

void ValueNoise::Draw()
{
    shaderValue.Use();
    glBindTexture(GL_TEXTURE_2D, texture);
    glBindVertexArray(VAO);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_INT, 0);
    shaderValue.Unuse();

    /*delete[] noiseMap;*/
}

void ValueNoise::initializing()
{
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(GL_ARRAY_BUFFER, (numVertices * sizeof(Vertex)), &vertexBuffer[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, (numIndices * sizeof(unsigned int)), &indexBuffer[0], GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normalVector));

    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, upVector));

    glBindVertexArray(0);
}

void ValueNoise::addIndex(int index)
{
    indexBuffer.push_back(index);
    ++numIndices;

    if (numIndices % 3 == 0)
    {
        ++numTris;
    }
}

void ValueNoise::addVertex(Vertex& v)
{
    vertexBuffer.push_back(v);
    ++numVertices;
}

void ValueNoise::BuildIndexBuffer(int stacks, int slices)
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

            if (!DegenerateTri(vertexBuffer[p0].position, vertexBuffer[p1].position, vertexBuffer[p2].position))
            {
                addIndex(p0);
                addIndex(p1);
                addIndex(p2);
            }

            p3 = p2;
            p4 = p3 - 1;
            p5 = p0;

            if (!DegenerateTri(vertexBuffer[p3].position, vertexBuffer[p4].position, vertexBuffer[p5].position))
            {
                addIndex(p3);
                addIndex(p4);
                addIndex(p5);
            }
        }
    }
}


float ValueNoise::eval(glm::vec2& p) const
{
    int xi = std::floor(p.x);
    int yi = std::floor(p.y);

    float tx = p.x - xi;
    float ty = p.y - yi;

    int rx0 = xi & kMaxTableSizeMask;
    int rx1 = (rx0 + 1) & kMaxTableSizeMask;
    int ry0 = yi & kMaxTableSizeMask;
    int ry1 = (ry0 + 1) & kMaxTableSizeMask;

    // random values at the corners of the cell using permutation table
    const float& c00 = r[permutationTable[permutationTable[rx0] + ry0]];
    const float& c10 = r[permutationTable[permutationTable[rx1] + ry0]];
    const float& c01 = r[permutationTable[permutationTable[rx0] + ry1]];
    const float& c11 = r[permutationTable[permutationTable[rx1] + ry1]];

    // remapping of tx and ty using the Smoothstep function 
    float sx = smoothstep2(tx);
    float sy = smoothstep2(ty);

    // linearly interpolate values along the x axis
    float nx0 = std::lerp(c00, c10, sx);
    float nx1 = std::lerp(c01, c11, sx);
    //
    //// linearly interpolate the nx0/nx1 along they y axis
    return std::lerp(nx0, nx1, sy);
}