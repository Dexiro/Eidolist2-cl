#include "AssetLoader.h"

db::TextureDB db::_texDB;












//namespace AssetLoader
//{
//
//	MapData LoadMap(const ProjectData& project, int map_id)
//	{
//		if (map_id == 731)
//			map_id++;
//
//
//
//		MapData map_data;
//		map_data.map_id = map_id;
//		map_data.map_filename = fmt::format("Map{:0{}}", map_id, 4);
//
//		std::string lmu_path = fmt::format("{}{}/{}.lmu", project.directory, AssetTypePath[AssetType::atMap], map_data.map_filename);
//
//		map_data.lcfMap = std::move(lcf::LMU_Reader::Load(lmu_path, "UTF-8"));
//		map_data.lmu_path = lmu_path;
//		map_data.chipset_path = "";
//		map_data.map_name = project.lcfMT->maps[map_id].name;
//
//		int32_t chipset_id = map_data.lcfMap->chipset_id;
//		lcf::rpg::Chipset chipset = project.lcfDB->chipsets[chipset_id - 1];
//		lcf::DBString name = chipset.chipset_name;
//		std::string chipset_name = name.c_str();
//
//		map_data.chipset_path = fmt::format("{}{}/{}.png", project.directory, AssetTypePath[AssetType::atChipSet], chipset_name);
//
//		ByteMapFlat passability_data;
//		passability_data.insert(passability_data.end(), chipset.passable_data_lower.begin() + 18, chipset.passable_data_lower.end());
//		passability_data.insert(passability_data.end(), chipset.passable_data_upper.begin(), chipset.passable_data_upper.end());
//
//		ByteMapFlat autoTilePass_data;
//		autoTilePass_data.insert(autoTilePass_data.end(), chipset.passable_data_lower.begin(), chipset.passable_data_lower.begin() + 18);
//
//		lcf::DBString parallax_name = map_data.lcfMap->parallax_name;
//		map_data.parallax_path = fmt::format("{}{}/{}.png", project.directory, AssetTypePath[AssetType::atPanorama], std::string(parallax_name.c_str()));
//
//		tsh::CollapseArray2D<int16_t>(map_data.lcfMap->lower_layer, map_data.lower_layer, map_data.lcfMap->width, map_data.lcfMap->height);
//		tsh::CollapseArray2D<int16_t>(map_data.lcfMap->upper_layer, map_data.upper_layer, map_data.lcfMap->width, map_data.lcfMap->height);
//
//		tsh::CollapseArray2D<uint8_t>(passability_data, map_data.terrain_passability_layer, 18, 16);
//		//tsh::CollapseArray2D<int16_t>(map_data.lcfMap->upper_layer, map_data.lower_passability_layer, 16, map_data.lcfMap->height);
//
//		// 18
//		return map_data;
//	}
//
//}
//

//std::ofstream myfile;
//myfile.open("passabilityOut.txt");
//for (auto& ch : passability_data)
//{
//	std::bitset<8> bits = ch;
//	myfile << bits.to_string() << "\n";
//}
//myfile.close();

std::string db::MapLayerTag(std::string project, int map_id, std::string layer_name)
{
	return fmt::format("{}{}{}", project, map_id, layer_name);
}

std::string db::ChipsetExTag(std::string chipset_path)
{
	return fmt::format("{}EX", chipset_path);
}
