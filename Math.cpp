#include "Math.h"
#include <algorithm>
#include <cstdlib>
#include "cmath"

std::vector<float> Math::CrossProduct(const std::vector<float>& Vec1, const std::vector<float>& Vec2)
{
	std::vector<float> result(3);
	if(Vec1.size() > 2)
	{ 
	result[0] = Vec1[1] * Vec2[2] - Vec1[2] * Vec2[1];
	result[1] = Vec1[2] * Vec2[0] - Vec1[0] * Vec2[2];
	}
	else
	{
		result[0] = 0;
		result[1] = 0;
	}
	result[2] = Vec1[0] * Vec2[1] - Vec1[1] * Vec2[0];
	return result;
}

float Math::DotProduct(const std::vector<float>& Vec1, const std::vector<float>& Vec2)
{
	float Result = 0.f;
	if (Vec1.size() != Vec2.size()) return Result;
	for (size_t i = 0; i < Vec1.size(); ++i)
	{
		Result += Vec1[i] * Vec2[i];
	}
	return Result;
}

void Math::DivideArray(std::vector<float>& Vec1, const float Denominator)
{
	for (auto it = Vec1.begin(); it != Vec1.end(); ++it)
	{
		*it /= Denominator;
	}
}

float Math::VectorSize(std::vector<float>& Vec)
{
	float Sum = 0.f;
	for (auto it = Vec.begin(); it != Vec.end(); ++it) Sum += (*it) * (*it);
	Sum = std::sqrt(Sum);
	return Sum;
}
