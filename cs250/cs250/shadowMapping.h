#pragma once
#include "GLShader.h"
#include "Mesh.h"
#include "Camera.h"

class shadowMapping
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

	void init();
	void setup();
	void setupDepthTexture();
	
	void draw();

	void renderScene(GLShader& shader);

	// mesh create functions
	void generatePlane();
	void generateCube();

private:
	const unsigned int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;
	unsigned int depthMapFBO;
	
	Vec3 lightPos{ -2.f, 4.f, -1.f };
	Camera camera;
	
	GLShader shader;
	GLShader simpleDepthShader;
	GLShader debugDepthQuad;
	
	std::vector<Vertex> vertexBuffer;
	std::vector<int> indexBuffer;

	unsigned int depthMap;
	
	unsigned int planeVAO;
	unsigned int planeVBO;
	
	unsigned int cubeVAO = 0;
	unsigned int cubeVBO = 0;
};