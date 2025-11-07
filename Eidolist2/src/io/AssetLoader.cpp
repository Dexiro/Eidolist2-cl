#include "AssetLoader.h"

db::TextureDB db::_texDB;

std::string db::GetAssetTag(std::string project, int map_id, std::string layer_name)
{
	return fmt::format("{}{}{}", project, map_id, layer_name);
}

std::string db::GetAssetTag(std::string chipset_path)
{
	return fmt::format("{}EX", chipset_path);
}
