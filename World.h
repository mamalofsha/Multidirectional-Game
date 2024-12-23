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

class World
{
private:

	MouseState mouseState;
	GridConfig gridConfig;
	GLFWwindow* Window;
	// Add panning variables
	float panX = 0.0f;
	float panY = 0.0f;
	float Zoom = 0.25f;
	//
	std::vector<std::shared_ptr<Object>> GameObjects;
	std::map<unsigned int, std::vector<std::shared_ptr<Object>>> objectRenderMap;
	std::shared_ptr<class PlayerObject> Player;
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
	float GetZoom() { return Zoom; };
	float GetPanX() { return panX; };
	float GetPanY() { return panY; };
	GridConfig GetGridConfig() { return gridConfig; };
	MouseState GetMouserState() { return mouseState; };
	GLFWwindow* GetWindow() { return Window; };
	std::pair<int, int> GetWindowSize();
	World(std::vector<std::shared_ptr<GameObject>>& GameObjects);
	World(const std::string& InFileName);
	~World();
	void Update(float DeltaSeconds);
	bool IsRunning();
};