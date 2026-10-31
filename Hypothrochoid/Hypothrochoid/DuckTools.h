#pragma once

#include "raylib.h"
#include "raymath.h"

inline Vector2 PolarToVector(float dist, float angle)
{
	return Vector2
	{
		dist * (float)cos(DEG2RAD * angle),
		dist * (float)sin(DEG2RAD * angle)
	};
}
