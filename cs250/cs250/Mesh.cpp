#include "GLShader.h"
#include "Mesh.h"
#include <GLFW/glfw3.h>
#include "Window.h"
#include "Camera.h"
#include <iostream>

GLShader shaderForMesh;
Camera cam;

Vec2 Mesh::changeEPos(float xpos, float ypos)
{
	changedpos = Vec2(xpos / 1100.f, ypos / 800.f);
	return changedpos;
}
  
void Mesh::Init(int stacks, int slices, meshSelect mesh)
{
	thisMesh = mesh;
	shaderForMesh.makeProgram(mVertexShaderPath, mFragShaderpath);
	shaderForMesh.Link();
	Stack = stacks;
	Slice = slices;
	
	switch (mesh)
	{
	case meshSelect::CUBE:
		//for (float stack = 0; stack <= Stack; ++stack)
		//{
		//	float row = stack / Stack;
		//	for (float slice = 0; slice <= Slice; ++slice)
		//	{
		//		float col = (float)slice / Slice;

		//		vt.position = Vec3(col - 0.5f, 0.5f - row, 0.f);
		//		vt.normalVector = Vec3(0.f, 0.f, 1.f);
		//		vt.upVector = Vec2(col, row);

		//		vPlaneBuffer.push_back(vt);
		//		++numVert;
		//	}
		//}

		//stride = Slice + 1;
		//for (int m = 0; m < Stack; ++m)
		//{
		//	int currNow = m * stride;
		//	for (int n = 0; n < Slice; ++n)
		//	{
		//		p0 = static_cast<int>(currNow + n);
		//		p1 = p0 + 1;
		//		p2 = p1 + stride;

		//		if (!DegenerateTri(vPlaneBuffer[p0].position, vPlaneBuffer[p1].position, vPlaneBuffer[p2].position))
		//		{
		//			iPlaneBuffer.push_back(p0);
		//			iPlaneBuffer.push_back(p1);
		//			iPlaneBuffer.push_back(p2);
		//			numIndi += 3;

		//			if (numIndi % 3 == 0)
		//			{
		//				++numTri;
		//			}
		//		}

		//		p3 = p2;
		//		p4 = p3 - 1;
		//		p5 = p0;

		//		if (!DegenerateTri(vPlaneBuffer[p3].position, vPlaneBuffer[p4].position, vPlaneBuffer[p5].position))
		//		{
		//			iPlaneBuffer.push_back(p3);
		//			iPlaneBuffer.push_back(p4);
		//			iPlaneBuffer.push_back(p5);
		//			numIndi += 3;

		//			if (numIndi % 3 == 0)
		//			{
		//				++numTri;
		//			}
		//		}
		//	}
		//}

		//for (float i = 0; i < 6; ++i)
		//{
		//	for (float j = 0; j < numVert; ++j)
		//	{
		//		v.position = Vec3(Vec4(vPlaneBuffer[j].position, 1.0));
		//		v.normalVector = Vec3(Vec4(vPlaneBuffer[j].normalVector, 1.0));
		//		v.upVector = vPlaneBuffer[j].upVector;

		//		v.position = RoundDecimal(v.position);
		//		v.normalVector = RoundDecimal(v.normalVector);

		//		addVertex(v);
		//	}

		//	for (float j = 0; j < numIndi; ++j)
		//	{
		//		addIndex(iPlaneBuffer[j] + numVert * i);
		//	}
		//}

		makeCube(stacks, slices);
		break;

	case meshSelect::CYLINDER:
		makeCylinder(stacks, slices);
		break;

	case meshSelect::SPHERE:
		makeSphere(stacks, slices);
		break;

	case meshSelect::TORUS:
		makeTorus(stacks, slices);
		break;

	case meshSelect::PLANE:
		makePlane(stacks, slices);
		break;

	case meshSelect::HERMITECURVE:
		break;
	case meshSelect::CATMULL_ROM:
		break;
	}
	//BuildIndexBuffer(stacks, slices);
	initializing();
}

void Mesh::Init(float count_ ,meshSelect mesh)
{
	if (mesh == meshSelect::HERMITECURVE)
	{
		thisMesh = meshSelect::HERMITECURVE;
	}
	else if (mesh == meshSelect::CATMULL_ROM)
	{
		thisMesh = meshSelect::CATMULL_ROM;

		Evec.push_back(startVec);
		for (int i = 0; i < 5; i++)
		{
			Evec.push_back(Vec2(-100.f + (static_cast<float>(i)*20), -100.f +(static_cast<float>(i) * 20)));
		}
		Spos.push_back(Vec2(-500.f, 0.f));
		Spos.push_back(Vec2(-300.f, -200.f));
		Spos.push_back(Vec2(-100.f, 300.f));
		Spos.push_back(Vec2(100.f, 400.f));
		Spos.push_back(Vec2(300.f, -250.f));
		Spos.push_back(Vec2(500.f, -100.f));
	}
	shaderForMesh.makeProgram(mVertexShaderPath, mFragShaderpath);
	shaderForMesh.Link();
	
	count = count_;
	makeHermite(count);
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glBufferData(GL_ARRAY_BUFFER, (IndexBuffer.size() * sizeof(glm::vec2)), &VertexBuffer[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, (IndexBuffer.size() * sizeof(int)), &IndexBuffer[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), (void*)0);

	glBindVertexArray(0);
}

void Mesh::Draw()
{
	RenderIMGUI();
	
	if (showShaders == true)
	{
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
	}
	else
	{
		glDisable(GL_CULL_FACE);
	}

	shaderForMesh.Use();
	if (thisMesh == meshSelect::HERMITECURVE || thisMesh == meshSelect::CATMULL_ROM)
	{
		glBindVertexArray(VAO);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glDrawElements(GL_LINE_STRIP, IndexBuffer.size(), GL_UNSIGNED_INT, nullptr);
	}
	else
	{
		view = Mat4(1.f);
		projection = Mat4(1.f);
		view = cam.GetViewMatrix();

		projection = glm::perspective(glm::radians(cam.Zoom), (float)Window::windowWidth / (float)Window::windowHeight, 0.1f, 100.0f);
		glUniformMatrix4fv(glGetUniformLocation(shaderForMesh.ShaderProgram, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(glGetUniformLocation(shaderForMesh.ShaderProgram, "view"), 1, GL_FALSE, glm::value_ptr(view));

		model = Mat4(1.f);
		model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(1.0f, 1.0f, 0.0f));
		GLint i = glGetUniformLocation(shaderForMesh.ShaderProgram, "model");
		glUniformMatrix4fv(i, 1, GL_FALSE, glm::value_ptr(model));

		//light
		glUniform3f(glGetUniformLocation(shaderForMesh.ShaderProgram, "lightPos"), 0, 0, 0);
		glUniform3f(glGetUniformLocation(shaderForMesh.ShaderProgram, "lightColor"), 1, 1, 1);
		glUniform3f(glGetUniformLocation(shaderForMesh.ShaderProgram, "objectColor"), 0.5, 0.2, 0.9);
		glBindVertexArray(VAO);

		//toon
		glUniform1f(glGetUniformLocation(shaderForMesh.ShaderProgram, "level"), 3);
		//fog
		glUniform3f(glGetUniformLocation(shaderForMesh.ShaderProgram, "fogColor"), fogColor.x, fogColor.y, fogColor.z);
		//glUniform3f(glGetUniformLocation(shaderForMesh.ShaderProgram, "camPos"), ); //get camera position
		glPolygonMode(GL_FRONT_AND_BACK, MeshType);
		glDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_INT, nullptr);
	}
	shaderForMesh.Unuse();
}


void Mesh::Update()
{
	//if (thisMesh == meshSelect::CYLINDER)
	//{
	//	makeCylinder(Stack, Slice);

	//	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	//	glBufferData(GL_ARRAY_BUFFER, (indexBuffer.size() * sizeof(glm::vec2)), &vertexBuffer[0], GL_STATIC_DRAW);

	//	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	//	glBufferData(GL_ELEMENT_ARRAY_BUFFER, (indexBuffer.size() * sizeof(int)), &indexBuffer[0], GL_STATIC_DRAW);

	//}
	if (thisMesh == meshSelect::SPHERE)
	{
		makeSphere(Stack, Slice);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);

		glBufferData(GL_ARRAY_BUFFER, (indexBuffer.size() * sizeof(glm::vec2)), &vertexBuffer[0], GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, (indexBuffer.size() * sizeof(int)), &indexBuffer[0], GL_STATIC_DRAW);

	}
	if (thisMesh == meshSelect::TORUS)
	{
		makeTorus(Stack, Slice);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);

		glBufferData(GL_ARRAY_BUFFER, (indexBuffer.size() * sizeof(glm::vec2)), &vertexBuffer[0], GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, (indexBuffer.size() * sizeof(int)), &indexBuffer[0], GL_STATIC_DRAW);
	}
	if (thisMesh == meshSelect::PLANE)
	{
		makePlane(Stack, Slice);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);

		glBufferData(GL_ARRAY_BUFFER, (indexBuffer.size() * sizeof(glm::vec2)), &vertexBuffer[0], GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, (indexBuffer.size() * sizeof(int)), &indexBuffer[0], GL_STATIC_DRAW);
	}
	if (thisMesh == meshSelect::HERMITECURVE)
	{
		makeHermite(count);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);

		glBufferData(GL_ARRAY_BUFFER, (IndexBuffer.size() * sizeof(glm::vec2)), &VertexBuffer[0], GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, (IndexBuffer.size() * sizeof(int)), &IndexBuffer[0], GL_STATIC_DRAW);
	}

	if (thisMesh == meshSelect::CATMULL_ROM)
	{
		makeCatmull(count);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);

		glBufferData(GL_ARRAY_BUFFER, (IndexBuffer.size() * sizeof(glm::vec2)), &VertexBuffer[0], GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, (IndexBuffer.size() * sizeof(int)), &IndexBuffer[0], GL_STATIC_DRAW);
	}
}

void Mesh::RenderIMGUI()
{
	ImGui::Begin("Mesh");
	{
		if (thisMesh == meshSelect::HERMITECURVE)
		{
		ImGui::SliderFloat("Start Velocity.x", &startVec.x, -8000.f, 8000.f);
		ImGui::SliderFloat("Start Velocity.y", &startVec.y, -8000.f, 8000.f);
		ImGui::SliderFloat("End Velocity.x", &endVec.x, -8000.f, 8000.f);
		ImGui::SliderFloat("End Velocity.y", &endVec.y, -8000.f, 8000.f);
		}
		else if (thisMesh == meshSelect::CATMULL_ROM)
		{
			ImGui::SliderFloat("Start Pos.x", &Spos[0].x, -500.f, 500.f);
			ImGui::SliderFloat("Start Pos.y", &Spos[0].y, -500.f, 500.f);
			ImGui::SliderFloat("Third Pos.x", &Spos[2].x, -500.f, 500.f);
			ImGui::SliderFloat("Third Pos.y", &Spos[2].y, -500.f, 500.f);
			ImGui::SliderFloat("End Pos.x", &Spos[5].x, -500.f, 500.f);
			ImGui::SliderFloat("End Pos.y", &Spos[5].y, -500.f, 500.f);
		}
	else
		{
			ImGui::Checkbox("Toon & Fog Shader", &showShaders);
			if (ImGui::MenuItem("Filled"))
			{
				MeshType = GL_FILL;
			}
			else if (ImGui::MenuItem("Lined"))
			{
				MeshType = GL_LINE;
			}
		
			if (ImGui::MenuItem("Cube")) {
				VertexBuffer.clear();
				IndexBuffer.clear();
				numVertices = 0;
				numIndices = 0;
				numTris = 0;
				Init(Stack, Slice, meshSelect::CUBE);
			}
			else if (ImGui::MenuItem("Cylinder")) {
				VertexBuffer.clear();
				IndexBuffer.clear();
				numVertices = 0;
				numIndices = 0;
				numTris = 0;
				Init(Stack, Slice, meshSelect::CYLINDER);
			}
			else if (ImGui::MenuItem("Sphere"))
			{
				vertexBuffer.clear();
				indexBuffer.clear();
				numVertices = 0;
				numIndices = 0;
				numTris = 0;
				Init(Stack, Slice, meshSelect::SPHERE);
			}
			else if (ImGui::MenuItem("Torus"))
			{
				vertexBuffer.clear();
				indexBuffer.clear();
				numVertices = 0;
				numIndices = 0;
				numTris = 0;
				Init(Stack, Slice, meshSelect::TORUS);
			}
			else if (ImGui::MenuItem("Plane"))
			{
				vertexBuffer.clear();
				indexBuffer.clear();
				numVertices = 0;
				numIndices = 0;
				numTris = 0;
				Init(Stack, Slice, meshSelect::PLANE);
			}
			ImGui::SliderInt("Stack", &Stack, 1, 20);
			ImGui::SliderInt("Slice", &Slice, 1, 20);
			if (thisMesh == meshSelect::SPHERE)
			{
				ImGui::SliderFloat("Radius", &radius, 0.01f, 1.f);
			}
			if (thisMesh == meshSelect::TORUS)
			{
				ImGui::SliderFloat("Outer Radius", &R, 0.01f, 1.f);
				ImGui::SliderFloat("Inner Radius", &r, 0.01f, 0.5f);
			}
		}
	}
}

void Mesh::makeCube(int stacks, int slices)
{
	vertexBuffer.clear();
	indexBuffer.clear();
	Stack = stacks;
	Slice = slices;

	for (int i = 0; i < stacks; ++i) {
		float row = i / stacks;

		for (int j = 0; j < slices; ++j) {
			float col = j / slices;

			v.position = glm::vec3(col - 0.5, row - 0.5, 0.0);
			v.normalVector = glm::vec3(0.0, 0.0, 1.0);
			v.upVector = glm::vec2(col, row);

			addVertex(v);
		}
	}

	BuildIndexBuffer(stacks, slices);
}

void Mesh::makeCylinder(int stacks, int slices)
{
	vertexBuffer.clear();
	indexBuffer.clear();
	Stack = stacks;
	Slice = slices;

	//for (int i = 0; i < slices; i++) {
	//	float theta = 2.0f * PI * (float)i / (float)slices;

	//	float x = radius * cos(theta);
	//	float y = radius * sin(theta);

	//	v.position = glm::vec3(x, y, 1.f);
	//	v.normalVector = glm::vec3(v.position.x / radius, v.position.z / radius, v.position.y / radius);
	//	v.upVector = glm::vec2(x, y);

	//	addVertex(v);
	//}

	for (int i = 0; i < stacks; ++i) {
		float row = i / stacks;

		for (int j = 0; j < slices; ++j) {
			float col = j / slices;

			float alpha = col * 2.0 * PI;
			v.position = glm::vec3(0.5 * sin(alpha), row - 0.5, 0.5 * cos(alpha));
			v.normalVector = glm::vec3(v.position.x / 0.5, 0.0, v.position.z / 0.5);
			v.upVector = glm::vec2(col, row);

			addVertex(v);
		}
	}

	BuildIndexBuffer(stacks, slices);
}

void Mesh::makeSphere(int stacks, int slices)
{
	vertexBuffer.clear();
	indexBuffer.clear();
	Stack = stacks;
	Slice = slices;
	
	for (float i = 0; i <= stacks; i++) {
		float row = i / stacks;
		float beta = static_cast<float>(PI * (row - 0.5));

		for (float j = 0; j <= slices; j++) {
			float col = j / slices;
			float alpha = static_cast<float>(col * PI * 2.0);

			v.position = { radius * sin(alpha) * cos(beta) , radius * sin(beta), radius * cos(alpha) * cos(beta) };
			v.normalVector = glm::vec3{ v.position.x / radius, v.position.y / radius, v.position.z / radius };
			v.upVector = glm::vec2{ col, row }; //texture
			addVertex(v);
		}
	}
	BuildIndexBuffer(stacks, slices);
}

void Mesh::makeTorus(int stacks, int slices)
{
	vertexBuffer.clear();
	indexBuffer.clear();
	Stack = stacks;
	Slice = slices;
	
	for (float i = 0; i <= stacks; i++) {
		float row = i / stacks;
		float alpha = 2.0 * row * PI;

		float sinAlpha = sin(alpha);
		float cosAlpha = cos(alpha);

		for (float j = 0; j <= slices; j++) {
			float col = j / slices;
			double beta = 2.0 * col * PI;

			v.position = { (R + r * cos(beta)) * sinAlpha, r * sin(beta), (R + r * cos(beta)) * cosAlpha };
			v.normalVector = { r * sin(alpha) * cos(beta), r * sin(beta), r * cos(alpha) * cos(beta) };
			v.normalVector /= r;
			v.upVector = { col, row };

			addVertex(v);
		}
	}
	BuildIndexBuffer(stacks, slices);
}


void Mesh::makePlane(int stacks, int slices)
{
	vertexBuffer.clear();
	indexBuffer.clear();
	Stack = stacks;
	Slice = slices;
	
	for (float i = 0; i <= stacks; ++i)
	{
		float row = i / stacks;

		for (float j = 0; j <= slices; ++j)
		{
			float col = j / slices;

			v.position = Vec3(col - 0.5f, 0.5f - row, 0.f);
			v.normalVector = Vec3(0.f, 0.f, 1.f);
			v.upVector = Vec2(col, row);
			addVertex(v);
		}
	}
	BuildIndexBuffer(stacks, slices);
}

void Mesh::makeHermite(float count)
{
	VertexBuffer.clear();
	IndexBuffer.clear();

	double t = static_cast<double>(1. / count);
	int countLocal = 0;

	for (double i = 0.; countLocal <= count; i += t)
	{
		double h0, h1, h2, h3;
		h0 = (2 * i * i) - (3 * i * i) + 1;
		h1 = (-2 * i * i * i) + (3 * i * i);
		h2 = (i * i * i) - (2 * i * i) + i;
		h3 = (i * i * i) - (i * i);

		p.x = (h0 * startPos.x) + (h1 * endPos.x) + (h2 * startVec.x) + (h3 * endVec.x);
		p.y = (h0 * startPos.y) + (h1 * endPos.y) + (h2 * startVec.y) + (h3 * endVec.y);


		VertexBuffer.push_back(Vec2{p.x / (1100.f / 2.f), p.y / 880.f / 2.f});
		IndexBuffer.push_back(countLocal);
		countLocal++;
	}
}

void Mesh::makeCatmull(float count)
{

	VertexBuffer.clear();
	IndexBuffer.clear();
	double t = static_cast<double>(1. / count);
	int countLocal = 0;

	for (int n = 0; n < 3; n++)
	{
		for (double i = 0.; i <= 1.; i += t)
		{
			p.x = 0.5f * ((2.f * Spos[n + 1].x) - (-Spos[n].x + Spos[n + 2].x) * i + (2.f * Spos[n].x - 5.f * Spos[n + 1].x + 4.f * Spos[n + 2].x - Spos[n + 3].x) * i * i +
				(-Spos[n].x + 3.f * Spos[n + 1].x - 3.f * Spos[n + 2].x + Spos[n + 3].x )* i * i * i);
			p.y = 0.5f * ((2.f * Spos[n + 1].y) - (-Spos[n].y + Spos[n + 2].y) * i + (2.f * Spos[n].y - 5.f * Spos[n + 1].y + 4.f * Spos[n + 2].y - Spos[n + 3].y) * i * i +
				(-Spos[n].y + 3.f * Spos[n + 1].y - 3.f * Spos[n + 2].y + Spos[n + 3].y )* i * i * i);

			VertexBuffer.push_back(Vec2{ p.x / (1100.f / 2.f), p.y / 880.f / 2.f });
			IndexBuffer.push_back(countLocal);
			countLocal++;
		}
	}
}

void Mesh::initializing()
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

void Mesh::addIndex(int index)
{
	indexBuffer.push_back(index);
	++numIndices;

	if (numIndices % 3 == 0)
	{
		++numTris;
	}
}

void Mesh::addVertex(Vertex& v)
{
	vertexBuffer.push_back(v);
	++numVertices;
}

void Mesh::BuildIndexBuffer(int stacks, int slices)
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
