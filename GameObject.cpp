#include "GameObject.h"
#include <algorithm>
#include <cstdlib>

GameObject::GameObject()
{
}

GameObject::GameObject(Transform SpawnTransform, bool InHaz)
{
	WorldTransform = SpawnTransform;
	IsHazard = InHaz;
}

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





