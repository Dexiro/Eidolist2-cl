#pragma once
#include "CommonIncludes.h"
#include "TilesetHelper.h"

struct AutoTileBlueprint
{
	tsh::TileSetIDs tilesetIDs;
	std::vector<std::vector<int>> subtileDBlueprint;
	std::vector<std::vector<glm::ivec2>> setDBlueprint;
	std::vector<tsh::CopyRect> setDCompileRects;

	const std::array<int, 18> idThresholds = {
	1000,
	2000,
	3000,
	3050,
	3100,
	4000,
	4050,
	4100,
	4150,
	4200,
	4250,
	4300,
	4350,
	4400,
	4450,
	4500,
	4550,
	5000
	};

	AutoTileBlueprint() { Load(); }
protected:
	void Load();
};

namespace global
{
	extern AutoTileBlueprint _atData;
}