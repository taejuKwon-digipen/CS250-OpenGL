#include "hermiteCurve.h"

Vec3 HermiteCurves::EvaluatePiecewiseLinear(float t, unsigned count, const Vec3* positions, const float* times)
{
	// handle boundary conditions
	if (t <= times[0])
	{
		return positions[0];
	}
	else if (t >= times[count-1])
	{
		return positions[count - 1];
	}

	// find segment and parameter
	unsigned int i;
	for (i = 0; i < count-1; ++i)
	{
		if (t < times[i+1])
		{
			break;
		}
	}
	float t0 = times[i];
	float t1 = times[i + 1];
	float u = (t - t0) / (t1 - t0);

	// evaluate
	return (1 - u) * positions[i] + u * positions[i + 1];
}
