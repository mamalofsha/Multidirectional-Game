#pragma once
#include <vector>
//#include "GameObject.h"
#include "Shader.h"
#include <memory>
#include <GLFW/glfw3.h>
#include "MouseInteraction.h"
#include "Graphics.h"
#include <functional>
#include "MouseInteraction.h"
#include "UIButton.h"
#include "HUD.h"
#include "XMLparser.h"
class World
{
private:
	GLFWwindow* Window;
	StartUpData StartUp;
	MouseState mouseState;
	GridConfig gridConfig;
	// 
	float panX = 0.0f;
	float panY = 0.0f;
	float Zoom = 0.25f;
	//
	std::vector<std::shared_ptr<Object>> GameObjects;
	std::map<unsigned int, std::vector<std::shared_ptr<Object>>> objectRenderMap;
	//
	std::vector<std::shared_ptr<Shader>> Shaders;
	Shader GridShader;
	Shader BackGround;
	Shader floating;
	//

	void InitBackground();
	void InitHUD();
	void InitGrid(const std::string& InFileName);
	void ProcessInputGL(GLFWwindow* window);
	void GarbageCollection();
	void InputUpdate();
	void SetupMouseCallbacks();
	void RenderUpdate();
	void LoadSave();
public:
	std::shared_ptr<Shader> Buildingshader;
	std::vector<std::unique_ptr<class Building>> builds;
	std::unique_ptr<HUD> GameHUD;

	World(const std::string& InFileName);
	~World();
	void Update(float DeltaSeconds);
	bool IsRunning();
	std::pair<int, int> GetWindowSize();
	std::pair<float, float> GetPan();
	std::pair<float, float> GetLevelSize();
	float GetZoom() { return Zoom; };
	GLFWwindow* GetWindow() { return Window; };
	MouseState GetMouserState() { return mouseState; };
	GridConfig GetGridConfig() { return gridConfig; };
};