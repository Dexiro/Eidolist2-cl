#pragma once
#include <bitset>
#include <array>
#include "graph/WikiData.h"

enum EMapLayer
{
	mlDefault,
	mlPano,
	mlLower,
	mlUpper,
	mlCharset,
	mlCollision,
	mlEvent,
	mlEventTags,
	mlTransferLines,
	mlChipset,
	mlChipsetEx,
	mlTerrainID,
	mlTileComparison,
	COUNT
};

const std::array<std::string, EMapLayer::COUNT> _mlNames =
{
	"Default",
	"Pano",
	"Lower",
	"Upper",
	"Charset",
	"Collision",
	"Event",
	"Event Tags",
	"Transfr Lns",
	"Chipset",
	"ChipsetEx",
	"TerrainID",
	"TileComparison"
};

namespace global
{
	//extern inline std::bitset<EMapLayer::COUNT> visibleLayersBS;
	extern std::array<bool, EMapLayer::COUNT> visibleLayers;

	extern wiki::WikiData wikiData;
};