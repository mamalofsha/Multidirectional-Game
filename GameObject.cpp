#include "GameObject.h"
#include <algorithm>
#include <cstdlib>
GameObject::GameObject()
{
}

// not pass by reference , because the input is hardcoded from the other side 
void GameObject::ConsumeInput(std::vector<float> InDir)
{
	HadInput = true;
	//std::transform(ActiveInput.begin(), ActiveInput.end(), InDir.begin(), InDir.end(), std::plus<float>());
	ActiveInput[0] += std::abs(ActiveInput[0])>=1? (std::signbit(InDir[0]) == std::signbit(ActiveInput[0]) ? 0 : InDir[0]):InDir[0];
	ActiveInput[1] += std::abs(ActiveInput[1])>=1? (std::signbit(InDir[1]) == std::signbit(ActiveInput[1]) ? 0 : InDir[1]) :InDir[1];

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
	WorldTransform.Location[0] += InDir[0]*Multiplier;
	WorldTransform.Location[1] += InDir[1]*Multiplier;
	WorldTransform.Rotation = acos(dotProduct) * (Forward ? 1 : -1);
}

void GameObject::ClearInput()
{
	if (HadInput) HadInput = false;
	else ActiveInput[0] = 0; ActiveInput[1] = 0;
}
