#include "Math.h"

std::vector<float> Math::CrossProduct(const std::vector<float>& vec1, const std::vector<float>& vec2)
{
	std::vector<float> result(3);
	result[0] = vec1[1] * 0 - 0 * vec2[1];
	result[1] = 0 * vec2[0] - vec1[0] * 0;
	result[2] = vec1[0] * vec2[1] - vec1[1] * vec2[0];
	return result;
}
