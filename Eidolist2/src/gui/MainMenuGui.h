#pragma once
#include "IGuiMenu.h"

#include "../system/MessageHandler.h"
#include "../obj/MapObj.h"

#include <map>
#include <vector>
#include <memory>
#include <cstddef>
#include <iostream>
#include <fstream>
#include <fmt/format.h>

//struct MainMenuGui : public IGuiMenu, public MsgProvider
//{
//	//SDL_Window* m_window = nullptr;
//	std::vector<std::shared_ptr<LoadedProject>>* m_loadedProjects = nullptr;
//	int m_currentProject = -1;
//	int m_selected = 0;
//
//	MainMenuGui(std::vector<std::shared_ptr<LoadedProject>>* loadedProjects);
//
//	virtual void Show();
//};