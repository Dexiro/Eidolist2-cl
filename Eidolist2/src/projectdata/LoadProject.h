#pragma once
#include <string>
#include "ProjectData.h"


namespace LoadProject
{
	bool LoadProject(ProjectData* project_data, std::string directory);

	void LoadMapList(ProjectData* project_data);
	bool LoadMapHeader(ProjectData* project_data, MapData* map_data, int map_id);
	bool LoadFullMapData(ProjectData* project_data, MapData* map_data);

	void FindMapTransfers(MapData* map_data, ProjectData* project_data = nullptr);
	void GetEventData(ProjectData* project_data, MapData* map_data);

	void GetTileData(MapData* map_data);

	void LoadXML(ProjectData* project_data);
}