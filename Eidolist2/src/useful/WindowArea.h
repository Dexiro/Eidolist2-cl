#pragma once
#include "SDLTypes.h"
#include "Rect.h"

namespace warea
{
	enum EArea
	{
		DOCKLEFT
	};

	rect::FRect GetArea(SDLContext& context, glm::vec2 percent);
	rect::FRect GetArea(SDLContext& context, glm::vec2 origin, glm::vec2 size);


}