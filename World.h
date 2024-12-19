#pragma once
#include <vector>
#include "GameObject.h"
#include <memory>
#include <GLFW/glfw3.h>
#include "MouseInteraction.h"
#include "Graphics.h"
#include <functional>
#include "MouseInteraction.h"

//MouseState mouseState;
enum class ShaderType { Background, Grid };
struct ShaderEntry {
	Shader shader;
	ShaderType type;
};

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
	void onHoverFunction(int gridX, int gridY);

	void onClickFunction(int gridX, int gridY);
	void SpawnPlayer();
	void SpawnObjects();
	void ProcessInputGL(GLFWwindow* window);
	void GarbageCollection();
	void InputUpdate();
	void SetupMouseCallbacks();
	void RenderUpdate();
	void CollisionUpdate();
	void HandleCollision(GameObject& GameObject1, GameObject& GameObject2);
public:
	std::weak_ptr<GameObject>  GetPlayer();
	World(std::vector<std::shared_ptr<GameObject>>& GameObjects);
	World(const unsigned int Width,const unsigned int Height);
	~World();
	void Update(float DeltaSeconds);
	void PassInput(std::vector<float> InDir);
	bool IsRunning();
	// Define callback functions

};