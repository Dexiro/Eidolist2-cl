#include "MathFun.h"

#include <numbers>


float mathf::DegToRad(float deg)
{
	return deg * (std::numbers::pi / 180.0f);
}