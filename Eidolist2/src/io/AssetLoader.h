#pragma once
#include "CommonIncludes.h"
#include "BaseObj.h"
#include "ProjectData.h"
#include "Spritesheet.h"

namespace db
{
	using CompileMapFunc = std::function<PTexture(SDLContext&, PMapData&)>;

	// Tons of room for optimisation, such as using a hash table 
	class TextureDB
	{
	public:
		TextureDB() = default;
		~TextureDB() = default;

		TextureDB(const TextureDB&) = delete;
		TextureDB& operator=(const TextureDB&) = delete;

		TextureDB(TextureDB&&) = delete;
		TextureDB& operator=(TextureDB&&) = delete;

		// Lazy implementation, I just have a load of versions of LoadTexture because I'm bad at figuring out template stuff...
		bool LoadTexture(SDLContext& context, std::string asset_tag, std::string path, PTexture& texture, bool useAlpha = true)
		{
			if (texture)
				return false;

			if (!db_map.contains(asset_tag))
			{
				texture = std::make_shared<sdl::Texture>();
				if (texture->LoadFromFile(context, path, useAlpha))
				{
					db_item item;
					item.path = path;
					item.tag = asset_tag;
					item.tex = texture;
					item.type = db_item::PROJECT;
					db_map.emplace(asset_tag, item);
				}
				else
				{
					return false;
				}
			}
			else
			{
				texture = db_map.at(asset_tag).tex;
			}
			return true;
		}

		bool GetTexture(SDLContext& context, std::string asset_tag, PTexture& texture, std::function<PTexture(SDLContext&, MapData*, MapData*, Spritesheet*)> func, MapData* map_data, MapData* map_data_2, Spritesheet* spritesheet)
		{
			if (!db_map.contains(asset_tag))
			{
				texture = func(context, map_data, map_data_2, spritesheet);
				if (texture)
				{
					db_item item;
					item.tag = asset_tag;
					item.tex = texture;
					item.type = db_item::CACHE;
					db_map.emplace(asset_tag, item);
				}
				else
				{
					return false;
				}
			}
			else
			{
				texture = db_map.at(asset_tag).tex;
			}
			return true;
		}

		bool GetTexture(SDLContext& context, std::string asset_tag, PTexture& texture, std::function<PTexture(SDLContext&, MapData*, Spritesheet*)> func, MapData* map_data, Spritesheet* spritesheet)
		{
			if (!db_map.contains(asset_tag))
			{
				texture = func(context, map_data, spritesheet);
				if (texture)
				{
					db_item item;
					item.tag = asset_tag;
					item.tex = texture;
					item.type = db_item::CACHE;
					db_map.emplace(asset_tag, item);
				}
				else
				{
					return false;
				}
			}
			else
			{
				texture = db_map.at(asset_tag).tex;
			}
			return true;
		}

		bool GetTexture(SDLContext& context, std::string asset_tag, PTexture& texture, std::function<PTexture(SDLContext&, MapData*)> func, MapData* map_data)
		{
			if (!db_map.contains(asset_tag))
			{
				texture = func(context, map_data);
				if (texture)
				{
					db_item item;
					item.tag = asset_tag;
					item.tex = texture;
					item.type = db_item::CACHE;
					db_map.emplace(asset_tag, item);
				}
				else
				{
					return false;
				}
			}
			else
			{
				texture = db_map.at(asset_tag).tex;
			}
			return true;
		}

		bool GetTexture(SDLContext& context, std::string asset_tag, PTexture& texture)
		{
			if (!db_map.contains(asset_tag))
			{
				return false;
			}
			else
			{
				texture = db_map.at(asset_tag).tex;
			}
			return true;
		}

	private:
		struct db_item
		{
			enum Type{ UNKNOWN, PROJECT, CACHE };
			Type type = Type::UNKNOWN;
			std::string path;
			std::string hash; // not currently used
			std::string tag;
			PTexture tex;
			int list_index = -1;
		};
		using p_db_item = std::shared_ptr<db_item>;

		std::map<std::string, db_item> db_map;
	};

	std::string MapLayerTag(std::string project, int map_id, std::string layer_name);

	std::string ChipsetExTag(std::string chipset_path);

	extern TextureDB _texDB;
}

namespace AssetLoader
{



	//MapData LoadMap(const ProjectData& project, int map_id);

}