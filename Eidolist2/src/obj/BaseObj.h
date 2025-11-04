#pragma once
#include "SDLTexture.h"



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
