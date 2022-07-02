// Juhong Mo, Geonhwi Sim, Taeju Kwon
// Hello Triangle
// CS250
// Spring 2022
#include "GLShader.h"
#include "Mesh.h"
#include "Window.h"
#include "ValueNoise.h"
#include "PerlinNoise.h"
#include "Tessellation.h"
#include "Window.h"
#include "ShadowMapping.h"

void InitIMGUI();
void InitBool(bool trueOne);

ImVec4 clear_color;
struct showScenes
{
	inline static bool showMesh = false;
	inline static bool showToon = false;
	inline static bool showFog = false;
	inline static bool showValue = false;
	inline static bool showPerlin = false;
	inline static bool showHermite = false;
	inline static bool showCatmullRom = false;
	inline static bool showGeoTes = false;
	inline static bool showShadow = false;
};

int main()
{
	Window::windowInit(1400, 880);
	GLenum err = glewInit();
	InitIMGUI();

	// Check Hermite Curve
	Mesh* HermiteMesh = new Mesh("shaders/hermiteCurve.vert", "shaders/hermiteCurve.frag");
	HermiteMesh->Init(10, Mesh::meshSelect::HERMITECURVE);

	// Check Catmull-Rom Splines
	Mesh* CatmullMesh = new Mesh("shaders/hermiteCurve.vert", "shaders/hermiteCurve.frag");
	CatmullMesh->Init(20, Mesh::meshSelect::CATMULL_ROM);
	
	// Check Procedural Modeling
	Mesh* mesh = new Mesh("shaders/mesh.vert", "shaders/mesh.frag");
	mesh->Init(20, 20, Mesh::meshSelect::SPHERE);
	
	Mesh* ToonMesh = new Mesh("shaders/toon.vert", "shaders/toon.frag");
	ToonMesh->Init(20, 20, Mesh::meshSelect::SPHERE);
	Mesh* FogMesh = new Mesh("shaders/light.vert", "shaders/light.frag");
	FogMesh->Init(20, 20, Mesh::meshSelect::SPHERE);

	// Check Value Noise
	ValueNoise* value = new ValueNoise("shaders/noise.vert", "shaders/noise.frag", 2016);
	value->Init(10, 10);

	// Check Perlin Noise
	PerlinNoise* perlin = new PerlinNoise("shaders/noise.vert", "shaders/noise.frag", 2016);
	perlin->Init(10, 10);
	
	// Check Geometry & Tessellation Shader
	Tessellation* tess = new Tessellation("shaders/tess.vert", "shaders/tess.tesc", "shaders/tess.tese", "shaders/tess.geom", "shaders/tess.frag");
	tess->Setup();

	// Check Shadow Mapping
	shadowMapping* shad = new shadowMapping();
	shad->setup();
	
	while (!glfwWindowShouldClose(Window::ptr_window))
	{
		float currentFrame = static_cast<float>(glfwGetTime());
		Window::deltaTime = currentFrame - Window::lastFrame;
		Window::lastFrame = currentFrame;
		
		glfwPollEvents();
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		glClear(GL_DEPTH_BUFFER_BIT);

		clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		ImGui::Begin("CS250 Project KaN");
		if (ImGui::Button("Mesh"))
		{
			showScenes::showMesh		= true;
			showScenes::showToon		= false;
			showScenes::showFog			= false;
			showScenes::showValue		= false;
			showScenes::showPerlin		= false;
			showScenes::showHermite		= false;
			showScenes::showCatmullRom	= false;
			showScenes::showGeoTes		= false;
			showScenes::showShadow		= false;
		}
		if (ImGui::Button("Toon"))
		{
			showScenes::showMesh		= false;
			showScenes::showToon		= true;
			showScenes::showFog			= false;
			showScenes::showValue		= false;
			showScenes::showPerlin		= false;
			showScenes::showHermite		= false;
			showScenes::showCatmullRom	= false;
			showScenes::showGeoTes		= false;
			showScenes::showShadow		= false;
		}
		if (ImGui::Button("Fog"))
		{
			showScenes::showMesh		= false;
			showScenes::showToon		= false;
			showScenes::showFog			= true;
			showScenes::showValue		= false;
			showScenes::showPerlin		= false;
			showScenes::showHermite		= false;
			showScenes::showCatmullRom	= false;
			showScenes::showGeoTes		= false;
			showScenes::showShadow		= false;
		}
		if (ImGui::Button("Value Noise"))
		{
			showScenes::showMesh		= false;
			showScenes::showToon		= false;
			showScenes::showFog			= false;
			showScenes::showValue		= true;
			showScenes::showPerlin		= false;
			showScenes::showHermite		= false;
			showScenes::showCatmullRom	= false;
			showScenes::showGeoTes		= false;
			showScenes::showShadow		= false;
		}
		if (ImGui::Button("Perlin Noise"))
		{
			showScenes::showMesh		= false;
			showScenes::showToon		= false;
			showScenes::showFog			= false;
			showScenes::showValue		= false;
			showScenes::showPerlin		= true;
			showScenes::showHermite		= false;
			showScenes::showCatmullRom	= false;
			showScenes::showGeoTes		= false;
			showScenes::showShadow		= false;
		}
		if (ImGui::Button("Hermite"))
		{
			showScenes::showMesh		= false;
			showScenes::showToon		= false;
			showScenes::showFog			= false;
			showScenes::showValue		= false;
			showScenes::showPerlin		= false;
			showScenes::showHermite		= true;
			showScenes::showCatmullRom	= false;
			showScenes::showGeoTes		= false;
			showScenes::showShadow		= false;
		}
		if (ImGui::Button("Catmull Rom"))
		{
			showScenes::showMesh		= false;
			showScenes::showToon		= false;
			showScenes::showFog			= false;
			showScenes::showValue		= false;
			showScenes::showPerlin		= false;
			showScenes::showHermite		= false;
			showScenes::showCatmullRom	= true;
			showScenes::showGeoTes		= false;
			showScenes::showShadow		= false;
		}
		if (ImGui::Button("Geometry & Tessellation"))
		{
			showScenes::showMesh		= false;
			showScenes::showToon		= false;
			showScenes::showFog			= false;
			showScenes::showValue		= false;
			showScenes::showPerlin		= false;
			showScenes::showHermite		= false;
			showScenes::showCatmullRom	= false;
			showScenes::showGeoTes		= true;
			showScenes::showShadow		= false;
		}
		if (ImGui::Button("Shadow Mapping"))
		{
			showScenes::showMesh		= false;
			showScenes::showToon		= false;
			showScenes::showFog			= false;
			showScenes::showValue		= false;
			showScenes::showPerlin		= false;
			showScenes::showHermite		= false;
			showScenes::showCatmullRom	= false;
			showScenes::showGeoTes		= false;
			showScenes::showShadow		= true;
		}
		if (showScenes::showFog == true)
		{
			FogMesh->Draw();
			FogMesh->Update();
		}
		if (showScenes::showValue == true)
		{
			value->Draw();
		}
		if (showScenes::showPerlin == true)
		{
			perlin->Draw();
		}
		if (showScenes::showHermite == true)
		{
			HermiteMesh->Draw();
			HermiteMesh->Update();
		}
		if (showScenes::showCatmullRom == true)
		{
			CatmullMesh->Draw();
			CatmullMesh->Update();
		}
		if (showScenes::showGeoTes == true)
		{
			tess->Draw();
			tess->Update();
		}
		if (showScenes::showMesh == true)
		{
			mesh->Draw();
			mesh->Update();
		}
		if (showScenes::showToon == true)
		{
			ToonMesh->Draw();
			ToonMesh->Update();
		}
		if (showScenes::showShadow == true)
		{
			shad->draw();
		}
		
		ImGui::End();
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		Window::processInput(Window::ptr_window);
		glfwSwapBuffers(Window::ptr_window);
	}
	/*delete mesh;
	delete ToonMesh;
	delete FogMesh;
	delete value;
	delete perlin;
	delete HermiteMesh;
	delete CatmullMesh;
	delete tess;*/
	
	glfwTerminate();
	return 0;
}

void InitIMGUI()
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui_ImplGlfw_InitForOpenGL(Window::ptr_window, false);
	const char* glsl_version = "#version 330";
	ImGui_ImplOpenGL3_Init(glsl_version);
	ImGui::StyleColorsDark();
}
