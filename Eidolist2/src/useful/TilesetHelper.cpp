#include "TilesetHelper.h"

int tsh::autotile::remap_setid(int setID)
{
	for (int i = 0; i < c_autoSetIds.size(); i++)
	{
		if (c_autoSetIds[i] == setID)
			return i;
	}
	return 0;
}

void tsh::TileSetIDs::Load(std::string configFolder)
{
	LoadTileIDs(configFolder + "\\TilesetD.txt", m_tilesetD);
	LoadTileIDs(configFolder + "\\TilesetE.txt", m_tilesetE);
	m_tsGridSizeE = glm::ivec2(18, 16);
	m_tsGridOriginE = glm::ivec2(12, 0);
	m_tsGridSizeD = glm::ivec2(48, 16);
	m_tsGridOriginD = glm::ivec2(30, 0);
}

void tsh::TileSetIDs::LoadTileIDs(std::string filepath, std::vector<int16_t>& idList)
{
	std::ifstream infile(filepath);
	std::string line;
	while (std::getline(infile, line))
	{
		std::istringstream iss(line);
		std::string id;
		while (std::getline(iss, id, ','))
		{
			if (!id.empty())
				idList.push_back(std::stoi(id));
		}
	}
}

void tsh::TileSetIDs::GetTilePos_Normal(int16_t tileID, glm::ivec2& pos)
{
	for (int i = 0; i < m_tilesetE.size(); i++)
	{
		if (m_tilesetE[i] == tileID)
		{
			pos = IndexToGridPos(i, m_tsGridSizeE.x, m_tsGridSizeE.y);
			pos += m_tsGridOriginE;
		}
	}
}

void tsh::TileSetIDs::GetTilePos_AutoD(int16_t tileID, glm::ivec2& pos)
{
	//=4000+(FLOOR(X/3) * 50)+(Y)+((X%3)*16)
	for (int i = 0; i < m_tilesetD.size(); i++)
	{
		if (m_tilesetD[i] == tileID)
		{
			pos = IndexToGridPos(i, m_tsGridSizeD.x, m_tsGridSizeD.y);
			pos += m_tsGridOriginD;
		}
	}

}

void tsh::TileSetIDs::GetTilePos(int16_t tileID, glm::ivec2& pos)
{
	if (tileID >= 5000)
	{
		GetTilePos(Type::normal, tileID, pos);
	}
	else if (tileID >= 4000)
	{
		GetTilePos(Type::autoD, tileID, pos);
	}
	else
	{
		pos = glm::ivec2(0, 4);
		// Placeholder
	}
}

void tsh::TileSetIDs::GetTilePos(Type type, int16_t tileID, glm::ivec2& pos)
{
	switch (type)
	{
	case Type::normal:
		GetTilePos_Normal(tileID, pos);
		break;
	case Type::autoD:
		GetTilePos_AutoD(tileID, pos);
		break;
	case Type::animA:
		break;
	case Type::animB:
		break;
	case Type::animC:
		break;
	default:
		break;
	}
}

glm::ivec2 tsh::IndexToGridPos(int index, int gridW, int gridH)
{
	glm::ivec2 pos;
	pos.x = index % gridW;
	pos.y = index / gridW;
	return pos;
}

void tsh::GetAutotileDRects(std::vector<std::vector<glm::ivec2>>& autoTileMap, std::vector<std::vector<int>>& autoTileCfgs, std::vector<tsh::CopyRect>& copyRect)
{
	std::vector<glm::ivec2> offsets;
	offsets.push_back(glm::ivec2(0, 0));
	offsets.push_back(glm::ivec2(8, 0));
	offsets.push_back(glm::ivec2(8, 8));
	offsets.push_back(glm::ivec2(0, 8));

	glm::ivec2 atOrigin(30 * 16, 0);
	for (int iSet = 0; iSet < autoTileMap.size(); iSet++)
	{
		//int setID = tsh::autotile::remap_setid(iSet);
		glm::ivec2 setOrigin = atOrigin + glm::ivec2(iSet * 48, 0);
		auto& atSet = autoTileMap[iSet];
		for (int iCfg = 0; iCfg < autoTileCfgs.size(); iCfg++)
		{
			glm::ivec2 tileOrigin = setOrigin + glm::ivec2(std::floor(iCfg / 16) * 16, (iCfg % 16) * 16);
			auto& cfg = autoTileCfgs[iCfg];
			for (int iPart = 0; iPart < cfg.size(); iPart++)
			{
				glm::ivec2 size = glm::ivec2(8, 8);

				glm::ivec2 srcPos(atSet[cfg[iPart]].x, atSet[cfg[iPart]].y);
				srcPos += offsets[iPart];
				glm::ivec2 dstPos = tileOrigin + offsets[iPart];

				tsh::CopyRect cpr;
				cpr.src_pos = srcPos;
				cpr.src_size = size;
				cpr.dst_pos = dstPos;
				cpr.dst_size = size;
				copyRect.push_back(cpr);
			}
		}
	}
}

void tsh::GetSetDBlueprint(std::vector<std::vector<glm::ivec2>>& setDBlueprint)
{
	glm::ivec2 atSize = glm::ivec2(48, 64);
	std::vector<glm::ivec2> atOrigins;
	atOrigins.push_back(glm::ivec2(0, 2) * atSize);
	atOrigins.push_back(glm::ivec2(1, 2) * atSize);
	atOrigins.push_back(glm::ivec2(0, 3) * atSize);
	atOrigins.push_back(glm::ivec2(1, 3) * atSize);
	atOrigins.push_back(glm::ivec2(2, 0) * atSize);
	atOrigins.push_back(glm::ivec2(3, 0) * atSize);
	atOrigins.push_back(glm::ivec2(2, 1) * atSize);
	atOrigins.push_back(glm::ivec2(3, 1) * atSize);
	atOrigins.push_back(glm::ivec2(2, 2) * atSize);
	atOrigins.push_back(glm::ivec2(3, 2) * atSize);
	atOrigins.push_back(glm::ivec2(2, 3) * atSize);
	atOrigins.push_back(glm::ivec2(3, 3) * atSize);

	// for each set of Autotiles
	for (auto& origin : atOrigins)
	{
		std::vector<glm::ivec2> tiles;
		for (int ty = 0; ty < 4; ty++)
		{
			for (int tx = 0; tx < 3; tx++)
			{
				int x = origin.x + (tx * 16);
				int y = origin.y + (ty * 16);
				tiles.push_back(glm::ivec2(x, y));
			}
		}
		setDBlueprint.push_back(tiles);
	}
}

void tsh::GetSubtileDBlueprint(std::string configFolder, std::vector<std::vector<int>>& subtileDBlueprint)
{
	std::ifstream infile(configFolder + "AutoTile.txt");// TEMPCONFIGS + "AutoTile.txt");
	std::string line;
	while (std::getline(infile, line))
	{
		std::istringstream iss(line);
		std::string index, tileIDs;
		std::getline(iss, index, ':');
		std::getline(iss, tileIDs, ':');
		std::istringstream iss2(tileIDs);
		std::string tileID;
		std::vector<int> tileIDList;
		while (std::getline(iss2, tileID, ','))
		{
			if (!tileID.empty())
			{
				tileIDList.push_back(std::stoi(tileID));
			}
		}
		subtileDBlueprint.push_back(tileIDList);
	}
}
