#pragma once
#include "Types.h"
#include "ProjectData.h"
#include "LoadProject.h"

#include "../obj/MapObj.h"

struct Request
{

};

class Project
{
public:
	Project();
	~Project();

	bool LoadProject(std::string directory);

	MapData* GetMapData(int id, bool fullData = false);
	
	int NMaps();
	int NMapsLoaded();
	int NMapsLoaded();
	ProjectData* Data();
	std::string Directory()

	bool LoadMapObj(SDLContext& context, int mapId, glm::vec2& pos);
	bool LoadRequestedMaps(SDLContext& context, glm::vec2& pos);

	MapObj* GetMapObjByIndex(int index);
	MapObj* GetMapObjByID(int id);

	std::vector<MapObj*> GetCollisions(MapObj* target, glm::vec2 wpos, glm::vec2 wscale);

	Spritesheet* m_debugSpritesheet = nullptr;

protected:
	void Reset();

	std::array<PProjectData, 3> m_data;

	std::vector<PMapObj> m_loadedMaps;
	std::array<std::vector<PMapObj>, 3> m_loadedMapsByProject;
	
	std::vector<int> m_requestLoadMapIDs;

	int m_selectedProject = 0;
	int m_nLoadedProjects = 0;

	glm::vec2 m_mapCaret = glm::vec2(0.0f);
};
