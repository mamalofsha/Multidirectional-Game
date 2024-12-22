#include "World.h"
#include "Graphics.h"
#include <glm/fwd.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/ext/matrix_transform.hpp>
#include "PlayerObject.h"
#include "Math.h"
#include "XMLParser.h"
#include <algorithm>
#include "UIPaginatedWindow.h"
#include <ft2build.h>
#include "TexturedObject.h"
#include "GridObject.h"
#include "MouseObject.h"
#include FT_FREETYPE_H

std::vector<int> World::GetWindowSize()
{
	std::vector<int> Output;
	int windowWidth, windowHeight;
	glfwGetWindowSize(Window, &windowWidth, &windowHeight);
	Output.push_back(windowWidth);
	Output.push_back(windowHeight);
	return Output;
}

World::World(std::vector<std::shared_ptr<GameObject>>& GameObjects)
{

}

World::World(const std::string& InFileName)
{
	StartUpData Temp = XMLParser::LoadLeveL(InFileName);
	Window = Graphics::InitWindow(Temp.LevelWidth * Temp.WindowScale, Temp.LevelHeight * Temp.WindowScale);
	InitBackground();

	//
	std::shared_ptr<Shader> shader = std::make_shared<Shader>("TempItem.vert", "TempItem.frag");
	// Vertex data with texture coordinates
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
	mous = std::make_shared<MouseObject>(shader,vertices,indices,"bridge.png", OutVertexData,this);
	mous->setSize(0.4f);
	//.push_back(shader);
	//objectRenderMap[shader->ID].push_back(mous);

	//
	InitHUD();
	InitGrid(Temp.GridFileName);
	SetupMouseCallbacks();
}

World::~World()
{
	// check to see if it actually clears ( eg there are not other objects pointing to this shared ptr 
	NewShaders.clear();
	MouseInteractionAPI* api = static_cast<MouseInteractionAPI*>(glfwGetWindowUserPointer(Window));
	if (api) {
		delete api; // Free the dynamically allocated memory
	}
	GameObjects.clear();
	Player = nullptr;
	Shaders.clear();
	glfwTerminate();
}

void World::InitBackground()
{
	std::shared_ptr<Shader> shader = std::make_shared<Shader>("Texture.vert", "Texture.frag");
	NewShaders.push_back(shader);
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
	std::shared_ptr<TexturedObject> texturedObject = std::make_shared<TexturedObject>(shader, vertices, indices, "Map1.jpg", OutVertexData,true,this);
	objectRenderMap[shader->ID].push_back(texturedObject);
}

void World::InitHUD()
{
	int windowWidth, windowHeight;
	glfwGetWindowSize(Window, &windowWidth, &windowHeight);
	GameHUD = std::make_unique<HUD>(windowWidth,windowHeight);
}

void World::InitGrid(const std::string& InFileName)
{
	std::shared_ptr<Shader> shader = std::make_shared<Shader>("Grid.vert", "Grid.frag");
	NewShaders.push_back(shader);
	gridConfig = XMLParser::ParseGridDataFromXML(InFileName);
	std::shared_ptr<GridObject> gridObject = std::make_shared<GridObject>(shader, gridConfig, this);
	objectRenderMap[shader->ID].push_back(gridObject);
}

void World::ProcessInputGL(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	int windowWidth, windowHeight;
	glfwGetWindowSize(window, &windowWidth, &windowHeight);
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

	Zoom = std::clamp(Zoom, windowWidth/2000.0f, (windowWidth / 2000.0f)* 3.5f);
	// Calculate visible bounds based on zoom
	float halfVisibleWidth = (windowWidth / Zoom) / 2.0f;
	float halfVisibleHeight = (windowHeight / Zoom) / 2.0f;

	// Background dimensions (2000x1404 assumed)
	float bgHalfWidth = 2000.0f / 2.0f;
	float bgHalfHeight = 1404.0f / 2.0f;

	// Calculate panning bounds
	float minPanX = (- bgHalfWidth + halfVisibleWidth)/1000.0f;
	float maxPanX = (bgHalfWidth - halfVisibleWidth) / 1000.0f;

	float minPanY = (- bgHalfHeight + halfVisibleHeight)/1000.0f;
	float maxPanY = (bgHalfHeight - halfVisibleHeight)/1000.0f;
	panX = std::clamp(panX, minPanX, maxPanX);
	// 2000/ 14004 = 1.42f
	panY = std::clamp(panY, minPanY*1.42f,maxPanY*1.42f);
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
		if ((*it)->MarkedForDelete)
		{
			it = GameObjects.erase(it);
		}
		else
			++it;
	}
}

void World::InputUpdate()
{
	ProcessInputGL(Window);
}

void World::SetupMouseCallbacks()
{
	// Mouse Interaction API setup
	MouseInteractionAPI* mouseAPI = new MouseInteractionAPI(Window,gridConfig);
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
	for (auto it = NewShaders.begin(); it < NewShaders.end(); it++)
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
	mous->ObjectShader->use();
	mous->draw();

	GameHUD->Update();
	glfwSwapBuffers(Window);
}









void World::Update(float DeltaSeconds)
{
	GarbageCollection();
	InputUpdate();
	RenderUpdate();
	glfwPollEvents();
	//CollisionUpdate();

}

bool World::IsRunning()
{
	return !glfwWindowShouldClose(Window);
}


///////// todo
// one shader cna be used with multiple purposes and different places , -> 1 shader for texture can draw multiple textures , shoyld 