#include "LoadProject.h"
#include "LCFTypes.h"
#include <fmt/format.h>
#include "TilesetHelper.h"
#include "MapLayers.h"
#include "AutotileData.h"
#include "MathFun.h"
#include "sha1.h"

#include "ListProjectData.h"


std::string getHash(std::string data)
{
	SHA1 sha1;
	sha1.add(data.data(), data.length());
	return sha1.getHash();
}

bool LoadProject::LoadProject(ProjectData* project_data, std::string directory)
{
	bool success = true;

	std::string ldb_path = fmt::format("{}/RPG_RT.ldb", directory);
	std::string lmt_path = fmt::format("{}/RPG_RT.lmt", directory);

	project_data->project_guid = getHash(ldb_path);
	project_data->m_lcf_db = lcf::LDB_Reader::Load(ldb_path, "UTF-8");
	project_data->m_lcf_mt = lcf::LMT_Reader::Load(lmt_path, "UTF-8");

	if (project_data->m_lcf_db && project_data->m_lcf_mt)
	{
		project_data->m_ldb_path = ldb_path;
		project_data->m_ldb_path = lmt_path;
		project_data->m_directory = directory;

		LoadMapList(project_data);
	
		std::vector<list_data::ListItem>& commonEvents = project_data->commonEvents;
		for (const auto& ce : project_data->m_lcf_db->commonevents)
		{
			list_data::ListItem item;
			list_data::List(item, ce);
			commonEvents.push_back(item);
		}
	}
	else
	{
		success = false;
	}

	return success;
}


void LoadProject::LoadMapList(ProjectData* project_data)
{
	for (int i = 0; i < project_data->m_lcf_mt->maps.size(); i++)
	{
		PMapData map_data = std::make_unique<MapData>();
		LoadMapHeader(project_data, map_data.get(), i);
		project_data->m_map_data.push_back(std::move(map_data));
	}
}


bool LoadProject::LoadMapHeader(ProjectData* project_data, MapData* map_data, int map_id)
{
	map_data->map_id = map_id;
	map_data->map_filename = fmt::format("Map{:0{}}", map_id, 4);
	map_data->lmu_path = fmt::format("{}{}/{}.lmu", project_data->m_directory, AssetTypePath[AssetType::atMap], map_data->map_filename);
	map_data->map_name = project_data->m_lcf_mt->maps[map_id].name.c_str();

	map_data->map_guid = getHash(map_data->lmu_path);

	if(!global::wikiData.mapToLocationId.empty())
	{
		if (global::wikiData.mapToLocationId.contains(map_id))
		{
			int locationIndex = global::wikiData.mapToLocationId[map_id];
			if(global::wikiData.locations.size() > locationIndex)
				map_data->wikiLocationData = global::wikiData.locations[locationIndex];
		}
	}

	return map_data;
}

bool LoadProject::LoadFullMapData(ProjectData* project_data, MapData* map_data)
{
	if (!map_data || map_data->map_id <= 0 || map_data->lmu_path.empty())
		return false;

	if(!map_data->lcfMap)
		map_data->lcfMap = std::move(lcf::LMU_Reader::Load(map_data->lmu_path, "UTF-8"));

	if (map_data->lcfMap)
	{
		// Get Chipset Path
		int32_t chipset_id = map_data->lcfMap->chipset_id;
		lcf::rpg::Chipset chipset = project_data->m_lcf_db->chipsets[chipset_id - 1];
		map_data->chipset_name = chipset.chipset_name.c_str();
		map_data->chipset_path = fmt::format("{}{}/{}.png", project_data->m_directory, AssetTypePath[AssetType::atChipSet], map_data->chipset_name);

		map_data->map_size = glm::ivec2(map_data->lcfMap->width, map_data->lcfMap->height);

		// Pull terrain id data from database
		const size_t _padUpperTileIds = 144;
		const int _AutotileTerrainCount = 18;

		std::vector<int16_t> terrainIds;
		terrainIds.insert(terrainIds.end(), chipset.terrain_data.begin() + _AutotileTerrainCount, chipset.terrain_data.end());
		terrainIds.resize(terrainIds.size() + _padUpperTileIds, -1);
		map_data->autotile_terrain_ids.insert(map_data->autotile_terrain_ids.end(), chipset.terrain_data.begin(), chipset.terrain_data.begin() + _AutotileTerrainCount);

		// Get Parallax Path
		map_data->parallax_path = fmt::format("{}{}/{}.png", project_data->m_directory, AssetTypePath[AssetType::atPanorama], map_data->lcfMap->parallax_name.c_str());

		// Mess with the order of the terrain passability data to better suit how we intend to use it
		// Take the non-autotile lower layer data and append the upper layer data.

		const int _ChipsetDEWidth = 18;
		const int _ChipsetDEHeight = 16;
		const int _ChipsetDEColumnStride = 6;

		ByteMap passability_data;
		passability_data.insert(passability_data.end(), chipset.passable_data_lower.begin() + _ChipsetDEWidth, chipset.passable_data_lower.end());
		passability_data.insert(passability_data.end(), chipset.passable_data_upper.begin(), chipset.passable_data_upper.end());

		map_data->autotile_passability_data.insert(map_data->autotile_passability_data.end(), chipset.passable_data_lower.begin(), chipset.passable_data_lower.begin() + _AutotileTerrainCount);

		// Collapse the lower/upper layer map data into a 2D array
		tsh::CollapseArray2D<int16_t>(map_data->lcfMap->lower_layer, map_data->lower_layer, map_data->lcfMap->width, map_data->lcfMap->height);
		tsh::CollapseArray2D<int16_t>(map_data->lcfMap->upper_layer, map_data->upper_layer, map_data->lcfMap->width, map_data->lcfMap->height);
		tsh::CollapseArray2D<uint8_t>(passability_data, map_data->terrain_passability_data, _ChipsetDEWidth, _ChipsetDEHeight, _ChipsetDEColumnStride);
		tsh::CollapseArray2D<int16_t>(terrainIds, map_data->terrain_ids, _ChipsetDEWidth, _ChipsetDEHeight, _ChipsetDEColumnStride);

		LoadProject::FindMapTransfers(map_data, project_data);
		LoadProject::GetTileData(map_data);
		LoadProject::GetEventData(project_data, map_data);

		return true;
	}
	return false;
}

void LoadProject::FindMapTransfers(MapData* map_data, ProjectData* project_data)
{
	for (auto& event : map_data->lcfMap->events)
	{
		for (auto& page : event.pages)
		{
			for (auto& command : page.event_commands)
			{
				if (command.code == commands::ccPlayerTransfer) // transfer
				{
					std::string str = command.string.c_str();
					lcf::DBArray params = command.parameters;
					MapTransferData data;
					data.mapID = params[0];
					data.srcMapID = map_data->map_id;
					data.dstCoord = { (int)params[1], (int)params[2] };
					data.srcCoord = { (int)event.x, (int)event.y };

					if (project_data)
						data.mapName = project_data->m_map_data.at(data.mapID)->map_name;

					map_data->map_transfers.push_back(data);

					if (data.mapID != data.srcMapID)
						map_data->connected_maps.insert(data.mapID);
				}
			}
		}
	}
}


void LoadProject::GetEventData(ProjectData* project_data, MapData* map_data)
{
	for (auto event : map_data->lcfMap->events)
	{
		EventData eventData;
		eventData.pos = glm::ivec2(event.x, event.y);
		eventData.lcfEvent = event;

		for (auto& page : event.pages)
		{
			CharsetData charset;
			charset.name = page.character_name.c_str();

			charset.path = fmt::format("{}{}/{}.png", project_data->m_directory, AssetTypePath[AssetType::atCharSet], charset.name);
			charset.asset_guid = getHash(charset.path);
			
			//charset.index = page.character_index;
			//charset.dir = page.character_direction;
			//charset.pattern = page.character_pattern;
			//charset.animType = page.animation_type;
			//charset.layer = page.layer;
			//charset.moveType = page.move_type;
			//charset.moveFreq = page.move_frequency;
			//charset.moveSpeed = page.move_speed;

			eventData.charsets.push_back(charset);
		}
		map_data->event_data.push_back(eventData);
	}

	std::sort(map_data->event_data.begin(), map_data->event_data.end(), [](const EventData& a, const EventData& b)
		{
			auto& pageA = a.lcfEvent.pages[a.current_page];
			auto& pageB = b.lcfEvent.pages[b.current_page];
			if (pageA.layer != pageB.layer)
			{
				return pageA.layer < pageB.layer;
			}
			else if (a.pos.y != b.pos.y)
			{
				return a.pos.y < b.pos.y;
			}
			else if(a.pos.x != b.pos.x)
			{
				return a.pos.x < b.pos.x;
			}
			else
			{
				return a.lcfEvent.ID < b.lcfEvent.ID;
			}
		});
}

void LoadProject::GetTileData(MapData* map_data)
{
	map_data->tile_data.resize(map_data->lcfMap->height);

	for (auto& tileRow : map_data->tile_data)
		tileRow.resize(map_data->lcfMap->width);

	for (int row = 0; row < map_data->lcfMap->height; row++)
	{
		for (int col = 0; col < map_data->lcfMap->width; col++)
		{
			TileData& td = map_data->tile_data[row][col];

			td.lowerTileID = map_data->lower_layer[row][col];
			td.upperTileID = map_data->upper_layer[row][col];

			{
				// upper tile data
				glm::ivec2 tilePos;
				global::_atData.tilesetIDs.GetTilePos(td.upperTileID, tilePos);

				td.upperSrcRect = rect::IRect(tilePos, glm::ivec2(1)) * 16;

				tilePos.x -= 12; // remove autotile offset (yeah it's a bit wonky...)
				td.upperPassData = map_data->terrain_passability_data[tilePos.y][tilePos.x];
			}

			{
				// lower tile data
				glm::ivec2 tilePos;
				global::_atData.tilesetIDs.GetTilePos(td.lowerTileID, tilePos);

				td.lowerSrcRect = rect::IRect(tilePos, glm::ivec2(1)) * 16;

				if (td.lowerTileID >= 5000)
				{
					tilePos.x -= 12; // remove autotile offset
					td.lowerPassData = map_data->terrain_passability_data[tilePos.y][tilePos.x];
					td.terrainID = (int)map_data->terrain_ids[tilePos.y][tilePos.x];
				}
				else // autotile
				{
					for (int i = 0; i < global::_atData.idThresholds.size(); i++)
					{
						if (td.lowerTileID < global::_atData.idThresholds[i])
						{
							td.autoTypeID = i;
							break;
						}
					}
					td.lowerPassData = (int)map_data->autotile_passability_data[td.autoTypeID];
					td.terrainID = (int)map_data->autotile_terrain_ids[td.autoTypeID];
				}
			}

			if ((td.upperPassData & Passability::Star) == Passability::Star)
			{
				td.useLowPassability = true;
			}

			if ((td.lowerPassData & Passability::Wall) == Passability::Wall)
			{
				int modTileID = td.lowerTileID % 50;
				if (mathf::in_range(modTileID, 20, 23) ||
					mathf::in_range(modTileID, 33, 37) ||
					mathf::in_range(modTileID, 42, 43) ||
					mathf::in_range(modTileID, 45, 46))
				{
					td.lowerPassData = Passability::Circle;
				}
				else
				{
					td.lowerPassData = 0;
				}
			}

		}
	}
}


void LoadProject::LoadXML(ProjectData* project_data)
{


}

