#include "GameObject.h"
#include <algorithm>
#include <cstdlib>

GameObject::GameObject()
{
}

Transform GameObject::GetTransform()
{
	return WorldTransform;
}

float GameObject::GetLength()
{
	return Length;
}



