#include "Spritesheet.h"



void glm::to_json(json& j, const glm::vec2& p)
{
	j = json{ {"x", p.x }, {"y", p.y } };
}

void glm::from_json(const json& j, glm::vec2& p)
{
	j.at("x").get_to(p.x);
	j.at("y").get_to(p.y);
}
