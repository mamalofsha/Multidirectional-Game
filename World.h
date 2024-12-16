#pragma once
#include <vector>
#include "GameObject.h"
#include <memory>
#include <GLFW/glfw3.h>


class World
{
private:
	GLFWwindow* Window;
	//
	std::vector<std::shared_ptr<GameObject>> GameObjects;
	std::shared_ptr<GameObject> Player;
	//
	std::vector<Shader> Shaders;
	//
	void InitBackground();
	void SpawnPlayer();
	void SpawnObjects();
	void ProcessInputGL(GLFWwindow* window);
	void InputUpdate();
	void RenderUpdate();
public:
	std::weak_ptr<GameObject>  GetPlayer();
	World(std::vector<std::shared_ptr<GameObject>>& GameObjects);
	World(const unsigned int Width,const unsigned int Height);
	~World();
	void Update(float DeltaSeconds);
	void PassInput(std::vector<float> InDir);
	bool IsRunning();
};