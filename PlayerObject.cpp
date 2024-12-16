#include "PlayerObject.h"
#include "Math.h"

void PlayerObject::ConsumeInput(std::vector<float> InDir)
{
	ActiveInput = InDir;
	float InputSum = 0.0f;
	float Multiplier = .005f;
	// doom
	for (auto it = ActiveInput.begin(); it != ActiveInput.end(); ++it)InputSum += (*it) * (*it);
	InputSum = std::sqrt(InputSum);
	if (InputSum > 1)
	{
		ActiveInput[0] = ActiveInput[0] / InputSum;
		ActiveInput[1] = ActiveInput[1] / InputSum;

	}
	float dotProduct = 0.0f;
	std::vector<float> CrossProductRes = Math::CrossProduct(ForwardDir, ActiveInput);
	for (size_t i = 0; i < InDir.size(); ++i)
	{
		dotProduct += ActiveInput[i] * ForwardDir[i];
	}
	WorldTransform.Location[0] += ActiveInput[0] * Multiplier;
	WorldTransform.Location[1] += ActiveInput[1] * Multiplier;
	if (ActiveInput[0] != 0 || ActiveInput[1] != 0)
		WorldTransform.Rotation = acos(dotProduct) * (CrossProductRes[2] > 0 ? 1 : -1);
}
