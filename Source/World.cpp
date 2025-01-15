#include "World.h"
#include <glm/fwd.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/ext/matrix_transform.hpp>
#include "Math.h"
#include <algorithm>
#include "UIPaginatedWindow.h"
#include "TexturedObject.h"
#include "GridObject.h"
#include "MouseObject.h"
#include "Building.h"
#include "WorkShop.h"
#include "Decoration.h"
#include "UIButton.h"
#include "HUD.h"
#include <ft2build.h>
#include FT_FREETYPE_H

World::World(const std::string& InFileName)
{
	StartUp = XMLParser::LoadLevel(InFileName);
	Window = Graphics::InitWindow(StartUp.LevelWidth * StartUp.WindowScale, StartUp.LevelHeight * StartUp.WindowScale);
	// shader for buildings and hovered items
	BuildingShader = std::make_shared<Shader>("Source/Shaders/TempItem.vert", "Source/Shaders/TempItem.frag");
	InitBackground();
	InitGrid(StartUp.GridFileName);
	InitHUD();
	SetupMouseCallbacks();
	LoadSave();
	//XMLParser::ResetSave("grid_config.xml");
}

World::~World()
{
	Shaders.clear();
	MouseInteractionAPI* Api = static_cast<MouseInteractionAPI*>(glfwGetWindowUserPointer(Window));
	if (Api) {
		delete Api;
	}
	glfwTerminate();
}

void World::Update(float InDeltaSeconds)
{
	DeltaSeconds = InDeltaSeconds;
	ProcessInputGL(Window);
	RenderUpdate();
	glfwPollEvents();
}

bool World::IsRunning()
{
	return !glfwWindowShouldClose(Window);
}

void World::InitBackground()
{
	std::shared_ptr<Shader> ShaderProgram = std::make_shared<Shader>("Source/Shaders/Texture.vert", "Source/Shaders/Texture.frag");
	Shaders.push_back(ShaderProgram);
	std::vector<float> Vertices = {
		// positions          // colors           // texture coords
		 1.0f,  1.0f,    1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
		 1.0f, -1.0f,    0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right
		-1.0f, -1.0f,    0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
		-1.0f,  1.0f,    1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // top left 
	};
	std::vector<unsigned int> Indices = {
		0, 1, 3, // first triangle
		1, 2, 3  // second triangle
	};
	VertexAttribute OutVertexData = { 7,{2,3,2} };
	std::weak_ptr WeakShaderPtr = ShaderProgram;
	std::unique_ptr<TexturedObject> TexturedObj = std::make_unique<TexturedObject>(WeakShaderPtr, Vertices, Indices, StartUp.LevelFileName.c_str(), OutVertexData, this);
	ObjectRenderMap[ShaderProgram->ID].push_back(std::move(TexturedObj));
}

void World::InitHUD()
{
	auto [WindowWidth, WindowHeight] = GetWindowSize();
	GameHUD = std::make_unique<HUD>(WindowWidth, WindowHeight, this);
}

void World::InitGrid(const std::string& InFileName)
{
	std::shared_ptr<Shader> ShaderProgram = std::make_shared<Shader>("Source/Shaders/Grid.vert", "Source/Shaders/Grid.frag");
	Shaders.push_back(ShaderProgram);
	GridConfigData = XMLParser::ParseGridDataFromXML(InFileName);
	std::weak_ptr<Shader> ShaderWeakPtr = ShaderProgram;
	std::unique_ptr<GridObject> GridObj = std::make_unique<GridObject>(ShaderWeakPtr, GridConfigData, this);
	ObjectRenderMap[ShaderProgram->ID].push_back(std::move(GridObj));
}

void World::ProcessInputGL(GLFWwindow* InWindow)
{
	if (glfwGetKey(InWindow, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(InWindow, true);
	auto [WindowWidth, WindowHeight] = GetWindowSize();
	float PanSpeed = 0.01f; // Speed of panning
	if (glfwGetKey(InWindow, GLFW_KEY_LEFT) == GLFW_PRESS) {
		PanX -= PanSpeed;
	}
	if (glfwGetKey(InWindow, GLFW_KEY_RIGHT) == GLFW_PRESS) {
		PanX += PanSpeed;
	}
	if (glfwGetKey(InWindow, GLFW_KEY_UP) == GLFW_PRESS) {
		PanY += PanSpeed;
	}
	if (glfwGetKey(InWindow, GLFW_KEY_DOWN) == GLFW_PRESS) {
		PanY -= PanSpeed;
	}
	if (glfwGetKey(InWindow, GLFW_KEY_W) == GLFW_PRESS) {
		if (ZoomLevel < 3.5f)
			ZoomLevel += 0.05f;
		// so it snaps out of the other zoom type 
		MagnifierZoomLevel = 0;
	}
	if (glfwGetKey(InWindow, GLFW_KEY_S) == GLFW_PRESS) {
		ZoomLevel -= 0.05f;
		MagnifierZoomLevel = 0;
	}
	float LevelWidth = StartUp.LevelWidth;
	float LevelHeight = StartUp.LevelHeight;
	if (MagnifierZoomLevel != 0)
	{
		if (WindowWidth < WindowHeight) {
			ZoomLevel = std::clamp(static_cast<float>(MagnifierZoomLevel * (WindowHeight / LevelHeight)), WindowHeight / LevelHeight, (WindowHeight / LevelHeight) * 4.5f);
		}
		else
		{
			ZoomLevel = std::clamp(static_cast<float>(MagnifierZoomLevel * (WindowWidth / LevelWidth)), WindowWidth / LevelWidth, (WindowWidth / LevelWidth) * 4.5f);
		}
	}
	else
	{
		// adjust the caps for zoom based on this condition so it know which dimension to look at for clamping 
		if (WindowWidth < WindowHeight) {
			ZoomLevel = std::clamp(ZoomLevel, WindowHeight / LevelHeight, (WindowHeight / LevelHeight) * 4.5f);
		}
		else
		{
			ZoomLevel = std::clamp(ZoomLevel, WindowWidth / LevelWidth, (WindowWidth / LevelWidth) * 4.5f);
		}
	}
	float HalfVisibleWidth = (WindowWidth / ZoomLevel) / 2.0f;
	float HalfVisibleHeight = (WindowHeight / ZoomLevel) / 2.0f;
	float BgHalfWidth = StartUp.LevelWidth / 2.0f;
	float BgHalfHeight = StartUp.LevelHeight / 2.0f;
	float MinPanX = (-BgHalfWidth + HalfVisibleWidth) / 1000.0f;
	float MaxPanX = (BgHalfWidth - HalfVisibleWidth) / 1000.0f;
	float MinPanY = (-BgHalfHeight + HalfVisibleHeight) / 1000.0f;
	float MaxPanY = (BgHalfHeight - HalfVisibleHeight) / 1000.0f;
	// to prevent clamp from crashing 
	if (std::abs(BgHalfWidth - HalfVisibleWidth) < 0.1f || (MinPanX > MaxPanX))
		PanX = 0.0f;
	else
		PanX = std::clamp(PanX, MinPanX, MaxPanX);
	if (std::abs(BgHalfHeight - HalfVisibleHeight) < 0.1f || (MinPanY > MaxPanY))
		PanY = 0.0f;
	else
		PanY = std::clamp(PanY, MinPanY * (LevelWidth / LevelHeight), MaxPanY * (LevelWidth / LevelHeight));
	MouseInteractionAPI* Api = static_cast<MouseInteractionAPI*>(glfwGetWindowUserPointer(InWindow));
	if (Api) {
		Api->SetPanZoom(PanX, PanY, ZoomLevel);
	}
}

void World::SetupMouseCallbacks()
{
	MouseAPI = new MouseInteractionAPI(Window, GridConfigData, StartUp.LevelWidth, StartUp.LevelHeight);
	MouseAPI->SetClickCallback([this](int InGridX, int InGridY, float InScreenX, float InScreenY) {
		this->GameHUD->OnClickFunction(InGridX, InGridY, InScreenX, InScreenY);
		});
	MouseAPI->SetHoverCallback([this](int InGridX, int InGridY, float InScreenX, float InScreenY) {
		this->GameHUD->OnHoverFunction(InGridX, InGridY, InScreenX, InScreenY);
		});
}

void World::RenderUpdate()
{
	glClearColor(0.2f, 0.3f, 0.76f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	// to reduce shader use calls 
	for (auto It = Shaders.begin(); It < Shaders.end(); It++) {
		(*It)->use();
		unsigned int ShaderID = (*It)->ID;
		auto MapIt = ObjectRenderMap.find(ShaderID);
		if (MapIt != ObjectRenderMap.end()) {
			for (const auto& Object : MapIt->second) {
				Object->Draw();
			}
		}
		else {
			std::cerr << "No objects found for shader ID: " << ShaderID << std::endl;
		}
	}
	if (!BuildingsMap.empty())
	{
		for (int j = GridConfigData.Height - 1; j >= 0; j--)
		{
			for (int i = GridConfigData.Width - 1; i >= 0; i--)
			{
				// does the save indicate there's something here ?
				if (XMLParser::GetGridValue(StartUp.GridFileName, i, j) == "0")continue;
				if (BuildingsMap[{i, j}])BuildingsMap[{i, j}]->Draw();
			}
		}
	}
	GameHUD->Update();
	glfwSwapBuffers(Window);
}

void World::LoadSave()
{
	// make the rows (based on grid config e.g 2x2 , 4x2 )in the xml file if this was initial launch 
	XMLParser::CheckInitEmptySave(StartUp.GridFileName, GridConfigData.Width, GridConfigData.Height);
	for (size_t i = 0; i < GridConfigData.Width; i++) {
		for (size_t j = 0; j < GridConfigData.Height; j++) {
			std::string Value = XMLParser::GetGridValue(StartUp.GridFileName, i, j);
			if (Value != "0") {
				std::vector<float> Vertices = {
					-0.5f, -0.5f,     0.0f, 0.0f,
					 0.5f, -0.5f,     1.0f, 0.0f,
					 0.5f,  0.5f,     1.0f, 1.0f,
					-0.5f,  0.5f,     0.0f, 1.0f
				};
				std::vector<unsigned int> Indices = {
					0, 1, 2,
					2, 3, 0
				};
				std::weak_ptr<Shader> WeakBuildingShaderPtr = BuildingShader;
				VertexAttribute OutVertexData = { 4,{2,2} };
				WorkshopData TempWorkShopData = XMLParser::LoadWorkshop("ShopItems.xml", "workshops", Value);
				if (!TempWorkShopData.Name.empty()) {
					BuildingsMap[{i, j}] = 
						std::make_unique<Workshop>(WeakBuildingShaderPtr, Vertices, Indices, OutVertexData, this, i, j, TempWorkShopData);
				}
				DecorationData TempDecorationData = XMLParser::LoadDecoration("ShopItems.xml", "decorations", Value);
				if (!TempDecorationData.Name.empty()) {
					BuildingsMap[{i, j}] =
						std::make_unique<Decoration>(WeakBuildingShaderPtr, Vertices, Indices, OutVertexData, this, i, j, TempDecorationData);
				}
			}
		}
	}
}

std::pair<int, int> World::GetWindowSize()
{
	int WindowWidth, WindowHeight;
	glfwGetWindowSize(Window, &WindowWidth, &WindowHeight);
	return { WindowWidth, WindowHeight };
}

std::pair<float, float> World::GetPan()
{
	return { PanX, PanY };
}

std::pair<float, float> World::GetLevelSize()
{
	return { static_cast<float>(StartUp.LevelWidth), static_cast<float>(StartUp.LevelHeight) };
}

void World::DeleteBuilding(int InGridX, int InGridY)
{
	BuildingsMap[{InGridX, InGridY}] = nullptr;
}

void World::ChangeMagnifierZoom(int InDelta)
{
	MagnifierZoomLevel = std::clamp(MagnifierZoomLevel + InDelta, 1, 3);
}
