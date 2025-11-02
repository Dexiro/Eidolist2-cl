#pragma once
#include "GLMIncludes.h"


namespace mathf
{
	static float DegToRad(float deg);

	template<typename T>
	bool in_range(const T& val, const T& min, const T& max)
	{
		return (val >= min && val <= max);
	}
	
	void screenToWorld(glm::vec2& obj, glm::vec2 scr, glm::vec2 wpos, glm::vec2 scale);

	void worldToScreen(glm::vec2& scr, glm::vec2 obj, glm::vec2 wpos, glm::vec2 scale);

}