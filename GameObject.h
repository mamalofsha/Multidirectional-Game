#pragma once
#include <vector>
#include "Shader.h"
const double PI = 3.14159265358979323846;

struct Transform
{
	std::vector<float> Location = {0.f,0.f};
	float Rotation = 0.f;
};

class GameObject
{
private :
public:
	std::vector<float> ForwardDir = { 1.0f, 0.0f };
	Transform WorldTransform;
//	std::vector<float> Coordinate = { 0.0f, 0.0f };
	float Length = 0.1f;
	Shader* ObjectShader;
	std::vector<float> ActiveInput = {0.f,0.f};

	GameObject();

	void ConsumeInput(std::vector<float> InDir);
	std::vector<float> CrossProduct(const std::vector<float>& vec1, const std::vector<float>& vec2);
};