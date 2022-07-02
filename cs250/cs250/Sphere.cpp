#include "GLShader.h"
#include "Sphere.h"

#include <GLFW/glfw3.h>

#include "Window.h"
#include "Camera.h"
#include "IMGUI.h"

GLShader shaderSphere;
Camera cam;

Sphere::Sphere(const char* vert, const char* frag) : Mesh(vert, frag)
{
	mVertexShaderPath = vert;
	mFragShaderpath = frag;
}

void Sphere::Init(int stacks, int slices)
{
    shaderSphere.makeProgram(mVertexShaderPath, mFragShaderpath);
    shaderSphere.Link();
	IMGUI::InitIMGUI();

	Stack = stacks;
	Slice = slices;

	for (float i = 0; i <= Stack; i++) {
		float row = i / Stack;
		float beta = static_cast<float>(PI * (row - 0.5));

		for (float j = 0; j <= Slice; j++) {
			float col = j / Slice;
			float alpha = static_cast<float>(col * PI * 2.0);

			v.position = { radius * sin(alpha) * cos(beta) , radius * sin(beta), radius * cos(alpha) * cos(beta) };
			v.normalVector = glm::vec3{ v.position.x / radius, v.position.y / radius, v.position.z / radius };
			v.upVector = glm::vec2{ col, row }; //texture
			addVertex(v);
		}
	}
	BuildIndexBuffer(Stack, Slice);
	initializing();
}

void Sphere::Draw()
{	
	shaderSphere.Use();
	
	view = Mat4(1.f);
	projection = Mat4(1.f);
	view = cam.GetViewMatrix();
	
	projection = glm::perspective(glm::radians(cam.Zoom), (float)Window::windowWidth / (float)Window::windowHeight, 0.1f, 100.0f);
	glUniformMatrix4fv(glGetUniformLocation(shaderSphere.ShaderProgram, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
	glUniformMatrix4fv(glGetUniformLocation(shaderSphere.ShaderProgram, "view"), 1, GL_FALSE, glm::value_ptr(view));
	
	model = Mat4(1.f);
	model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));
	glUniformMatrix4fv(glGetUniformLocation(shaderSphere.ShaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));

	glBindVertexArray(VAO);
	
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_INT, nullptr);
	//shaderSphere.Unuse();
}

void Sphere::Update()
{
	
}

void Sphere::RenderIMGUI()
{
	
}