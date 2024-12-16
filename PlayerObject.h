#pragma once
#include "GameObject.h"

class PlayerObject : public GameObject
{
protected:

public:
	void ConsumeInput(std::vector<float> InDir);
};