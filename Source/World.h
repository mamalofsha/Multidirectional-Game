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
	GridConfig GridConfigData;
	// 
	float PanX = 0.0f;
	float PanY = 0.0f;
	float ZoomLevel = 0.25f;
	//
	std::vector<std::shared_ptr<Object>> GameObjects;
	std::map<unsigned int, std::vector<std::shared_ptr<Object>>> ObjectRenderMap;
	//
	std::vector<std::shared_ptr<Shader>> Shaders;
	Shader GridShader;
	Shader BackgroundShader;
	Shader FloatingShader;
	//
	MouseInteractionAPI* MouseAPI;

	void InitBackground();
	void InitHUD();
	void InitGrid(const std::string& InFileName);
	void ProcessInputGL(GLFWwindow* InWindow);
	void GarbageCollection();
	void SetupMouseCallbacks();
	void RenderUpdate();
	void LoadSave();
public:
	std::shared_ptr<Shader> BuildingShader;
	std::vector<std::unique_ptr<class Building>> Buildings;
	std::unique_ptr<HUD> GameHUD;

	World(const std::string& InFileName);
	~World();
	void Update(float InDeltaSeconds);
	bool IsRunning();
	std::pair<int, int> GetWindowSize();
	std::pair<float, float> GetPan();
	std::pair<float, float> GetLevelSize();
	float GetZoom() { return ZoomLevel; };
	GLFWwindow* GetWindow() { return Window; };
	MouseState GetMouseState() { return MouseAPI->GetMouseState(); };
	GridConfig GetGridConfig() { return GridConfigData; };
	StartUpData GetStartupData() { return StartUp; };
};
