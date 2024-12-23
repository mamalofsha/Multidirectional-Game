#include "World.h"
#include "Graphics.h"
#include <glm/fwd.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/ext/matrix_transform.hpp>
#include "Math.h"
#include "XMLParser.h"
#include <algorithm>
#include "UIPaginatedWindow.h"
#include <ft2build.h>
#include "TexturedObject.h"
#include "GridObject.h"
#include "MouseObject.h"
#include "Building.h"
#include "WorkShop.h"

#include FT_FREETYPE_H





World::World(const std::string& InFileName)
{
	StartUp = XMLParser::LoadLeveL(InFileName);
	Window = Graphics::InitWindow(StartUp.LevelWidth * StartUp.WindowScale, StartUp.LevelHeight * StartUp.WindowScale);
	// shader for buildings and hovered items
	Buildingshader = std::make_shared<Shader>("Source/Shaders/TempItem.vert", "Source/Shaders/TempItem.frag");
	InitBackground();
	InitGrid(StartUp.GridFileName);
	InitHUD();
	SetupMouseCallbacks();
	LoadSave();
	//XMLParser::ResetSave("grid_config.xml");
}

World::~World()
{
	// check to see if it actually clears ( eg there are not other objects pointing to this shared ptr 
	Shaders.clear();
	MouseInteractionAPI* api = static_cast<MouseInteractionAPI*>(glfwGetWindowUserPointer(Window));
	if (api) {
		delete api; // Free the dynamically allocated memory
	}
	GameObjects.clear();
	glfwTerminate();
}

void World::Update(float DeltaSeconds)
{
	GarbageCollection();
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
	std::shared_ptr<Shader> shader = std::make_shared<Shader>("Source/Shaders/Texture.vert", "Source/Shaders/Texture.frag");
	Shaders.push_back(shader);
	std::vector<float> vertices = {
		// positions          // colors           // texture coords
		 1.0f,  1.0f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
		 1.0f, -1.0f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right
		-1.0f, -1.0f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
		-1.0f,  1.0f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // top left 
	};
	std::vector<unsigned int> indices = {
		0, 1, 3, // first triangle
		1, 2, 3  // second triangle
	};
	VertexAttribute OutVertexData = { 8,{3,3,2} };
	std::shared_ptr<TexturedObject> texturedObject = std::make_shared<TexturedObject>(shader, vertices, indices, StartUp.LevelFileName.c_str(), OutVertexData, this);
	objectRenderMap[shader->ID].push_back(texturedObject);
}

void World::InitHUD()
{
	auto [windowWidth, windowHeight] = GetWindowSize();
	GameHUD = std::make_unique<HUD>(windowWidth,windowHeight,this);
}

void World::InitGrid(const std::string& InFileName)
{
	std::shared_ptr<Shader> shader = std::make_shared<Shader>("Source/Shaders/Grid.vert", "Source/Shaders/Grid.frag");
	Shaders.push_back(shader);
	gridConfig = XMLParser::ParseGridDataFromXML(InFileName);
	std::shared_ptr<GridObject> gridObject = std::make_shared<GridObject>(shader, gridConfig, this);
	objectRenderMap[shader->ID].push_back(gridObject);
}

void World::ProcessInputGL(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	auto [windowWidth, windowHeight] = GetWindowSize();
	float panSpeed = 0.01f; // Speed of panning
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
		panX -= panSpeed;
	}
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
		panX += panSpeed;
	}
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
		panY += panSpeed;
	}
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
		panY -= panSpeed;
	}
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		if(Zoom < 3.5f)
		Zoom +=  0.05f;
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		Zoom -= 0.05f;
	}
	float LevelWidth = StartUp.LevelWidth;
	float LevelHeight = StartUp.LevelHeight;
	// clamp with smaller one if todo
	Zoom = std::clamp(Zoom, windowWidth/ LevelWidth, (windowWidth / LevelWidth)* 3.5f);
	// Calculate visible bounds based on zoom
	float halfVisibleWidth = (windowWidth / Zoom) / 2.0f;
	float halfVisibleHeight = (windowHeight / Zoom) / 2.0f;

	// Background dimensions (2000x1404 assumed)
	float bgHalfWidth = StartUp.LevelWidth / 2.0f;
	float bgHalfHeight = StartUp.LevelHeight/ 2.0f;

	// Calculate panning bounds
	float minPanX = (- bgHalfWidth + halfVisibleWidth)/1000.0f;
	float maxPanX = (bgHalfWidth - halfVisibleWidth) / 1000.0f;

	float minPanY = (- bgHalfHeight + halfVisibleHeight)/1000.0f;
	float maxPanY = (bgHalfHeight - halfVisibleHeight)/1000.0f;
	panX = std::clamp(panX, minPanX, maxPanX);
	// 2000/ 14004 = 1.42f
	panY = std::clamp(panY, minPanY * (LevelWidth/ LevelHeight), maxPanY * (LevelWidth / LevelHeight));
	MouseInteractionAPI* api = static_cast<MouseInteractionAPI*>(glfwGetWindowUserPointer(window));
	if (api) {
		api->SetPanZoom(panX, panY, Zoom);
	}
	//std::cout << "zoom: " << Zoom << std::endl;
	//std::cout << "pan: " << panX << std::endl;
	//std::cout << "panY lim: " << minPanY << ": " << maxPanY << std::endl;
}

void World::GarbageCollection()
{
	for (auto it = GameObjects.begin(); it != GameObjects.end(); )
	{
		if ((*it)->GetMarkedForDelete())
		{
			it = GameObjects.erase(it);
		}
		else
			++it;
	}
}

void World::SetupMouseCallbacks()
{
	// Mouse Interaction API setup
	MouseInteractionAPI* mouseAPI = new MouseInteractionAPI(Window,gridConfig,StartUp.LevelWidth,StartUp.LevelHeight);
	mouseAPI->SetClickCallback([this](int gridX, int gridY, float screenX, float screenY) {
		this->GameHUD->onClickFunction(gridX, gridY, screenX, screenY);
		});
	mouseAPI->SetHoverCallback([this](int gridX, int gridY, float screenX, float screenY) {
		this->GameHUD->onHoverFunction(gridX, gridY, screenX, screenY);
		});
	// Set the user pointer
}

void World::RenderUpdate()
{
	//
	glClearColor(0.2f, 0.3f, 0.76f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	for (auto it = Shaders.begin(); it < Shaders.end(); it++)
	{
		(*it)->use();
		unsigned int shaderID = (*it)->ID;

		// Lookup objects in the map
		auto mapIt = objectRenderMap.find(shaderID);
		if (mapIt != objectRenderMap.end()) {
			// Iterate through all objects associated with this shader
			for (const auto& object : mapIt->second) {
				object->draw(); // Draw the object
			}
		}
		else {
			std::cerr << "No objects found for shader ID: " << shaderID << std::endl;
		}
	}
	if(!builds.empty())
		for (auto it = builds.begin(); it < builds.end(); it++)
		{
			(*it)->draw();
		}

	GameHUD->Update();
	glfwSwapBuffers(Window);
}

void World::LoadSave()
{
	XMLParser::CheckInitEmptySave(StartUp.GridFileName,gridConfig.width, gridConfig.height);
	for (size_t i = 0; i < gridConfig.width; i++)
	{
		for (size_t j = 0; j < gridConfig.height; j++)
		{
			std::string value =  XMLParser::GetGridValue(StartUp.GridFileName, i, j);
			if (value != "0")
			{
				std::vector<float> vertices = {
					// Positions      // Texture Coords
					-0.5f, -0.5f,     0.0f, 0.0f, // Bottom-left
					 0.5f, -0.5f,     1.0f, 0.0f, // Bottom-right
					 0.5f,  0.5f,     1.0f, 1.0f, // Top-right
					-0.5f,  0.5f,     0.0f, 1.0f  // Top-left
				};

				std::vector<unsigned int> indices = {
					0, 1, 2, // First triangle
					2, 3, 0  // Second triangle
				};
				VertexAttribute OutVertexData = { 4,{2,2} };

				WorkshopData TempWorkShopData = XMLParser::LoadWorkShop("ShopItems.xml", "workshops", value);
				if (!TempWorkShopData.Name.empty())
				{
					builds.emplace_back(std::make_unique<Workshop>(Buildingshader, vertices, indices, OutVertexData, this, i, j, TempWorkShopData));
				}
				DecorationData TempDecoratoinData = XMLParser::LoadDecoration("ShopItems.xml", "decorations", value);
				if (!TempDecoratoinData.Name.empty())
				{
					builds.emplace_back(std::make_unique<Decoration>(Buildingshader, vertices, indices, OutVertexData, this, i, j, TempDecoratoinData));
				}
			}
		}

	}
}

std::pair<int, int> World::GetWindowSize()
{
	int windowWidth, windowHeight;
	glfwGetWindowSize(Window, &windowWidth, &windowHeight);
	return { windowWidth ,windowHeight };
}

std::pair<float, float> World::GetPan()
{
	return { panX ,panY };
}
std::pair<float, float> World::GetLevelSize()
{
	return {static_cast<float>(StartUp.LevelWidth),static_cast<float>(StartUp.LevelHeight) };
}

/// make different grid sizes work with save 