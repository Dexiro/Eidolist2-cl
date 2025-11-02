#include "TilesetHelper_Texture.h"




void tsh::texture::CompileTextureFromTileset(std::shared_ptr<sdl::Texture>& pTilesetTexture, std::vector<CopyRect>& copyRect, std::shared_ptr<sdl::Texture>& newTexture, SDLContext& context, glm::ivec2 fixedSize)
{
	SDL_Rect tsRect = { 0, 0, pTilesetTexture->Width(), pTilesetTexture->Height() };

	// Calculate the size of the new texture
	SDL_Rect rectA = tsRect;
	if (fixedSize.x <= 0 || fixedSize.y <= 0)
	{
		for (auto& cpr : copyRect)
		{
			SDL_Rect rectB = { cpr.dst_pos.x, cpr.dst_pos.y, cpr.dst_size.x, cpr.dst_size.y };
			SDL_GetRectUnion(&rectA, &rectB, &rectA);
		}
	}
	else
	{
		rectA = { 0, 0, fixedSize.x, fixedSize.y };
	}

	// Create the new texture
	newTexture->CreateEmpty(context, rectA.w, rectA.h, SDL_TextureAccess::SDL_TEXTUREACCESS_TARGET);

	// Set the new texture as the render target
	SDL_SetRenderTarget(context.Renderer, newTexture->GetTexture());

	// Copy all of the extra tile parts to the new texture
	for (auto& cpr : copyRect)
	{
		SDL_FRect srcRect = ToFRect(cpr.src_pos, cpr.src_size);
		SDL_FRect dstRect = ToFRect(cpr.dst_pos, cpr.dst_size);
		SDL_RenderTexture(context.Renderer, pTilesetTexture->GetTexture(), &srcRect, &dstRect);
	}

	// Reset the render target
	SDL_SetRenderTarget(context.Renderer, NULL);
}

SDL_Rect tsh::ToRect(const glm::ivec2& pos, const glm::ivec2& size)
{
	return SDL_Rect{ pos.x, pos.y, size.x, size.y };
}

SDL_FRect tsh::ToFRect(const glm::vec2& pos, const glm::vec2& size)
{
	return SDL_FRect{ pos.x, pos.y, size.x, size.y };
}

