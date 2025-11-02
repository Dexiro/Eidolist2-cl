#include "MapObj.h"
#include "../Transform.h"
#include "../AssetLoader.h"
#include "CompileMap.h"


MapObj::~MapObj()
{
}

void MapObj::FixedUpdate(SDLContext& context, int frames, double dt)
{
	if (m_parallax)
	{
		if (m_parallax->m_bLoopX)
		{
			m_parallax->m_scrollOffset.x += 16.0f * dt;
		}
		if (m_parallax->m_bLoopY)
		{
			m_parallax->m_scrollOffset.y += 16.0f * dt;
		}
	}
}

void MapObj::Draw(SDLContext& context, glm::vec2 pos, glm::vec2 scale)
{
	for (auto& layer : m_layers)
	{
		if (global::visibleLayers[layer.m_id])
			layer.m_obj->Draw(context, m_pos + pos, m_scale * scale);
	}

	for (auto child : m_children)
	{
		child->Draw(context, m_pos + pos, m_scale * scale);
	}

	if (m_texture)
		m_texture->Render(context, m_pos + pos, m_scale * scale, &clipRect);
}

void MapObj::LoadMap(SDLContext& context, MapData* map_data)
{
	m_mapData = map_data;
	m_mapTextureSize = glm::ivec2(m_mapData->lcfMap->width, m_mapData->lcfMap->height) * 16;

	PTexture chipsetTexture = std::make_shared<sdl::Texture>();
	chipsetTexture->LoadFromFile(context, m_mapData->chipset_path);
	m_srcTextures.emplace(_mlNames[EMapLayer::mlChipset], chipsetTexture);

	PTexture parallaxTexture = std::make_shared<sdl::Texture>();
	parallaxTexture->LoadFromFile(context, m_mapData->parallax_path, false);
	m_srcTextures.emplace(_mlNames[EMapLayer::mlPano], parallaxTexture);
}

void MapObj::AddComparisonLayers(SDLContext& context, MapData* compare_data)
{
	std::string dir = m_mapData->lmu_path;
	int id = m_mapData->map_id;

	PTexture tileCompLayer;
	db::_texDB.GetTexture(context, db::MapLayerTag(dir, id, _mlNames[(int)mlTileComparison]), tileCompLayer, &comp_map::MakeTileComparisonLayer, m_mapData, compare_data, m_spritesheet);

	m_dstTextures.emplace(_mlNames[mlTileComparison], tileCompLayer);

	CreateDefaultLayerObj(250, EMapLayer::mlTileComparison);

	SortLayers();
}

void MapObj::CreateDefaultLayerObj(int depth, PTexture texture, std::string tag, EMapLayer layerID)
{
	if (texture)
	{
		PBaseObject obj = std::make_shared<SimpleObject>();
		obj->m_pos = glm::vec2(0);
		obj->m_texture = texture;
		obj->clipRect = SDL_FRect(0, 0, texture->Width(), texture->Height());
		m_layers.push_back(ObjLayer(depth, obj, tag, layerID));
	}
}

void MapObj::CreateDefaultLayerObj(int depth, EMapLayer layerID)
{
	std::string tag = _mlNames[(int)layerID];
	PTexture tex = m_dstTextures[tag];
	CreateDefaultLayerObj(depth, tex, tag, layerID);
}

void MapObj::AddLayerObject(int depth, PBaseObject obj, std::string tag, EMapLayer layerID)
{
	m_layers.push_back(ObjLayer(depth, obj, tag, layerID));
}

void MapObj::SortLayers()
{
	std::sort(m_layers.begin(), m_layers.end(), [&](ObjLayer& a, ObjLayer& b)
	{
		return a.m_depth < b.m_depth;
	});
}

void MapObj::CompileMap(SDLContext& context, ProjectData* data)
{
	std::string dir = data->m_directory;
	int id = m_mapData->map_id;

	PTexture chipsetEx, lowerLayer, upperLayer, collisionLayer, eventLayer, terrainIDLayer, charsetLayer;
	db::_texDB.GetTexture(context, db::ChipsetExTag(m_mapData->chipset_path), chipsetEx, &comp_map::MakeExpandedChipset, m_mapData);
	db::_texDB.GetTexture(context, db::MapLayerTag(dir, id, _mlNames[(int)mlLower]), lowerLayer, &comp_map::MakeLowerLayer, m_mapData);
	db::_texDB.GetTexture(context, db::MapLayerTag(dir, id, _mlNames[(int)mlUpper]), upperLayer, &comp_map::MakeUpperLayer, m_mapData);
	db::_texDB.GetTexture(context, db::MapLayerTag(dir, id, _mlNames[(int)mlCollision]), collisionLayer, &comp_map::MakeTileCollisionLayer, m_mapData, m_spritesheet);
	db::_texDB.GetTexture(context, db::MapLayerTag(dir, id, _mlNames[(int)mlEvent]), eventLayer, &comp_map::MakeEventLayer, m_mapData, m_spritesheet);
	db::_texDB.GetTexture(context, db::MapLayerTag(dir, id, _mlNames[(int)mlTerrainID]), terrainIDLayer, &comp_map::MakeTerrainIDLayer, m_mapData, m_spritesheet);
	db::_texDB.GetTexture(context, db::MapLayerTag(dir, id, _mlNames[(int)mlCharset]), charsetLayer, &comp_map::MakeCharsetLayer, m_mapData);

	m_srcTextures.emplace(_mlNames[mlChipsetEx], chipsetEx);
	m_dstTextures.emplace(_mlNames[mlLower], lowerLayer);
	m_dstTextures.emplace(_mlNames[mlUpper], upperLayer);
	m_dstTextures.emplace(_mlNames[mlCollision], collisionLayer);
	m_dstTextures.emplace(_mlNames[mlEvent], eventLayer);
	m_dstTextures.emplace(_mlNames[mlTerrainID], terrainIDLayer);
	m_dstTextures.emplace(_mlNames[mlCharset], charsetLayer);

	CreateDefaultLayerObj(1, EMapLayer::mlLower);
	CreateDefaultLayerObj(2, EMapLayer::mlUpper);
	CreateDefaultLayerObj(10, EMapLayer::mlCollision);
	CreateDefaultLayerObj(100, EMapLayer::mlEvent);
	CreateDefaultLayerObj(140, EMapLayer::mlTerrainID);
	CreateDefaultLayerObj(150, EMapLayer::mlEventTags);
	CreateDefaultLayerObj(95, EMapLayer::mlCharset);

	{ // Map Name
		glm::ivec2 size;
		TTF_Text* mapNameText = TTF_CreateText(context.TtfEngine, context.Fonts[1], m_mapData->map_name.c_str(), 0);
		TTF_GetTextSize(mapNameText, &size.x, &size.y);

		PTexture mapNameTexture = std::make_shared<sdl::Texture>();
		mapNameTexture->CreateEmpty(context, size.x, size.y, SDL_TextureAccess::SDL_TEXTUREACCESS_TARGET);
		SDL_SetRenderTarget(context.Renderer, mapNameTexture->GetTexture());
		SDL_SetRenderDrawColor(context.Renderer, 0xFF, 0xFF, 0xFF, 0xBB);
		TTF_DrawRendererText(mapNameText, 0.0f, 0.0f);
		TTF_DestroyText(mapNameText);
		SDL_SetRenderTarget(context.Renderer, NULL);
		m_dstTextures.emplace("MapName", mapNameTexture);
	}

	if (m_dstTextures["MapName"])
	{ // Event Layer
		PBaseObject obj = std::make_shared<SimpleObject>();
		obj->m_texture = m_dstTextures["MapName"];
		obj->m_pos = glm::vec2(0.0f, -(obj->m_texture->Height() + 10.0f));
		obj->clipRect = SDL_FRect(0, 0, obj->m_texture->Width(), obj->m_texture->Height());
		AddLayerObject(300, obj, "MapName");
	}
	if (m_srcTextures[_mlNames[mlPano]] && m_srcTextures[_mlNames[mlPano]]->GetTexture())
	{
		std::shared_ptr<ParallaxObj> obj = std::make_shared<ParallaxObj>();
		obj->SetTexture(m_srcTextures[_mlNames[mlPano]]);
		obj->m_tiled = true;
		obj->m_bLoopX = m_mapData->lcfMap->parallax_loop_x;
		obj->m_bLoopY = m_mapData->lcfMap->parallax_loop_y;
		obj->ResizeTexture(context, m_mapTextureSize);
		AddLayerObject(0, obj, _mlNames[mlPano], mlPano);
		m_parallax = obj;
	}
	{ // Expanded Chipset
		PBaseObject obj = std::make_shared<SimpleObject>();
		obj->m_pos = glm::vec2(0, 0 - m_srcTextures[_mlNames[mlChipsetEx]]->Height());
		obj->m_texture = m_srcTextures[_mlNames[mlChipsetEx]];
		obj->clipRect = SDL_FRect(0, 0, obj->m_texture->Width(), obj->m_texture->Height());
		AddLayerObject(0, obj, "MapName", mlChipsetEx);
	}
	SortLayers();
}

rect::FRect MapObj::MapRect(glm::vec2 wpos, glm::vec2 wscale)
{
	glm::vec2 size = { (float)m_mapData->lcfMap->width, (float)m_mapData->lcfMap->height };
	size *= 16.0f * m_scale * wscale;
	glm::vec2 pos;
	tr::worldToScreen(pos, m_pos, wpos, wscale);
	return rect::FRect(pos.x, pos.y, size.x, size.y);
}

glm::vec2 MapObj::MapCenter(glm::vec2 wpos, glm::vec2 wscale)
{
	rect::FRect rect = MapRect(wpos, wscale);
	return glm::vec2(rect.x + (rect.w / 2.0f), rect.y + (rect.h / 2.0f));
}

bool MapObj::PointCollision(glm::vec2 pos, glm::vec2 wpos, glm::vec2 wscale)
{
	rect::FRect mapRect = MapRect(wpos, wscale);
	SDL_FRect testRect = mapRect.SDLRect();
	SDL_FPoint point = { pos.x, pos.y };
	return SDL_PointInRectFloat(&point, &testRect);
}

bool MapObj::RectCollision(rect::FRect rect, glm::vec2 wpos, glm::vec2 wscale)
{
	rect::FRect mapRect = MapRect(wpos, wscale);
	SDL_FRect testRectA = mapRect.SDLRect();
	SDL_FRect testRectB = rect.SDLRect();
	SDL_FRect result;
	return SDL_GetRectIntersectionFloat(&testRectA, &testRectB, &result);
}

void TableRow_AddString(std::string key, std::string value)
{
	ImGui::TableNextRow(ImGuiTableRowFlags_None, 0.0f);
	ImGui::TableSetColumnIndex(0);
	ImGui::TextUnformatted(key.c_str());
	ImGui::TableSetColumnIndex(1);
	ImGui::TextUnformatted(value.c_str());
}

bool MapObj::ShowInfo(SDLContext& context)
{
	// Main body of the Demo window starts here.
	ImGui::PushID(fmt::format("MapInfo{}", m_mapData->map_id).c_str());

	static const float TEXT_BASE_WIDTH = ImGui::CalcTextSize("A").x;
	static const float TEXT_BASE_HEIGHT = ImGui::GetTextLineHeightWithSpacing();
	static const ImGuiTableFlags table_flags = ImGuiTableFlags_BordersV | ImGuiTableFlags_BordersOuterH | ImGuiTableFlags_Resizable | ImGuiTableFlags_RowBg | ImGuiTableFlags_NoBordersInBody;
	static const ImGuiTreeNodeFlags tree_node_flags_base = ImGuiTreeNodeFlags_SpanAllColumns | ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_DrawLinesFull;

	if (ImGui::BeginTable("Table", 2, table_flags))
	{
		ImGui::TableSetupColumn("Name", ImGuiTableColumnFlags_WidthFixed | ImGuiTableColumnFlags_DefaultSort | ImGuiTableColumnFlags_DefaultHide, 0.0f, 0);
		ImGui::TableSetupColumn("Value", ImGuiTableColumnFlags_WidthFixed | ImGuiTableColumnFlags_NoHide, 0.0f, 1);
		ImGui::TableHeadersRow();

		TableRow_AddString("name", fmt::format("{}", m_mapData->map_name));
		TableRow_AddString("size", fmt::format("{},{}", m_mapData->lcfMap->width, m_mapData->lcfMap->height));
		TableRow_AddString("bgm", fmt::format("{}", m_mapData->lcfMapInfo.music.name.c_str()));
		TableRow_AddString("event count", fmt::format("{}", m_mapData->lcfMap->events.size()));
		TableRow_AddString("parallax", fmt::format("{}", m_mapData->lcfMap->parallax_name.c_str()));

		ImGui::Separator();
		TableRow_AddString("chipset id", fmt::format("{}", m_mapData->lcfMap->chipset_id));
		TableRow_AddString("chipset name", fmt::format("{}", m_mapData->chipset_name));
		TableRow_AddString("scroll type", fmt::format("{}", lcf::rpg::Map::kScrollTypeTags[m_mapData->lcfMap->scroll_type]));

		ImGui::EndTable();
	}
	ImGui::Separator();
	ImGui::PopID();

	return true;
}

