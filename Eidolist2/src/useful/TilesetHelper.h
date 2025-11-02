#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <memory>
#include "GLMIncludes.h"

namespace tsh
{
	template<typename T>
	bool CollapseArray2D(const std::vector<T>& array, std::vector<std::vector<T>>& array2d, int width, int height, int columnWidth = -1)
	{
		if (columnWidth == -1)
			columnWidth = width;

		int columnStride = columnWidth * height;


		if (array.size() != width * height)
		{
			std::cout << "CollapseArray2D: Incorrect height and/or width" << std::endl;
			return false;
		}

		for (int y = 0; y < height; y++)
		{
			std::vector<T> row;
			for (int x = 0; x < width; x++)
			{
				int columnNum = x / columnWidth;
				int arrayPos = (columnNum * columnStride) + (x % columnWidth) + (y * columnWidth);

				row.push_back(array.at(arrayPos));
			}
			array2d.push_back(row);
		}
		return true;
	};

	struct CopyRect
	{
		glm::ivec2 src_pos = glm::ivec2(0);
		glm::ivec2 src_size = glm::ivec2(0);
		glm::ivec2 dst_pos = glm::ivec2(0);
		glm::ivec2 dst_size = glm::ivec2(0);
	};

	namespace autotile
	{
		static const std::vector<int> c_autoSetIds = { -1, -1, 4, 5, -1, -1, 6, 7, 0, 1, 8, 9, 2, 3, 10, 11 };
		int remap_setid(int setID);


	}

	glm::ivec2 IndexToGridPos(int index, int gridW, int gridH);

	struct TileSetIDs
	{
		enum class Type
		{
			animA,
			animB,
			animC,
			autoD,
			normal
		};

		std::vector<int16_t> m_tilesetE;
		std::vector<int16_t> m_tilesetD;
		glm::ivec2 m_tsGridSizeE = glm::ivec2(0);
		glm::ivec2 m_tsGridSizeD = glm::ivec2(0);
		glm::ivec2 m_tsGridOriginE = glm::ivec2(0);
		glm::ivec2 m_tsGridOriginD = glm::ivec2(0);


		void Load(std::string configFolder);

		void LoadTileIDs(std::string filename, std::vector<int16_t>& idList);

		void GetTilePos_Normal(int16_t tileID, glm::ivec2& pos);

		void GetTilePos_AutoD(int16_t tileID, glm::ivec2& pos);

		void GetTilePos(Type type, int16_t tileID, glm::ivec2& pos);
		void GetTilePos(int16_t tileID, glm::ivec2& pos);
	};

	void GetAutotileDRects(std::vector<std::vector<glm::ivec2>>& autoTileMap, std::vector<std::vector<int>>& autoTileCfgs, std::vector<tsh::CopyRect>& copyRect);

	void GetSetDBlueprint(std::vector<std::vector<glm::ivec2>>& setDBlueprint);

	void GetSubtileDBlueprint(std::string configFolder, std::vector<std::vector<int>>& subtileDBlueprint);

}


