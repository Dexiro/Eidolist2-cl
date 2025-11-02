// eidolist2.h : Include file for standard system include files,
// or project specific include files.

#pragma once

#include <iostream>
#include "src/SDLTypes.h"

#include "imgui.h"
#include "imgui_impl_sdl3.h"
#include "imgui_impl_sdlrenderer3.h"


// TODO: Reference additional headers your program requires here.

void InitSDL(bool& bInitSDL, SDLContext& context);

//void CheckLoadProjects(int& projectID, std::vector<std::shared_ptr<IGuiMenu>>& imguiMenus, std::vector<std::shared_ptr<LoadedProject>>& loadedProjects);

//void CheckLoadMaps(std::vector<std::shared_ptr<LoadedProject>>& loadedProjects, SDLContext& context, AutoTileBlueprint& atBlueprint, vmath_hpp::fvec2& mapCaret);

//void OutputMapIds(MapData& map);


//void OutputMapIds(MapData& map)
//{
//	std::ofstream fout("out.txt");
//	fout << "Upper Layer:" << std::endl;
//	for (auto row : map.upper_layer)
//	{
//		for (auto tile : row)
//		{
//			fout << fmt::format("{:6},", std::to_string(tile));
//		}
//		fout << std::endl;
//	}
//	fout << std::endl << "Lower Layer:" << std::endl;
//	for (auto row : map.lower_layer)
//	{
//		for (auto tile : row)
//		{
//			fout << fmt::format("{:6},", std::to_string(tile));
//		}
//		fout << std::endl;
//	}
//}