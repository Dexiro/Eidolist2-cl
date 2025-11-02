#include "Project.h"


Project::Project()
{
	for (int i = 0; i < m_data.size(); i++)
	{
		m_data[i] = std::make_unique<ProjectData>();
	}
}

Project::~Project()
{
	Reset();
}

bool Project::LoadProject(int project, std::string directory)
{
	bool success = true;
	if(!LoadProject::LoadProject(m_data[project].get(), directory))
	{
		success = false;
		Reset();
	}
	else
	{
		m_nLoadedProjects++;
	}

	return success;
}

MapData* Project::GetMapData(int project, int id, bool fullData)
{
	MapData* map_data = nullptr;

	if (id < m_data[project]->m_map_data.size())
		map_data = m_data[project]->m_map_data[id].get();

	if (fullData && map_data && !map_data->lcfMap)
	{
		if (!LoadProject::LoadFullMapData(m_data[project].get(), map_data))
		{
			map_data = nullptr;
		}
	}

	return map_data;
}

int Project::NMaps(int project)
{
	return (int)m_data[project]->m_map_data.size();
}

int Project::NMapsLoaded(int project)
{
	return m_loadedMapsByProject[project].size();
}

int Project::NMapsLoaded()
{
	return m_loadedMaps.size();
}

ProjectData* Project::Data(int project)
{ 
	return m_data[project].get();
}

std::string Project::Directory(int project)
{ 
	return m_data[project]->m_directory;
}

bool Project::LoadMapObj(SDLContext& context, int project, int mapId, glm::vec2& pos)
{
	for (auto mapObj : m_loadedMapsByProject[project])
	{
		if (mapObj->m_mapData->map_id == mapId)
			return true;
	}

	MapData* map_data = GetMapData(project, mapId, true);

	if (map_data != nullptr)
	{
		std::shared_ptr<MapObj> mapObj = std::make_shared<MapObj>();
		mapObj->m_spritesheet = m_debugSpritesheet;
		mapObj->LoadMap(context, map_data);
		mapObj->CompileMap(context, m_data[project].get());
		mapObj->m_pos = pos + m_mapCaret;
		m_mapCaret.x += (mapObj->m_mapData->lcfMap->width * 16) + 128;
		m_loadedMaps.push_back(mapObj);
		m_loadedMapsByProject[project].push_back(mapObj);
		return true;
	}
	return false;
}

bool Project::LoadRequestedMaps(SDLContext& context, glm::vec2& pos)
{
	if (m_requestLoadMapIDs.empty()) return false;

	for (auto& i : m_requestLoadMapIDs)
	{
		LoadMapObj(context, m_selectedProject, i, pos);
	}
	m_requestLoadMapIDs.clear();

	return true;
}

MapObj* Project::GetMapObjByIndex(int project, int index)
{
	MapObj* obj = nullptr;
	if (m_loadedMapsByProject[project].size() > index)
		obj = m_loadedMapsByProject[project][index].get();

	return obj;
}

MapObj* Project::GetMapObjByID(int project, int mapId)
{
	MapObj* obj = nullptr;

	for (auto& mapObj : m_loadedMapsByProject[project])
	{
		if (mapObj->m_mapData->map_id == mapId)
			obj = mapObj.get();
	}

	return obj;
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
	for (int i = 0; i < m_data.size(); i++)
	{
		if (m_data[i]->m_lcf_db)
			m_data[i]->m_lcf_db.reset();

		if (m_data[i]->m_lcf_mt)
			m_data[i]->m_lcf_mt.reset();

		for (auto& map_data : m_data[i]->m_map_data)
			map_data.reset();
	}

	for (int i = 0; i < m_loadedMaps.size(); i++)
	{
		for (auto& map_obj : m_loadedMapsByProject[i])
			map_obj.reset();

		for (auto& map_obj : m_loadedMaps)
			map_obj.reset();
	}
}

