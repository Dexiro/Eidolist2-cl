#include "CompileMap.h"
#include "AssetLoader.h"
#include "AutotileData.h"
#include "ImguiStyle.h"

PTexture comp_map::MakeExpandedChipset(SDLContext& context, MapData* map_data)
{
	std::string path = map_data->chipset_path;

	PTexture chipsetTex;
	db::_texDB.LoadTexture(context, path, path, chipsetTex);

	PTexture chipsetExTexture = std::make_shared<sdl::Texture>();
	tsh::texture::CompileTextureFromTileset(chipsetTex, global::_atData.setDCompileRects, chipsetExTexture, context);

	return chipsetExTexture;
}

PTexture comp_map::MakeUpperLayer(SDLContext& context, MapData* map_data)
{
	std::vector<tsh::CopyRect> cprList;

	for (int row = 0; row < map_data->lcfMap->height; row++)
	{
		for (int col = 0; col < map_data->lcfMap->width; col++)
		{
			auto& td = map_data->tile_data[row][col];

			tsh::CopyRect cpr;
			cpr.dst_pos = glm::ivec2(col, row) * 16;
			cpr.src_pos = td.upperSrcRect.Pos();
			cpr.dst_size = td.upperSrcRect.Size();
			cpr.src_size = td.upperSrcRect.Size();
			cprList.push_back(cpr);
		}
	}

	PTexture chipsetEx;
	db::_texDB.GetTexture(context, db::ChipsetExTag(map_data->chipset_path), chipsetEx);

	PTexture upperLayerTexture = std::make_shared<sdl::Texture>();
	tsh::texture::CompileTextureFromTileset(chipsetEx, cprList, upperLayerTexture, context);

	return upperLayerTexture;
}

PTexture comp_map::MakeLowerLayer(SDLContext& context, MapData* map_data)
{
	std::vector<tsh::CopyRect> cprList;

	for (int row = 0; row < map_data->lcfMap->height; row++)
	{
		for (int col = 0; col < map_data->lcfMap->width; col++)
		{
			auto& td = map_data->tile_data[row][col];

			tsh::CopyRect cpr;
			cpr.dst_pos = glm::ivec2(col, row) * 16;
			cpr.src_pos = td.lowerSrcRect.Pos();
			cpr.dst_size = td.lowerSrcRect.Size();
			cpr.src_size = td.lowerSrcRect.Size();
			cprList.push_back(cpr);
		}
	}

	PTexture chipsetEx;
	db::_texDB.GetTexture(context, db::ChipsetExTag(map_data->chipset_path), chipsetEx);

	PTexture lowerLayerTexture = std::make_shared<sdl::Texture>();
	tsh::texture::CompileTextureFromTileset(chipsetEx, cprList, lowerLayerTexture, context);

	return lowerLayerTexture;
}

PTexture comp_map::MakeTileCollisionLayer(SDLContext& context, MapData* map_data, Spritesheet* spritesheet)
{
	// 0 Circle, 1 Cross, 2 Star, 3 Square
	glm::ivec2 tileSize = glm::ivec2(16, 16);
	std::vector<tsh::CopyRect> cprList;

	for (int row = 0; row < map_data->lcfMap->height; row++)
	{
		for (int col = 0; col < map_data->lcfMap->width; col++)
		{
			tsh::CopyRect cpr;

			auto& td = map_data->tile_data[row][col];
			uint8_t passData = (td.useLowPassability) ? td.lowerPassData : td.upperPassData;

			passData = Passability::Circle & passData;
			if (passData == Passability::Circle)
			{
				continue;
			}
			else if (passData == 0)
			{
				cpr.src_pos = spritesheet->m_spriteMap.at("PassCollision").bounds.IRect().Pos();
				cpr.dst_pos = glm::ivec2(col, row) * 16;
				cpr.dst_size = tileSize;
				cpr.src_size = tileSize;
				cprList.push_back(cpr);
			}
			else
			{
				for (int i = 0; i < 4; i++)
				{
					if ((passData >> i) & (uint8_t)1)
					{
						cpr.src_pos = spritesheet->m_spriteMap.at("PassDown").bounds.IRect().Pos();
					}
					else
					{
						cpr.src_pos = spritesheet->m_spriteMap.at("WallDown").bounds.IRect().Pos();
					}

					cpr.src_pos.x += 16 * i;
					cpr.dst_pos = glm::ivec2(col, row) * 16;
					cpr.dst_size = tileSize;
					cpr.src_size = tileSize;
					cprList.push_back(cpr);
				}
			}
		}
	}

	PTexture collisionLayerTexture = std::make_shared<sdl::Texture>();
	tsh::texture::CompileTextureFromTileset(spritesheet->m_texture, cprList, collisionLayerTexture, context, map_data->map_size * 16);
	return collisionLayerTexture;
}

PTexture comp_map::MakeEventLayer(SDLContext& context, MapData* map_data, Spritesheet* spritesheet)
{
	SpriteBounds bounds = spritesheet->m_spriteMap.at("EventTile");
	SpriteBounds boundsCol = spritesheet->m_spriteMap.at("EventTileCollision");

	std::vector<tsh::CopyRect> eventCPR;
	for (auto& event : map_data->lcfMap->events)
	{
		tsh::CopyRect cpr;
		cpr.src_pos = { (int)bounds.bounds.x, (int)bounds.bounds.y };
		cpr.dst_pos = { (int)(event.x * 16), (int)(event.y * 16) };
		cpr.dst_size = { (int)bounds.bounds.w, (int)bounds.bounds.h };
		cpr.src_size = { (int)bounds.bounds.w, (int)bounds.bounds.h };

		if (event.pages[0].layer == 1)
		{
			cpr.src_pos = { (int)boundsCol.bounds.x, (int)boundsCol.bounds.y };
		}

		eventCPR.push_back(cpr);
	}

	PTexture eventLayerTexture = std::make_shared<sdl::Texture>();
	tsh::texture::CompileTextureFromTileset(spritesheet->m_texture, eventCPR, eventLayerTexture, context, map_data->map_size * 16);
	return eventLayerTexture;
}

PTexture comp_map::MakeTerrainIDLayer(SDLContext& context, MapData* map_data, Spritesheet* spritesheet)
{
	PTexture layerTexture = std::make_shared<sdl::Texture>();
	layerTexture->CreateEmpty(context, map_data->map_size.x * 16, map_data->map_size.y * 16, SDL_TextureAccess::SDL_TEXTUREACCESS_TARGET);
	SDL_SetRenderTarget(context.Renderer, layerTexture->GetTexture());

	for (int row = 0; row < map_data->lcfMap->height; row++)
	{
		for (int col = 0; col < map_data->lcfMap->width; col++)
		{
			auto& td = map_data->tile_data[row][col];
			int terrainID = td.terrainID;

			float r, g, b;
			color::hexToRGB(_sampleColorPalette[terrainID], r, g, b);
			SDL_FRect bgRect = { (col * 16.0f), (row * 16.0f), 16.0f, 16.0f };
			SDL_SetRenderDrawColorFloat(context.Renderer, r, g, b, 1.0f);
			SDL_RenderFillRect(context.Renderer, &bgRect);

			std::string terrainIDString = fmt::format("{:0{}}", terrainID, 2);
			{
				std::string number = terrainIDString.substr(0, 1);
				rect::FRect src = spritesheet->m_spriteMap[number].bounds;
				float x = (col * 16.0f) + 2.0f;
				float y = (row * 16.0f) + 2.0f;
				SDL_FRect fsrc = src.SDLRect();
				SDL_FRect fdst(x, y, 6.0f, 12.0f);
				SDL_RenderTexture(context.Renderer, spritesheet->m_texture->GetTexture(), &fsrc, &fdst);
			}
			{
				std::string number = terrainIDString.substr(1, 1);
				rect::FRect src = spritesheet->m_spriteMap[number].bounds;
				float x = (col * 16.0f) + 8.0f;
				float y = (row * 16.0f) + 2.0f;
				SDL_FRect fsrc = src.SDLRect();
				SDL_FRect fdst(x, y, 6.0f, 12.0f);
				SDL_RenderTexture(context.Renderer, spritesheet->m_texture->GetTexture(), &fsrc, &fdst);
			}
		}
	}

	SDL_SetRenderTarget(context.Renderer, NULL);
	return layerTexture;
}

PTexture comp_map::MakeCharsetLayer(SDLContext& context, MapData* map_data)
{
	PTexture layerTexture = std::make_shared<sdl::Texture>();
	layerTexture->CreateEmpty(context, map_data->map_size.x * 16, map_data->map_size.y * 16, SDL_TextureAccess::SDL_TEXTUREACCESS_TARGET);
	SDL_SetRenderTarget(context.Renderer, layerTexture->GetTexture());

	for (auto& event : map_data->event_data)
	{
		CharsetData & charset = event.charsets[0];
		lcf::rpg::EventPage& eventPage = event.lcfEvent.pages[0];

		PTexture charsetTex;
		db::_texDB.LoadTexture(context, charset.asset_guid, charset.path, charsetTex, true);

		float charW = (float)charsetTex->Width();
		float charH = (float)charsetTex->Height();
		float baseW = 288.0f;
		float baseH = 256.0f;

		glm::vec2 sizeMod = glm::vec2((1.0f / baseW) * charW, (1.0f / baseH) * charH);
		glm::vec2 tileSize = glm::vec2(24, 32);
		tileSize.x *= sizeMod.x;
		tileSize.y *= sizeMod.y;

		glm::vec2 centerOffset = glm::vec2(-(tileSize.x / 2.0f) + 8.0f, -tileSize.y + 16.0f);

		glm::vec2 partSize = glm::vec2(tileSize.x * 3, tileSize.y * 4);

		int partX = (eventPage.character_index % 4);
		int partY = (eventPage.character_index / 4);

		auto isrcrect = rect::FRect((partX * partSize.x) + (eventPage.character_pattern * tileSize.x), (partY * partSize.y) + (eventPage.character_direction * tileSize.y), tileSize.x, tileSize.y).IRect();
		auto idstrect = rect::FRect((event.pos.x * 16.0f) + centerOffset.x, (event.pos.y * 16.0f) + centerOffset.y, tileSize.x, tileSize.y).IRect();

		SDL_FRect fsrc = isrcrect.FRect().SDLRect();
		SDL_FRect fdst = idstrect.FRect().SDLRect();

		SDL_RenderTexture(context.Renderer, charsetTex->GetTexture(), &fsrc, &fdst);
	}

	SDL_SetRenderTarget(context.Renderer, NULL);
	return layerTexture;
}

PTexture comp_map::MakeTileComparisonLayer(SDLContext& context, MapData* map_data, MapData* map_data_2, Spritesheet* spritesheet)
{
	SpriteBounds upperTileB = spritesheet->m_spriteMap.at("UpperTileDiff");
	SpriteBounds lowerTileB = spritesheet->m_spriteMap.at("LowerTileDiff");
	SpriteBounds terrainB = spritesheet->m_spriteMap.at("TerrainDiff");
	SpriteBounds passB = spritesheet->m_spriteMap.at("PassDiff");
	std::vector<tsh::CopyRect> layerCpr;

	for (int row = 0; row < map_data->lcfMap->height; row++)
	{
		for (int col = 0; col < map_data->lcfMap->width; col++)
		{
			if (col >= map_data_2->map_size.x || row >= map_data_2->map_size.y)
				continue;

			auto& td1 = map_data->tile_data[row][col];
			auto& td2 = map_data_2->tile_data[row][col];

			if (td1.lowerTileID != td2.lowerTileID)
			{
				tsh::CopyRect cpr;
				cpr.src_pos = lowerTileB.bounds.IRect().Pos();
				cpr.src_size = lowerTileB.bounds.IRect().Size();
				cpr.dst_pos = glm::ivec2(col, row) * 16;
				cpr.dst_size = lowerTileB.bounds.IRect().Size();
				layerCpr.push_back(cpr);
			}			
			if (td1.upperTileID != td2.upperTileID)
			{
				tsh::CopyRect cpr;
				cpr.src_pos = upperTileB.bounds.IRect().Pos();
				cpr.src_size = upperTileB.bounds.IRect().Size();
				cpr.dst_pos = glm::ivec2(col, row) * 16;
				cpr.dst_size = upperTileB.bounds.IRect().Size();
				layerCpr.push_back(cpr);
			}			
			if (td1.terrainID != td2.terrainID)
			{
				tsh::CopyRect cpr;
				cpr.src_pos = terrainB.bounds.IRect().Pos();
				cpr.src_size = terrainB.bounds.IRect().Size();
				cpr.dst_pos = glm::ivec2(col, row) * 16;
				cpr.dst_size = terrainB.bounds.IRect().Size();
				layerCpr.push_back(cpr);
			}			
			if (td1.lowerPassData != td2.lowerPassData || td1.upperPassData != td2.upperPassData)
			{
				tsh::CopyRect cpr;
				cpr.src_pos = passB.bounds.IRect().Pos();
				cpr.src_size = passB.bounds.IRect().Size();
				cpr.dst_pos = glm::ivec2(col, row) * 16;
				cpr.dst_size = passB.bounds.IRect().Size();
				layerCpr.push_back(cpr);
			}
		}
	}

	PTexture layerTex = std::make_shared<sdl::Texture>();
	tsh::texture::CompileTextureFromTileset(spritesheet->m_texture, layerCpr, layerTex, context, map_data->map_size * 16);
	return layerTex;
}
