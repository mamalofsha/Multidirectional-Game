#pragma once
#include <vector>
#include "Graphics.h"
#include <memory>
#include <functional>
#include "MouseInteraction.h"
#include "XMLparser.h"

class Object;
class MouseInteractionAPI;
class Building;
class HUD;

class World
{
private:
	GLFWwindow* Window;
	StartUpData StartUp;
	GridConfig GridConfigData;
	bool IsGridDrawn = true;
	// use arrow keys to pan around 
	float PanX = 0.0f;
	float PanY = 0.0f;
	// works with W/S keys
	float ZoomLevel = 0.25f;
	// used with magnifier buttons
	int MagnifierZoomLevel = 1;
	//
	std::map<unsigned int, std::vector<std::unique_ptr<Object>>> ObjectRenderMap;
	//
	std::vector<std::shared_ptr<Shader>> Shaders;
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
	std::vector<std::unique_ptr<Building>> Buildings;
	std::unique_ptr<HUD> GameHUD;
	float DeltaSeconds;

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
	void DeleteBuilding(int InGridX, int InGridY);
	void ChangeMagnifierZoom(int InDelta);
	void ToggleGridDraw() { IsGridDrawn = !IsGridDrawn; };
	bool GetGridDraw() { return IsGridDrawn; };
};
