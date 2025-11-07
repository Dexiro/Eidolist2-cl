#pragma once
#include "TilesetHelper.h"
#include "ProjectData.h"
#include "Spritesheet.h"
#include "MapLayers.h"

namespace comp_map
{

	PTexture MakeExpandedChipset(SDLContext& context, MapData* map_data);
	PTexture MakeUpperLayer(SDLContext& context, MapData* map_data);
	PTexture MakeLowerLayer(SDLContext& context, MapData* map_data);
	PTexture MakeTileCollisionLayer(SDLContext& context, MapData* map_data, Spritesheet* spritesheet);
	PTexture MakeEventLayer(SDLContext& context, MapData* map_data, Spritesheet* spritesheet);
	PTexture MakeTerrainIDLayer(SDLContext& context, MapData* map_data, Spritesheet* spritesheet);

	PTexture MakeCharsetLayer(SDLContext& context, MapData* map_data);
	
	PTexture MakeTileComparisonLayer(SDLContext& context, MapData* map_data, MapData* map_data_2, Spritesheet* spritesheet);



	void CompileTextureFromTileset(std::shared_ptr<sdl::Texture>& pChipsetTexture, std::vector<tsh::CopyRect>& copyRect, std::shared_ptr<sdl::Texture>& atTex, SDLContext& context, glm::ivec2 fixedSize = glm::ivec2(0));

}