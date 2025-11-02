#include "SDLFun.h"



static void sdlf::DrawArrow(SDLContext& context, glm::vec2 start, glm::vec2 end, bool dotted)
{
	glm::vec2 between = end - start;
	glm::vec2 normal = glm::normalize(between);
	glm::vec2 r1 = end + (glm::rotate(normal, DegToRad(-150.0f)) * 10.0f);
	glm::vec2 r2 = end + (glm::rotate(normal, DegToRad(150.0f)) * 10.0f);

	float len = glm::length(between);
	if (dotted)
	{
		float dashLen = 10.0f;
		glm::vec2 ndash = normal * dashLen;
		glm::vec2 c1 = start;
		glm::vec2 c2;
		int nCount = (int)(len / (dashLen * 2));
		for (int i = 0; i < nCount; i++)
		{
			c2 = c1 + ndash;
			SDL_RenderLine(context.Renderer, c1.x, c1.y, c2.x, c2.y);
			c1 = c2 + ndash;
		}
		c2 = end - (ndash * 2.0f);
		SDL_RenderLine(context.Renderer, end.x, end.y, c2.x, c2.y);
	}
	else
	{
		SDL_RenderLine(context.Renderer, start.x, start.y, end.x, end.y);
	}
	SDL_RenderLine(context.Renderer, end.x, end.y, r1.x, r1.y);
	SDL_RenderLine(context.Renderer, end.x, end.y, r2.x, r2.y);
}