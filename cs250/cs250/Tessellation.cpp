#include "GLShader.h"
#include "Window.h"
#include "Tessellation.h"

#include <algorithm>

#include "MeshTess.h"
#include <glm/gtc/matrix_inverse.hpp>

GLShader shaderForTess;

void Tessellation::Setup()
{
	shaderForTess.makeProgram(mVertShaderPath, mTcsShaderPath, mTesShaderPath, mGeomShaderPath, mFragShaderPath);
	shaderForTess.Link();
	meshT = CreateIcosahedron();
	SendVertexData(meshT);
	
	TessLevelInner = 1;
	TessLevelOuter = 1;

	constexpr float HalfWidth = 0.6f;
	const float		HalfHeight = (HalfWidth * Window::windowHeight) / Window::windowWidth;
	ProjectionMatrix = glm::frustum(-HalfWidth, HalfWidth, -HalfHeight, HalfHeight, 5.f, 100.0f);
	
	shaderForTess.Use();
	glUniform3f(glGetUniformLocation(shaderForTess.ShaderProgram, "AmbientMaterial"), 0.04f, 0.04f, 0.04f);
	glUniform3f(glGetUniformLocation(shaderForTess.ShaderProgram, "DiffuseMaterial"), 0.392f, 0.584f, 0.929f);
	glUniform3f(glGetUniformLocation(shaderForTess.ShaderProgram, "LightPosition"), 0.25f, 0.25f, 1.0f);
}

void Tessellation::Update()
{
	const double ellapsed_seconds = glfwGetTime();
	const float theta = static_cast<float>(ellapsed_seconds * 0.25);
	glm::mat4 rotation = Rotate(theta, Vec3{ 1, 0, 0 });
	glm::mat4 translation = Translate(Vec3{ 0, 0, depth });
	Vec3 eyePosition{ 0, 0, -10 };
	Vec3 targetPosition{ 0, 0, 0 };
	const Vec3 relativeUp{ 0, 1, 0 };
	glm::mat4 look_at = LookAt(eyePosition, targetPosition, relativeUp);
	ModelviewMatrix = look_at * translation * rotation;
}

void Tessellation::Draw()
{
	shaderForTess.Use();
	glUniform1f(glGetUniformLocation(shaderForTess.ShaderProgram, "TessLevelInner"), TessLevelInner);
	glUniform1f(glGetUniformLocation(shaderForTess.ShaderProgram, "TessLevelOuter"), TessLevelOuter);
	glUniformMatrix4fv(glGetUniformLocation(shaderForTess.ShaderProgram, "Projection"), 1, GL_FALSE, &ProjectionMatrix[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(shaderForTess.ShaderProgram, "Modelview"), 1, GL_FALSE, &ModelviewMatrix[0][0]);
	glUniform1f(glGetUniformLocation(shaderForTess.ShaderProgram, "Shrink"), Shrink);
	
	NormalMatrix = inverseTranspose(glm::mat3(ModelviewMatrix));
	glUniformMatrix3fv(glGetUniformLocation(shaderForTess.ShaderProgram, "NormalMatrix"), 1, GL_FALSE, &NormalMatrix[0][0]);
	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_BUFFER);
	glEnable(GL_CULL_FACE);
	
	glPatchParameteri(GL_PATCH_VERTICES, 3);
	glBindVertexArray(meshT.VAO);
	glDrawElements(GL_PATCHES, meshT.numIndices, GL_UNSIGNED_INT, nullptr);
	RenderIMGUI();
}

void Tessellation::RenderIMGUI()
{
	ImGui::Begin("Tess Parameters");

	ImGui::DragFloat("TessLevelInner", &TessLevelInner, 1.0f);
	TessLevelInner = std::clamp(TessLevelInner, 1.0f, 64.f);
	
	ImGui::DragFloat("TessLevelOuter", &TessLevelOuter, 1.0f, 64.0f);
	TessLevelOuter = std::clamp(TessLevelOuter, 1.0f, 64.f);

	ImGui::DragFloat("Depth", &depth, 5.f);
	depth = std::clamp(depth, -4.0f, 140.0f);
	
	ImGui::DragFloat("Shrink", &Shrink, 0.025f);
	Shrink = std::clamp(Shrink, 0.0f, 1.0f);
}