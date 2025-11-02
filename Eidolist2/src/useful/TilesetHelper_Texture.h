#pragma once
#include "SDLTypes.h"
#include "obj/BaseObj.h"
#include "TilesetHelper.h"

namespace tsh
{
	SDL_Rect ToRect(const glm::ivec2& pos, const glm::ivec2& size);
	SDL_FRect ToFRect(const glm::vec2& pos, const glm::vec2& size);

	namespace texture
	{
		void CompileTextureFromTileset(std::shared_ptr<sdl::Texture>& pChipsetTexture, std::vector<CopyRect>& copyRect, std::shared_ptr<sdl::Texture>& atTex, SDLContext& context, glm::ivec2 fixedSize = glm::ivec2(0));
	}
}


