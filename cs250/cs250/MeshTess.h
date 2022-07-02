#pragma once
#include <GL/glew.h>
#include <vector>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

struct Vertex
{
	glm::vec3 pos{};
	Vertex() = default;
	Vertex(float x, float y, float z) : pos(x, y, z) {}
};

struct MeshTess
{
	typedef std::vector<Vertex> VertexBufferType;
	typedef std::vector<int>	IndexBufferType;

	VertexBufferType vertexBuffer;
	IndexBufferType	 indexBuffer;

	unsigned int VAO, VBO, EBO = 0;
	int numVertices = 0;
	int numTris = 0;
	int numIndices = 0;
	
public:
	MeshTess CreateIcosahedron();
	void SendVertexData(MeshTess& meshTess);
};