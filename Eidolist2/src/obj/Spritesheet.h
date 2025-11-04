#pragma once
#include "json.hpp"
#include "SDLTypes.h"
#include "Rect.h"
#include "BaseObj.h"
using json = nlohmann::json;

enum ESlice
{
	TL, T, TR,
	L, M, R,
	BL, B, BR,
	Full
};

namespace glm
{
	void to_json(json& j, const vec2& p);
	void from_json(const json& j, vec2& p);
}
struct SpriteBounds
{
	rect::FRect bounds;
	glm::vec2 pivot = glm::vec2(0.0f);

	// Nine-Slice Data
	rect::FRect center;
	bool nine_slice = false;
	int left_width = 0;
	int right_width = 0;
	int top_height = 0;
	int btm_height = 0;
};

struct Spritesheet
{
	Spritesheet() {};
	~Spritesheet() {};

	std::shared_ptr<sdl::Texture> m_texture;
	json m_json;

	std::map<std::string, SpriteBounds> m_spriteMap;

	bool LoadSpritesheet(SDLContext& context, std::string texturePath, std::string jsonPath)
	{
		m_texture = std::make_shared<sdl::Texture>();
		m_texture->LoadFromFile(context, texturePath);

		std::ifstream ifs(jsonPath);
		json full_data = json::parse(ifs);

		m_json = full_data.at("meta");
		for (auto& slice : m_json.at("slices"))
		{
			SpriteBounds bounds;

			std::string name = slice.at("name");
			json frame = slice.at("keys")[0];
			bounds.bounds = frame.at("bounds").template get<rect::FRect>();
			if (frame.contains("center"))
			{
				bounds.nine_slice = true;
				bounds.center = frame.at("center").template get<rect::FRect>();
				bounds.left_width = bounds.center.x;
				bounds.right_width = bounds.bounds.w - (bounds.center.x + bounds.center.w);
				bounds.top_height = bounds.center.y;
				bounds.btm_height = bounds.bounds.h - (bounds.center.y + bounds.center.h);
			}
			if (frame.contains("pivot"))
			{
				bounds.pivot = frame.at("pivot").template get<glm::vec2>();
			}

			m_spriteMap.emplace(name, bounds);
		}
		return true;
	}
};
