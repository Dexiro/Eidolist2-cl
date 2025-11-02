#include "Rect.h"


void rect::to_json(json& j, const rect::FRect& p) {
	j = json{ { "x", p.x },{ "y", p.y },{ "w", p.w },{ "h", p.h } };
}

void rect::from_json(const json& j, rect::FRect& p) {
	j.at("x").get_to(p.x);
	j.at("y").get_to(p.y);
	j.at("w").get_to(p.w);
	j.at("h").get_to(p.h);
}

void rect::to_json(json& j, const rect::IRect& p) {
	j = json{ { "x", p.x },{ "y", p.y },{ "w", p.w },{ "h", p.h } };
}

void rect::from_json(const json& j, rect::IRect& p) {
	j.at("x").get_to(p.x);
	j.at("y").get_to(p.y);
	j.at("w").get_to(p.w);
	j.at("h").get_to(p.h);
}

rect::FRect rect::IRect::FRect() { return { (float)x, (float)y, (float)w, (float)h }; }
