#pragma once
#include "math.hpp"

class HermiteCurves {
	 Vec3 EvaluatePiecewiseLinear(float t, unsigned int count, const Vec3* positions, const float* times);
};