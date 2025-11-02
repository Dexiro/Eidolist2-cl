//#pragma once
//#include "MapObj.h"
//
//
//struct MapComparisonObj : public SimpleObject
//{
//	std::array<MapObj*, 3> m_mapObjects;
//
//	std::vector<MapTransferData> m_transfers;
//
//	std::map<std::string, PTexture> m_srcTextures;
//	std::map<std::string, PTexture> m_dstTextures;
//	std::vector<ObjLayer> m_layers;
//
//	Spritesheet* m_spritesheet;
//
//	glm::ivec2 m_mapTextureSize = glm::ivec2(0);
//	bool m_guiOpen = false;
//
//	~MapComparisonObj();
//
//	virtual void FixedUpdate(SDLContext& context, int frames, double dt);
//	virtual void Draw(SDLContext& context, glm::vec2 pos = glm::vec2(0), glm::vec2 scale = glm::vec2(1));
//
//	void RunMapComparison(SDLContext& context, MapData* map_data);
//
//	void AddComparisonLayers(SDLContext& context, MapData* compare_data);
//
//	void CompileMap(SDLContext& context, ProjectData* data);
//
//	rect::FRect MapRect(glm::vec2 wpos = glm::vec2(0), glm::vec2 wscale = glm::vec2(1));
//	glm::vec2 MapCenter(glm::vec2 wpos = glm::vec2(0), glm::vec2 wscale = glm::vec2(1));
//	bool PointCollision(glm::vec2 point, glm::vec2 wpos = glm::vec2(0), glm::vec2 wscale = glm::vec2(1));
//	bool RectCollision(rect::FRect rect, glm::vec2 wpos = glm::vec2(0), glm::vec2 wscale = glm::vec2(1));
//
//	bool ShowInfo(SDLContext& context);
//
//	void CreateDefaultLayerObj(int depth, PTexture texture, std::string tag, EMapLayer layerID = EMapLayer::mlDefault);
//	void CreateDefaultLayerObj(int depth, EMapLayer layerID);
//	void AddLayerObject(int depth, PBaseObject obj, std::string tag, EMapLayer layerID = EMapLayer::mlDefault);
//
//	void SortLayers();
//
//};
//using PMapObj = std::shared_ptr<MapObj>;
//
//
