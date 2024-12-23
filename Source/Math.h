#pragma once
#include <vector>


class Math
{
public:
	static std::vector<float> CrossProduct(const std::vector<float>& Vec1, const std::vector<float>& Vec2);
	static float DotProduct(const std::vector<float>& Vec1, const std::vector<float>& Vec2);
	static void DivideArray(std::vector<float>& Vec1, const float Denominator);
	static float VectorSize(std::vector<float>& Vec);
};