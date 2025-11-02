#pragma once
#include <string>
#include "../CommonIncludes.h"
#include "../SDLTypes.h"
#include "../Transform.h"

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
using uPTexture = std::unique_ptr<sdl::Texture>;

struct SimpleObject
{
    std::vector<SimpleObject*> m_children;

    std::shared_ptr<sdl::Texture> m_texture;
    glm::vec2 m_pos = glm::vec2(0);
    glm::vec2 m_scale = glm::vec2(1);

    SDL_FRect clipRect = SDL_FRect(0.0f, 0.0f);

    glm::vec2 m_pivot = glm::vec2(0);

    virtual void Draw(SDLContext& context, glm::vec2 pos = glm::vec2(0), glm::vec2 scale = glm::vec2(1))
    {
        glm::vec2 targetPos = (m_pos + pos) - m_pivot;

        for (auto child : m_children)
        {
            child->Draw(context, targetPos, m_scale * scale);
        }

        if (m_texture)
            m_texture->Render(context, targetPos, m_scale * scale, &clipRect);
    }

    virtual void FixedUpdate(SDLContext& context, int frames, double dt)
    {

    }

    virtual void Reset()
    {
        for (auto child : m_children)
        {
            child->Reset();
        }

        if (m_texture)
            m_texture.reset();
    }

    void AddChild(SimpleObject* obj)
    {
        m_children.push_back(obj);
    }

    virtual SDL_FRect FRect(glm::vec2 pos = glm::vec2(0))
    {
        glm::vec2 targetPos = (m_pos + pos) - m_pivot;

        // This function needs testing before use, it's probably incomplete
        for (auto child : m_children)
        {
            child->FRect(targetPos);
        }

        return SDL_FRect{ targetPos.x, targetPos.y, (float)clipRect.w, (float)clipRect.h };
    }
};
using PBaseObject = std::shared_ptr<SimpleObject>;
