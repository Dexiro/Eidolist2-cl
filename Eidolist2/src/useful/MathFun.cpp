#include "MathFun.h"

#include <numbers>


float mathf::DegToRad(float deg)
{
	return deg * (std::numbers::pi / 180.0f);
}

void mathf::screenToWorld(glm::vec2& obj, glm::vec2 scr, glm::vec2 wpos, glm::vec2 scale)
{
	obj.x = (scr.x / scale.x) - wpos.x;
	obj.y = (scr.y / scale.y) - wpos.y;
}

void mathf::worldToScreen(glm::vec2& scr, glm::vec2 obj, glm::vec2 wpos, glm::vec2 scale)
{
	scr.x = (wpos.x + obj.x) * scale.x;
	scr.y = (wpos.y + obj.y) * scale.y;
}
