#pragma once
#include <string>
#include "CommonIncludes.h"
#include "SDLTypes.h"
#include "MathFun.h"

namespace sdl
{
    //Texture wrapper class
    class Texture
    {
    public:
        //Initializes variables
        Texture();

        //Deallocates memory
        ~Texture();

        //Loads image at specified path
        bool LoadFromFile(SDLContext& context, std::string path, bool useAlpha = true);
        bool CreateEmpty(SDLContext& context, int w, int h, SDL_TextureAccess access);// SDL_PixelFormat* pixelFormat);// int depth, Uint32 format, Uint32 colorKey);
        //bool SetBlendMode();

        //Deallocates texture
        void Free();

        //Renders texture at given point
        void Render(SDLContext& context, glm::vec2 pos, glm::vec2 scale, SDL_FRect* clip = NULL);
        void Render(SDLContext& context, SDL_FRect* src, SDL_FRect* dst);

        void UpdateScaleMode(glm::vec2 scale);

        //Gets image dimensions
        SDL_Texture* GetTexture();
        int Width() const;
        int Height() const;
        int Depth() const;
        int ColorKey() const;
        SDL_PixelFormat Format() const;
        SDL_ScaleMode ScaleMode() const;

    private:
        //The actual hardware texture
        SDL_Texture* m_texture;

        //Image dimensions
        int m_width;
        int m_height;
        int m_depth;
        Uint32 m_colorKey;
        SDL_PixelFormat m_format;
        SDL_ScaleMode m_scaleMode;
    };

    //SDL_FRect ToFRect(glm::vec2 pos, glm::vec2 size);
}
using PTexture = std::shared_ptr<sdl::Texture>;