#pragma once
#include "MeshTess.h"
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

class Tessellation : public MeshTess
{
public:
	Tessellation(const char* vert, const char* tcs, const char* tes, const char* geo, const char* frag)
		: mVertShaderPath(vert), mTcsShaderPath(tcs), mTesShaderPath(tes), mGeomShaderPath(geo), mFragShaderPath(frag) {}

	void Setup();
	void Update();
	void Draw();
	void RenderIMGUI();

protected:
	MeshTess meshT;
	const char* mVertShaderPath = nullptr;
	const char* mFragShaderPath = nullptr;
	const char* mTcsShaderPath = nullptr;
	const char* mTesShaderPath = nullptr;
	const char* mGeomShaderPath = nullptr;

	ImVec4 clear_color;
	float TessLevelInner = 1.f;
	float TessLevelOuter = 1.f;
	glm::mat4 ProjectionMatrix;
	glm::mat4 ModelviewMatrix;
	glm::mat3 NormalMatrix;
	float Shrink = 0.9f;
	float depth = 5.f;
};
