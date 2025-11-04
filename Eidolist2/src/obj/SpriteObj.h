#pragma once
#include "json.hpp"
#include "SDLTypes.h"
#include "Rect.h"
#include "BaseObj.h"
#include "Spritesheet.h"
using json = nlohmann::json;


struct SpriteObj : public SimpleObject
{
	SpriteObj() {};

	void SetSprite(const Spritesheet& spritesheet, std::string spriteTag, bool tiled = false)
	{
		m_tiled = tiled;
		m_texture = spritesheet.m_texture;
		m_bounds = spritesheet.m_spriteMap.at(spriteTag);
		m_dstRect = { 0.0f, 0.0f, m_bounds.bounds.w, m_bounds.bounds.h };
	}

	void SetTexture(std::shared_ptr<sdl::Texture> texture)
	{
		m_texture = texture;
		m_bounds.bounds = { 0.0f, 0.0f, (float)texture->Width(), (float)texture->Height() };
		m_dstRect = { 0.0f, 0.0f, m_bounds.bounds.w, m_bounds.bounds.h };
	}

	SpriteBounds m_bounds;
	rect::FRect m_dstRect;
	bool m_tiled = false;

	virtual void Draw(SDLContext& context, glm::vec2 pos = glm::vec2(0), glm::vec2 scale = glm::vec2(1))
	{
		for (auto child : m_children)
		{
			child->Draw(context, m_pos + pos, m_scale * scale);
		}

		if (m_texture)
		{
			if (!m_bounds.nine_slice)
			{
				if(m_tiled)
					_DrawTiled(context, pos, scale);
				else
					_Draw(context, pos, scale);
			}
			else
			{
				if (m_tiled)
					_Draw9GridTiled(context, pos, scale);
				else
					_Draw9Grid(context, pos, scale);
			}
		}
	}

protected:

	void _Draw(SDLContext& context, glm::vec2 pos = glm::vec2(0), glm::vec2 scale = glm::vec2(1))
	{
		SDL_FRect src = m_bounds.bounds.SDLRect();
		SDL_FRect dst = m_dstRect.SDLRect();
		dst.x += ((m_pos.x + pos.x) - m_bounds.pivot.x) * scale.x;
		dst.y += ((m_pos.y + pos.y) - m_bounds.pivot.y) * scale.y;
		dst.w *= scale.x;
		dst.h *= scale.y;

		m_texture->UpdateScaleMode(scale);
		SDL_RenderTexture(context.Renderer, m_texture->GetTexture(), &src, &dst);
	}

	void _Draw9Grid(SDLContext& context, glm::vec2 pos = glm::vec2(0), glm::vec2 scale = glm::vec2(1))
	{
		SDL_FRect src = m_bounds.bounds.SDLRect();
		SDL_FRect dst = m_dstRect.SDLRect();

		m_texture->UpdateScaleMode(scale);
		SDL_RenderTexture9Grid(context.Renderer, m_texture->GetTexture(), &src, m_bounds.left_width, m_bounds.right_width, m_bounds.top_height, m_bounds.btm_height, 0.0f, &dst);
	}

	void _DrawTiled(SDLContext& context, glm::vec2 pos = glm::vec2(0), glm::vec2 scale = glm::vec2(1))
	{
		SDL_FRect src = m_bounds.bounds.SDLRect();
		SDL_FRect dst = m_dstRect.SDLRect();
		dst.x += ((m_pos.x + pos.x) - m_bounds.pivot.x) * scale.x;
		dst.y += ((m_pos.y + pos.y) - m_bounds.pivot.y) * scale.y;
		dst.w *= scale.x;
		dst.h *= scale.y;

		m_texture->UpdateScaleMode(scale);
		SDL_RenderTextureTiled(context.Renderer, m_texture->GetTexture(), &src, 1.0f, &dst);
	}

	void _Draw9GridTiled(SDLContext& context, glm::vec2 pos = glm::vec2(0), glm::vec2 scale = glm::vec2(1))
	{
		SDL_FRect src = m_bounds.bounds.SDLRect();
		SDL_FRect dst = m_dstRect.SDLRect();

		m_texture->UpdateScaleMode(scale);
		SDL_RenderTexture9GridTiled(context.Renderer, m_texture->GetTexture(), &src, m_bounds.left_width, m_bounds.right_width, m_bounds.top_height, m_bounds.btm_height, 0.0f, &dst, 1.0f);
	}

};

struct SpriteTextObj : public SpriteObj
{
	std::string text;

	virtual void Draw(SDLContext& context, glm::vec2 pos = glm::vec2(0), glm::vec2 scale = glm::vec2(1))
	{
		for (auto child : m_children)
		{
			child->Draw(context, m_pos + pos, m_scale * scale);
		}

		if (m_texture)
		{
			_Draw(context, pos, scale);
		}
	}
};

struct ParallaxObj : public SpriteObj
{
	bool m_bLoopX = false;
	bool m_bLoopY = false;
	glm::vec2 m_scrollOffset = glm::vec2(0.0f);

	void ResizeTexture(SDLContext& context, glm::ivec2 mapSize)
	{
		m_dstRect = { 0.0f, 0.0f, (float)mapSize.x, (float)mapSize.y };
		if (!m_bLoopX && !m_bLoopY)
			return;

		glm::ivec2 panoSize = { m_texture->Width(), m_texture->Height() };
		glm::ivec2 newSize = mapSize;
		if (m_bLoopX)
			newSize.x += panoSize.x;
		if (m_bLoopY)
			newSize.y += panoSize.y;

		std::shared_ptr<sdl::Texture> newTexture = std::make_shared<sdl::Texture>();
		newTexture->CreateEmpty(context, newSize.x, newSize.y, SDL_TextureAccess::SDL_TEXTUREACCESS_TARGET);

		SDL_SetRenderTarget(context.Renderer, newTexture->GetTexture());
		SDL_RenderTextureTiled(context.Renderer, m_texture->GetTexture(), NULL, 1.0f, NULL);
		SDL_SetRenderTarget(context.Renderer, NULL);

		m_texture.reset();
		m_texture = newTexture;
	}


	virtual void Draw(SDLContext& context, glm::vec2 pos = glm::vec2(0), glm::vec2 scale = glm::vec2(1))
	{
		for (auto child : m_children)
		{
			child->Draw(context, m_pos + pos, m_scale * scale);
		}

		if (m_texture)
		{
			glm::vec2 targetPos = pos + m_scrollOffset;
			targetPos /= 2.0f;
			
			SDL_FRect src = m_bounds.bounds.SDLRect();
			if(m_bLoopX)
				src.x = (float)(((int)(targetPos.x) + m_texture->Width()) % (int)m_bounds.bounds.w);
			if(m_bLoopY)
				src.y = (float)(((int)(targetPos.y) + m_texture->Height()) % (int)m_bounds.bounds.h);
			src.w = (float)m_texture->Width();
			src.h = (float)m_texture->Height();
			SDL_FRect dst = m_dstRect.SDLRect();
			dst.x += ((m_pos.x + pos.x) - m_bounds.pivot.x) * scale.x;
			dst.y += ((m_pos.y + pos.y) - m_bounds.pivot.y) * scale.y;
			dst.w *= scale.x;
			dst.h *= scale.y;

			m_texture->UpdateScaleMode(scale);
			SDL_RenderTextureTiled(context.Renderer, m_texture->GetTexture(), &src, scale.x, &dst);
		}
	}

};