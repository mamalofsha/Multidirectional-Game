#include "GameObject.h"

GameObject::GameObject()
{
}

// not pass by reference , because the input is hardcoded from the other side 
void GameObject::ConsumeInput(std::vector<float> InDir)
{
	float InputSum = 0.0f;
	float Multiplier = .01f;
	// doom
	for (auto it = InDir.begin(); it != InDir.end(); ++it)InputSum += (*it);
	if (InputSum > 1)
	{
		Multiplier *= 1.0f / InputSum;
	}
	float dotProduct = 0.0f; 
	for (size_t i = 0; i < InDir.size(); ++i)
	{
		dotProduct += InDir[i] * ForwardDir[i];
	}
	bool Forward = InDir[1] > 0;
	Rotation = acos(dotProduct) * (Forward?1:-1);
	Coordinate[0] += InDir[0]*Multiplier;
	Coordinate[1] += InDir[1]*Multiplier;
}
