#include "GameObject.h"
#include <algorithm>
#include <cstdlib>
GameObject::GameObject()
{
}

// not pass by reference , because the input is hardcoded from the other side 
void GameObject::ConsumeInput(std::vector<float> InDir)
{
	//std::transform(ActiveInput.begin(), ActiveInput.end(), InDir.begin(), InDir.end(), std::plus<float>());
	//ActiveInput[0] += std::abs(ActiveInput[0])>=1? (std::signbit(InDir[0]) == std::signbit(ActiveInput[0]) ? 0 : InDir[0]):InDir[0];
	//ActiveInput[1] += std::abs(ActiveInput[1])>=1? (std::signbit(InDir[1]) == std::signbit(ActiveInput[1]) ? 0 : InDir[1]) :InDir[1];
	ActiveInput = InDir;
	float InputSum = 0.0f;
	float Multiplier = .005f;
	// doom
	for (auto it = ActiveInput.begin(); it != ActiveInput.end(); ++it)InputSum += (*it)*(*it);
	InputSum = std::sqrt(InputSum);
	if (InputSum > 1)
	{
		ActiveInput[0] = ActiveInput[0] / InputSum;
		ActiveInput[1] = ActiveInput[1] / InputSum;

	}
	float dotProduct = 0.0f; 
	std::vector<float> CrossProductRes = CrossProduct(ForwardDir, ActiveInput);
	for (size_t i = 0; i < InDir.size(); ++i)
	{
		dotProduct += ActiveInput[i]* ForwardDir[i];
	}
	WorldTransform.Location[0] += ActiveInput[0] *Multiplier;
	WorldTransform.Location[1] += ActiveInput[1] *Multiplier;
	if (ActiveInput[0] != 0 || ActiveInput[1] != 0)
		WorldTransform.Rotation = acos(dotProduct) * (CrossProductRes[2] > 0 ? 1 : -1);

}

std::vector<float> GameObject::CrossProduct(const std::vector<float>& vec1, const std::vector<float>& vec2)
{


	std::vector<float> result(3);

	result[0] = vec1[1] * 0 - 0 * vec2[1];
	result[1] = 0 * vec2[0] - vec1[0] * 0;
	result[2] = vec1[0] * vec2[1] - vec1[1] * vec2[0];

	return result;
}



