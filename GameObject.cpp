#include "GameObject.h"
#include <algorithm>
#include <cstdlib>


Transform GameObject::GetTransform()
{
	return WorldTransform;
}

void GameObject::SetTransform(Transform InTransform)
{
	WorldTransform = InTransform;
}

float GameObject::GetLength()
{
	return Length;
}

bool GameObject::GetIsHazard()
{
	return IsHazard;
}





