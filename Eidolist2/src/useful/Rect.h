#pragma once
#include <SDL3/SDL.h>
#include "GLMIncludes.h"
#include "../external/nlohmann/json.hpp"
using namespace nlohmann;


namespace rect
{
	struct FRect;
	struct IRect
	{
		int x, y, w, h;
		IRect(int init = 0) : x(init), y(init), w(init), h(init) {};
		IRect(int _x, int _y, int _w, int _h) : x(_x), y(_y), w(_w), h(_h) {};
		IRect(glm::ivec2 pos, glm::ivec2 size) : x(pos.x), y(pos.y), w(size.x), h(size.y) {};
		glm::ivec2 Pos() { return { x, y }; }
		glm::ivec2 Size() { return { w, h }; }
		SDL_Rect SDLRect() { return { x, y, w ,h }; }

		FRect FRect();

		IRect operator*(int i) const
		{
			return IRect(x * i, y * i, w * i, h * i);
		}
	};
	void to_json(json& j, const IRect& p);
	void from_json(const json& j, IRect& p);

	struct FRect
	{
		float x, y, w, h;
		FRect(float init = 0.0f) : x(init), y(init), w(init), h(init) { Normalize(); };
		FRect(float _x, float _y, float _w, float _h) : x(_x), y(_y), w(_w), h(_h) { Normalize(); };
		FRect(glm::vec2 pos, glm::vec2 size) : x(pos.x), y(pos.y), w(size.x), h(size.y) { Normalize(); };
		glm::vec2 Pos() { return { x, y }; }
		glm::vec2 Size() { return { w, h }; }
		glm::vec2 Center() { return { x + (w / 2.0f), y + (h / 2.0f) }; }
		SDL_FRect SDLRect() { return { x, y, w ,h }; }

		IRect IRect() { return { (int)x, (int)y, (int)w, (int)h }; }

		FRect Move(glm::vec2 move) { return FRect(x + move.x, y + move.y, w, h); }
		FRect Scale(glm::vec2 scale) { return FRect( x * scale.x, y * scale.y, w * scale.x, h * scale.y); }

		float Top() const { return std::min(y, y + h); }
		float Left() const { return std::min(x, x + w); }
		float Right() const { return std::max(x, x + w); }
		float Bottom() const { return std::max(y, y + h); }

		void RecenterOnPosition(glm::vec2 newCenter)
		{
			glm::vec2 center = Center();
			glm::vec2 diff = newCenter - center;
			x += diff.x;
			y += diff.y;
		}

		FRect& Normalize()
		{
			x = std::min(x, x + w);
			y = std::min(y, y + h);
			w = std::abs(w);
			h = std::abs(h);
			return *this;
		}

		FRect Expand(float amount) const
		{
			return FRect(x - amount, y - amount, w + (amount * 2), h + (amount * 2));
		}

		FRect Shrink(float amount) const
		{
			return FRect(x + amount, y + amount, w - (amount*2), h - (amount*2));
		}

		FRect operator+(const FRect& r) const
		{
			// Get Union of both Rects
			float x1 = std::min(Left(), r.Left());
			float y1 = std::min(Top(), r.Top());
			float x2 = std::max(Right(), r.Right());
			float y2 = std::max(Bottom(), r.Bottom());
			return FRect(x1, y1, x2 - x1, y2 - y1);
		}

		FRect operator-(const FRect& r) const
		{
			// Get Intersection of both Rects
			float x1 = std::max(Left(), r.Left());
			float y1 = std::max(Top(), r.Top());
			float x2 = std::min(Right(), r.Right());
			float y2 = std::min(Bottom(), r.Bottom());
			return FRect(x1, y1, std::max(0.0f, x2 - x1), std::max(0.0f, y2 - y1));
		}

		FRect& operator+=(const FRect& r)
		{
			// Get Union of both Rects
			*this = *this + r;
			//float x2 = std::max(x + w, r.x + r.w);
			//float y2 = std::max(y + h, r.y + r.h);
			//x = std::min(x, r.x);
			//y = std::min(y, r.y);
			//w = x2 - x;
			//h = y2 - y;
			return *this;
		}

		FRect& operator-=(const FRect& r)
		{
			// Get Intersection of both Rects
			float x2 = std::min(Right(), r.Right());
			float y2 = std::min(Bottom(), r.Bottom());
			x = std::max(Left(), r.Left());
			y = std::max(Top(), r.Top());
			w = std::max(0.0f, x2 - x);
			h = std::max(0.0f, y2 - y);
			return *this;
		}

		bool Intersects(const FRect& r) const
		{
			return !(Left() > r.Right() || r.Left() > Right() || Top() > r.Bottom() || r.Top() > Bottom());
		}

	};
	void to_json(json& j, const FRect& p);
	void from_json(const json& j, FRect& p);
}