#pragma once
#include <string>
#include <memory>
#include "LCFTypes.h"
#include "../graph/WikiData.h"
#include "GLMIncludes.h"
#include "Rect.h"
#include "ListProjectData.h"


struct CharsetData
{
	std::string name;
	std::string path;
	std::string asset_guid;
};

struct EventData
{
	lcf::rpg::Event lcfEvent;
	glm::ivec2 pos;
	std::vector<CharsetData> charsets;

	int current_page = 0;
};

struct TileData
{
	rect::IRect upperSrcRect;
	rect::IRect lowerSrcRect;
	uint8_t upperPassData = 0;
	uint8_t lowerPassData = 0;
	int autoTypeID = -1;
	int terrainID = -1;
	int lowerTileID = -1;
	int upperTileID = -1;
	bool useLowPassability = false;
};
using TileData2D = std::vector<std::vector<TileData>>;

struct MapTransferData
{
	std::string mapName;
	int mapID;
	int srcMapID;
	glm::ivec2 dstCoord;
	glm::ivec2 srcCoord;
};

struct MapData
{
	// header data (fetched using LoadMapHeader)
	int map_id = -1;
	std::string map_filename;
	std::string map_name;
	std::string lmu_path;
	std::string map_guid;

	// liblcf data types
	LCFMap lcfMap;
	lcf::rpg::MapInfo lcfMapInfo;
	
	// data pulled from LMU / LMT / LDB files
	glm::ivec2 map_size = glm::ivec2(0);

	std::string chipset_path;
	std::string chipset_name;
	std::string parallax_path;

	TileMap lower_layer;
	TileMap upper_layer;
	ByteMap2D terrain_passability_data;
	ByteMap autotile_passability_data;
	TileData2D tile_data;

	std::vector<int16_t> autotile_terrain_ids;
	std::vector<std::vector<int16_t>> terrain_ids;

	// external / metadata 
	wiki::LocationData wikiLocationData;

	// collected data
	std::vector<MapTransferData> map_transfers;
	std::set<int> connected_maps;

	std::vector<EventData> event_data;

};
using PMapData = std::unique_ptr<MapData>;

struct ProjectData
{
	std::string m_directory;
	std::string m_ldb_path;
	std::string m_lmt_path;

	std::vector<PMapData> m_map_data;
	LCFDatabase m_lcf_db;
	LCFMapTree m_lcf_mt;

	std::string project_guid;

	std::vector<list_data::ListItem> commonEvents;
};
using PProjectData = std::unique_ptr<ProjectData>;

/*
11340 WaitForAllMovement
11030 TintScreen
11040 FlashScreen
11120 MovePicture
11320 FlashEvent
11060 ScrollMap
11610 KeyInputProcessing
*/

namespace commands
{
	enum CommandCode
	{
		ccMovePicture = 11120,
		ccKeyInputProcessing = 11610,
		ccScrollMap = 11060,
		ccTintScreen = 11030,
		ccFlashScreen = 11040,
		ccFlashEvent = 11320,
		ccMoveRoute = 11330,
		ccWait = 11410,
		ccWaitForAllMovement = 11340,
		ccPlayerTransfer = 10810
	};
}
