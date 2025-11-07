#pragma once
#include "SDLTypes.h"
#include "GLMIncludes.h"

// Helpful SDL Functions

namespace sdlf
{
	void DrawArrow(SDLContext& context, glm::vec2 start, glm::vec2 end, bool dotted);
}