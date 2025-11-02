#include "WindowArea.h"

rect::FRect warea::GetArea(SDLContext& context, glm::vec2 percent)
{
	int winW, winH;
	SDL_GetWindowSizeInPixels(context.Window, &winW, &winH);
	rect::FRect rect = rect::FRect{ 0.0f, 0.0f, winW * percent.x, winH * percent.y };
	rect.h -= 18.0f;
	rect.y += 18.0f;
	return rect;
}

rect::FRect warea::GetArea(SDLContext& context, glm::vec2 origin, glm::vec2 size)
{
	int winW, winH;
	SDL_GetWindowSizeInPixels(context.Window, &winW, &winH);
	rect::FRect rect = rect::FRect{ origin.x * winW, origin.y * winH, winW * size.x, winH * size.y };
	rect.h -= 18.0f;
	rect.y += 18.0f;
	return rect;
}
;