#pragma once
#include <vector>
#include "Shader.h"

struct Transform
{
	std::vector<float> Location = {0.f,0.f};
	float Rotation = 0.f;
};

class GameObject
{
protected :
	std::vector<float> ForwardDir = { 1.0f, 0.0f };
	std::vector<float> ActiveInput = { 0.f,0.f };
	Transform WorldTransform;
	float Length = 0.1f;
	bool IsHazard = false;

public:
	std::unique_ptr<Shader> ObjectShader;
	bool MarkedForDelete = false;

	GameObject();
	GameObject(Transform SpawnTransform,bool InHaz);
	Transform GetTransform();
	void SetTransform(Transform InTransform);
	float GetLength();
	bool GetIsHazard();

};