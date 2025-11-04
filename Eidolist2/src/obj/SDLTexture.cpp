#include "SDLTexture.h"

namespace sdl
{
	Texture::Texture()
	{
		//Initialize
		m_texture = NULL;
		m_width = 0;
		m_height = 0;
		m_depth = 0;
		m_colorKey = 0;
		m_format = SDL_PixelFormat::SDL_PIXELFORMAT_RGBA32;
		m_scaleMode = SDL_SCALEMODE_PIXELART;
	}

	Texture::~Texture()
	{
		//Deallocate
		Free();
	}

	bool Texture::LoadFromFile(SDLContext& context, std::string path, bool useAlpha)
	{
		//Get rid of preexisting texture
		Free();

		//The final texture
		SDL_Texture* newTexture = NULL;

		//Load image at specified path
		if (SDL_Surface* srf = IMG_Load(path.c_str()); srf == nullptr)
		{
			printf("Unable to load image %s! SDL_image Error: %s\n", path.c_str(), SDL_GetError());
		}
		else
		{
			//m_depth = srf->;
			m_width = srf->w;
			m_height = srf->h;
			m_format = (SDL_PixelFormat)srf->format;

			if (useAlpha)
			{
				if (m_format == SDL_PixelFormat::SDL_PIXELFORMAT_INDEX8)
				{
					SDL_Palette* palette = SDL_GetSurfacePalette(srf);
					if (palette && palette->ncolors > 0)
					{
						const SDL_PixelFormatDetails* details = SDL_GetPixelFormatDetails(m_format);
						SDL_Color color = palette->colors[0];
						m_colorKey = SDL_MapRGB(details, palette, color.r, color.g, color.b);
						SDL_SetSurfaceColorKey(srf, true, m_colorKey);
					}
				}
				else
				{
					//Color key image
					m_colorKey = SDL_MapRGB(SDL_GetPixelFormatDetails(m_format), nullptr, 0, 0xFF, 0xFF);
					SDL_SetSurfaceColorKey(srf, true, m_colorKey);
				}
			}
			//SDL_GetColorKey(srf, &m_colorKey);

			//Create texture from surface pixels
			m_texture = SDL_CreateTextureFromSurface(context.Renderer, srf);
			if (m_texture == NULL)
			{
				printf("Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError());
			}

			m_scaleMode = SDL_SCALEMODE_PIXELART;
			SDL_SetTextureScaleMode(m_texture, m_scaleMode);
			SDL_SetTextureBlendMode(m_texture, SDL_BLENDMODE_BLEND);

			//Get rid of old loaded surface
			SDL_DestroySurface(srf);
		}

		//Return success
		return m_texture != nullptr;
	}

	bool Texture::CreateEmpty(SDLContext& context, int w, int h, SDL_TextureAccess access)// SDL_PixelFormat* pixelFormat)
	{
		m_texture = SDL_CreateTexture(context.Renderer, SDL_GetWindowPixelFormat(context.Window), access, w, h);

		//m_depth = depth;
		m_width = w;
		m_height = h;
		m_format = (SDL_PixelFormat)SDL_GetWindowPixelFormat(context.Window);
		SDL_SetTextureBlendMode(m_texture, SDL_BLENDMODE_BLEND);
		m_scaleMode = SDL_SCALEMODE_PIXELART;
		SDL_SetTextureScaleMode(m_texture, m_scaleMode);

		return false;
	}

	void Texture::Free()
	{
		//Free texture if it exists
		if (m_texture != NULL)
		{
			SDL_DestroyTexture(m_texture);
			m_texture = NULL;
			m_width = 0;
			m_height = 0;
		}
	}

	void Texture::Render(SDLContext& context, glm::vec2 pos, glm::vec2 scale, SDL_FRect* clip)
	{
		pos *= scale;

		//Set rendering space and render to screen
		glm::vec2 dstScale = (clip == NULL) ? glm::vec2(m_width, m_height) : glm::vec2(clip->w, clip->h);
		dstScale *= scale;

		SDL_FRect renderQuad = { pos.x, pos.y, dstScale.x, dstScale.y };

		UpdateScaleMode(scale);

		//Render to screen
		//SDL_RenderSetScale(context.Renderer, scale.x, scale.y);
		SDL_RenderTexture(context.Renderer, m_texture, clip, &renderQuad);
	}

	void Texture::Render(SDLContext& context, SDL_FRect* src, SDL_FRect* dst)
	{
		//Render to screen
		SDL_RenderTexture(context.Renderer, m_texture, src, dst);
	}

	void Texture::UpdateScaleMode(glm::vec2 scale)
	{
		if (scale.x < 1.0f)
		{
			if (m_scaleMode != SDL_SCALEMODE_LINEAR)
			{
				m_scaleMode = SDL_SCALEMODE_LINEAR;
				SDL_SetTextureScaleMode(m_texture, m_scaleMode);
			}
		}
		else
		{
			if (m_scaleMode != SDL_SCALEMODE_PIXELART)
			{
				m_scaleMode = SDL_SCALEMODE_PIXELART;
				SDL_SetTextureScaleMode(m_texture, m_scaleMode);
			}
		}
	}

	SDL_Texture* Texture::GetTexture()
	{
		return m_texture;
	}

	int Texture::Width() const
	{
		return m_width;
	}

	int Texture::Height() const
	{
		return m_height;
	}

	int Texture::Depth() const
	{
		return m_depth;
	}

	int Texture::ColorKey() const
	{
		return m_colorKey;
	}

	SDL_PixelFormat Texture::Format() const
	{
		return m_format;
	}

	SDL_ScaleMode Texture::ScaleMode() const
	{
		return m_scaleMode;
	}

	//SDL_FRect ToFRect(glm::vec2 pos, glm::vec2 size)
	//{
	//	return SDL_FRect{ pos.x, pos.y, size.x, size.y };
	//}
}
