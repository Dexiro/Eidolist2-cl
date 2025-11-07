#pragma once
#include "BaseObj.h"
#include "ProjectData.h"
#include "Spritesheet.h"
#include "SpriteObj.h"
#include "WindowArea.h"
#include "MapLayers.h"
#include "AutotileData.h"

struct EventObj : public SimpleObject
{
	lcf::rpg::Event lcfEvent;
	std::map<std::string, PTexture> m_textures;
	std::vector<CharsetData> m_charsetData;
};

struct ObjLayer
{
	ObjLayer(int depth, PBaseObject obj, std::string tag, EMapLayer id = mlDefault) : m_depth(depth), m_id(id), m_tag(tag), m_obj(obj) {};
	int m_depth = 0;
	EMapLayer m_id;
	PBaseObject m_obj;
	std::string m_tag;
};

struct MapObj : public SimpleObject
{
	MapData* m_mapData;

	std::vector<MapTransferData> m_transfers;

	std::map<std::string, PTexture> m_srcTextures;
	std::map<std::string, PTexture> m_dstTextures;
	//std::map<std::string, ObjLayer> m_layers;
	std::vector<ObjLayer> m_layers;

	std::shared_ptr<ParallaxObj> m_parallax;

	Spritesheet* m_spritesheet;

	glm::ivec2 m_mapTextureSize = glm::ivec2(0);
	TTF_Text* m_mapNameText = nullptr;
	bool m_guiOpen = false;

	~MapObj();

	virtual void FixedUpdate(SDLContext& context, int frames, double dt);

	virtual void Draw(SDLContext& context, glm::vec2 pos = glm::vec2(0), glm::vec2 scale = glm::vec2(1));

	void LoadMap(SDLContext& context, MapData* map_data);

	void AddComparisonLayers(SDLContext& context, MapData* compare_data);

	void CompileMap(SDLContext& context, ProjectData* data);

	rect::FRect MapRect(glm::vec2 wpos = glm::vec2(0), glm::vec2 wscale = glm::vec2(1));
	glm::vec2 MapCenter(glm::vec2 wpos = glm::vec2(0), glm::vec2 wscale = glm::vec2(1));
	bool PointCollision(glm::vec2 point, glm::vec2 wpos = glm::vec2(0), glm::vec2 wscale = glm::vec2(1));
	bool RectCollision(rect::FRect rect, glm::vec2 wpos = glm::vec2(0), glm::vec2 wscale = glm::vec2(1));

	bool ShowInfo(SDLContext& context);

	void CreateDefaultLayerObj(int depth, PTexture texture, std::string tag, EMapLayer layerID = EMapLayer::mlDefault);
	void CreateDefaultLayerObj(int depth, EMapLayer layerID);
	void AddLayerObject(int depth, PBaseObject obj, std::string tag, EMapLayer layerID = EMapLayer::mlDefault);

	void SortLayers();

};
using PMapObj = std::shared_ptr<MapObj>;


