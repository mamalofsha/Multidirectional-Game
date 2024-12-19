#pragma once
#include <vector>
#include "GameObject.h"
#include <memory>
#include <GLFW/glfw3.h>
#include "MouseInteraction.h"
#include "Graphics.h"
#include <functional>
#include "MouseInteraction.h"



class World
{
private:
	MouseState mouseState;
	GridConfig gridConfig;
	GLFWwindow* Window;
	//
	std::vector<std::shared_ptr<GameObject>> GameObjects;
	std::shared_ptr<class PlayerObject> Player;
	//
	std::vector<ShaderEntry> Shaders;
	Shader GridShader;
	Shader BackGround;
	//
	void InitBackground();
	void InitGrid();
	static void onHoverFunction(int gridX, int gridY);
	static void onClickFunction(int gridX, int gridY);
	static void CursorCallback(GLFWwindow* window, double xpos, double ypos);
	void ProcessInputGL(GLFWwindow* window);
	void GarbageCollection();
	void InputUpdate();
	void SetupMouseCallbacks();
	void RenderUpdate();
	void CollisionUpdate();
	void HandleCollision(GameObject& GameObject1, GameObject& GameObject2);
public:

	World(std::vector<std::shared_ptr<GameObject>>& GameObjects);
	World(const unsigned int Width, const unsigned int Height);
	~World();
	std::weak_ptr<GameObject>  GetPlayer();
	void Update(float DeltaSeconds);
	void PassInput(std::vector<float> InDir);
	bool IsRunning();
};