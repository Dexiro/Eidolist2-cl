#pragma once
#include "LCFIncludes.h"
#include "Types.h"

using LCFMap = std::unique_ptr<lcf::rpg::Map>;
using LCFDatabase = std::unique_ptr<lcf::rpg::Database>;
using LCFMapTree = std::unique_ptr<lcf::rpg::TreeMap>;
using LCFChipset = std::unique_ptr<lcf::rpg::Chipset>;
using LCFTerrain = std::unique_ptr<lcf::rpg::Terrain>;

using TileMap = std::vector<std::vector<int16_t>>;
using ByteMap = std::vector<uint8_t>;
using ByteMap2D = std::vector<std::vector<uint8_t>>;

namespace Passability
{
	enum Passability
	{
		Down = 0x01,
		Left = 0x02,
		Right = 0x04,
		Up = 0x08,
		Star = 0x10,
		Wall = 0x20,
		Counter = 0x40,
		Circle = 0x0F,
		Unknown = 0x80
	};
}

enum AssetType
{
	atLDB,
	atLMT,
	atMap,
	atBackdrop,
	atBackup,
	atBattle,
	atBattleCharSet,
	atBattleWeapon,
	atCharSet,
	atChipSet,
	atFaceSet,
	atFont,
	atLanguage,
	atMonster,
	atMusic,
	atPanorama,
	atPicture,
	atSound,
	atSystem,
	atTitle,
	atCOUNT
};

static const char* AssetTypePath[AssetType::atCOUNT] =
{
	"/",
	"/",
	"/",
	"/Backdrop",
	"/Backup",
	"/Battle",
	"/BattleCharSet",
	"/BattleWeapon",
	"/CharSet",
	"/ChipSet",
	"/FaceSet",
	"/Font",
	"/language",
	"/Monster",
	"/Music",
	"/Panorama",
	"/Picture",
	"/Sound",
	"/System",
	"/Title"
};