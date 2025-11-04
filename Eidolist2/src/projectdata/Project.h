#pragma once
#include "ProjectData.h"
#include "LoadProject.h"

#include "../obj/MapObj.h"
#include "../obj/Spritesheet.h"

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
	ProjectData* Data();
	std::string Directory();

	bool LoadMapObj(SDLContext& context, int mapId, glm::vec2& pos);
	bool LoadRequestedMaps(SDLContext& context, glm::vec2& pos);

	MapObj* GetMapObjByID(int id);

	std::vector<MapObj*> GetCollisions(MapObj* target, glm::vec2 wpos, glm::vec2 wscale);

	Spritesheet* m_debugSpritesheet = nullptr;

protected:
	void Reset();

	PProjectData m_data;

	std::vector<PMapObj> m_loadedMaps;
	std::vector<int> m_requestLoadMapIDs;

	glm::vec2 m_mapCaret = glm::vec2(0.0f);
};
