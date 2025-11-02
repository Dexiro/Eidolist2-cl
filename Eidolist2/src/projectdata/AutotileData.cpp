#include "AutotileData.h"

AutoTileBlueprint global::_atData;

void AutoTileBlueprint::Load()
{
	tilesetIDs.Load(TEMPCONFIGS);
	tsh::GetSubtileDBlueprint(TEMPCONFIGS, subtileDBlueprint);
	tsh::GetSetDBlueprint(setDBlueprint);
	tsh::GetAutotileDRects(setDBlueprint, subtileDBlueprint, setDCompileRects);

	// Temp? Add base chipset to the CPR list
	tsh::CopyRect baseChipsetCPR;
	glm::ivec2 chipSize = glm::ivec2(480, 256); // Eh just hardcode values for now
	baseChipsetCPR.src_size = chipSize;
	baseChipsetCPR.dst_size = chipSize;
	setDCompileRects.push_back(baseChipsetCPR);
}
