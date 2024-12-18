#include "PlayerObject.h"
#include "Math.h"

void PlayerObject::ConsumeInput(std::vector<float> InDir)
{
	ActiveInput = InDir;
	//speed
	float Multiplier = .005f;
	// doom
	float VectorSize = Math::VectorSize(ActiveInput);
	if (VectorSize > 1)
	{
		/// replace with transform todo
		Math::DivideArray(ActiveInput, VectorSize);
	}
	float DotProduct = Math::DotProduct(ActiveInput, ForwardDir);
	std::vector<float> CrossProductRes = Math::CrossProduct(ForwardDir, ActiveInput);
	WorldTransform.Location[0] += ActiveInput[0] * Multiplier;
	WorldTransform.Location[1] += ActiveInput[1] * Multiplier;
	if (ActiveInput[0] != 0 || ActiveInput[1] != 0)
		WorldTransform.Rotation = acos(DotProduct) * (CrossProductRes[2] > 0 ? 1 : -1);
}
