#pragma once
#include <GL/glew.h>
#include <vector>
#include <glm/glm.hpp>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include "math.hpp"

class Mesh
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

	enum class meshSelect
	{
		CUBE = 0,
		CYLINDER,
		SPHERE,
		TORUS,
		PLANE,
		HERMITECURVE,
		CATMULL_ROM,
		ValueNoise,
		PerlinNoise,
	};

	Mesh() {}
	Mesh(const char* vert, const char* frag)
	: mVertexShaderPath(vert), mFragShaderpath(frag) {}
	virtual ~Mesh() {}

	void Init(int stacks, int slices, meshSelect mesh);
	void Init(float count_, meshSelect mesh);
	void Draw();
	void Update();
	void RenderIMGUI();

	void makeCube(int stacks, int slices);
	void makeCylinder(int stacks, int slices);
	void makeSphere(int stacks, int slices);
	void makeTorus(int stacks, int slices);
	void makePlane(int stacks, int slices);
	void makeHermite(float count);
	void makeCatmull(float count);
	
	void initializing();
	void addIndex(int index);
	void addVertex(Vertex& v);
	void BuildIndexBuffer(int stacks, int slices);

protected:
	meshSelect thisMesh;
	int MeshType = GL_LINE;
	bool showShaders = false;
	
	const char* mVertexShaderPath	= nullptr;
	const char* mFragShaderpath		= nullptr;

	std::vector<Vertex> vertexBuffer;
	std::vector<int> indexBuffer;
	
	unsigned int VAO, VBO, EBO = 0;
	int Stack, Slice;
	int numVertices = 0;
	int numTris		= 0;
	int numIndices	= 0;
	
	Mat4 view;
	Mat4 projection;
	Mat4 model;

	Vertex v;

	// CUBE
	std::vector<Vertex> vPlaneBuffer;
	std::vector<int>	iPlaneBuffer;
	int numVert = 0, numIndi = 0, numTri = 0;
	int p0{ 0 }, p1{ 0 }, p2{ 0 }, p3{ 0 }, p4{ 0 }, p5{ 0 };
	int stride;
	Vertex vt;
	// SPHERE
	float radius = 0.5f;
	// TORUS
	float R = 0.3f;
	float r = 0.1f;
	// HERMITE CURVE

	std::vector<Vec2>VertexBuffer;
	std::vector<int>IndexBuffer;
	float count = 0.f;
	Vec2 startPos = Vec2(-500.f, 0.f);
	Vec2 endPos = Vec2(500.f, 0.f);
	Vec2 changeEPos(float xpos, float ypos);
	Vec2 changedpos = Vec2(0.f, 0.f);
	Vec2 startVec = Vec2(100, 100);
	Vec2 endVec = Vec2(-100, -100);
	Vec2 p;

	//CATMULL-ROM
	std::vector<Vec2> Spos;
	std::vector<Vec2> Evec;

	glm::vec3 fogColor = glm::vec3(0.2, 0.3, 0.3);

};