#pragma once
#include <vector>
#include "GameObject.h"
#include <memory>


class World
{
private:
	//
	std::vector<std::shared_ptr<GameObject>> GameObjects;
	std::shared_ptr<GameObject> Player;
	//
	std::vector<Shader> Shaders;
	//
	void InitBackground();
	void SpawnPlayer();
	void SpawnObjects();
public:
	std::weak_ptr<GameObject>  GetPlayer();
	World(std::vector<std::shared_ptr<GameObject>>& GameObjects);
	World();
	~World();
	void Update(float DeltaSeconds);
	void ProcessInput(std::vector<float> InDir);
};