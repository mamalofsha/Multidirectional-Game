#pragma once
#include <vector>
const double PI = 3.14159265358979323846;

class GameObject
{
public:
	std::vector<float> ForwardDir = { 1.0f, 0.0f };
	std::vector<float> Coordinate = { 0.0f, 0.0f };
	float Length = 0.1f;
	float Rotation =0.0f;
	GameObject();

	void ConsumeInput(std::vector<float> InDir);
};