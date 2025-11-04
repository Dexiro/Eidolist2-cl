#include "Project.h"


Project::Project()
{
	m_data = std::make_unique<ProjectData>();
}

Project::~Project()
{
	Reset();
}

bool Project::LoadProject(std::string directory)
{
	bool success = true;
	if(!LoadProject::LoadProject(m_data.get(), directory))
	{
		success = false;
		Reset();
	}

	return success;
}

MapData* Project::GetMapData(int id, bool fullData)
{
	MapData* map_data = nullptr;

	if (id < m_data->m_map_data.size())
		map_data = m_data->m_map_data[id].get();

	if (fullData && map_data && !map_data->lcfMap)
	{
		if (!LoadProject::LoadFullMapData(m_data.get(), map_data))
		{
			map_data = nullptr;
		}
	}

	return map_data;
}

int Project::NMaps()
{
	return (int)m_data->m_map_data.size();
}

int Project::NMapsLoaded()
{
	return m_loadedMaps.size();
}

ProjectData* Project::Data()
{ 
	return m_data.get();
}

std::string Project::Directory()
{ 
	return m_data->m_directory;
}

bool Project::LoadMapObj(SDLContext& context, int mapId, glm::vec2& pos)
{
	for (auto mapObj : m_loadedMaps)
	{
		if (mapObj->m_mapData->map_id == mapId)
			return true;
	}

	MapData* map_data = GetMapData(mapId, true);

	if (map_data != nullptr)
	{
		std::shared_ptr<MapObj> mapObj = std::make_shared<MapObj>();
		mapObj->m_spritesheet = m_debugSpritesheet;
		mapObj->LoadMap(context, map_data);
		mapObj->CompileMap(context, m_data.get());
		mapObj->m_pos = pos + m_mapCaret;
		m_mapCaret.x += (mapObj->m_mapData->lcfMap->width * 16) + 128;
		m_loadedMaps.push_back(mapObj);
		return true;
	}
	return false;
}

bool Project::LoadRequestedMaps(SDLContext& context, glm::vec2& pos)
{
	if (m_requestLoadMapIDs.empty()) return false;

	for (auto& i : m_requestLoadMapIDs)
	{
		LoadMapObj(context, i, pos);
	}
	m_requestLoadMapIDs.clear();

	return true;
}

MapObj* Project::GetMapObjByID(int id)
{
	return m_loadedMaps[id].get();
}

std::vector<MapObj*> Project::GetCollisions(MapObj* target, glm::vec2 wpos, glm::vec2 wscale)
{
	std::vector<MapObj*> collisions;
	if (!target) return collisions;

	SDL_FRect rectA = target->MapRect(wpos, wscale).SDLRect();

	for(auto& map : m_loadedMaps)
	{
		if (!map || map.get() == target)
			continue;

		SDL_FRect result;
		SDL_FRect rectB = map->MapRect(wpos, wscale).SDLRect();
		SDL_GetRectIntersectionFloat(&rectA, &rectB, &result);

		if (result.w != 0 && result.h != 0)
		{
			collisions.push_back(map.get());
		}
	}
	return collisions;
}

void Project::Reset()
{
	m_data->m_lcf_db.reset();
	m_data->m_lcf_mt.reset();

	for (auto& map_data : m_data->m_map_data)
		map_data.reset();

	for (auto& map_obj : m_loadedMaps)
		map_obj.reset();
}

