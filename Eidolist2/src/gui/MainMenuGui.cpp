#include "MainMenuGui.h"
#include "../../external/imgui/imgui.h"


//MainMenuGui::MainMenuGui(std::vector<std::shared_ptr<LoadedProject>>* loadedProjects)//, SDL_Window* window)
//{
//	//m_window = window;
//	m_loadedProjects = loadedProjects;
//	m_currentProject = -1;
//}
//
//void MainMenuGui::Show()
//{
//	if (ImGui::BeginMainMenuBar())
//	{
//		std::string menuText = fmt::format("[Projects ({})]", (int)m_loadedProjects->size());
//		if (ImGui::BeginMenu(menuText.c_str()))
//		{
//			for (int i = 0; i < m_loadedProjects->size(); i++)
//			{
//				LoadedProject* proj = (*m_loadedProjects)[i].get();
//				std::string dir = proj->m_projectData->Directory();
//
//				if (ImGui::MenuItem(dir.c_str(), "", &m_selected)) {}
//			}
//			ImGui::EndMenu();
//		}
//
//		ImGui::Separator();
//		if (ImGui::Button("Load Project"))
//		{
//			//if (m_window)
//			//	SDL_ShowOpenFolderDialog(callback, NULL, m_window, NULL, true);
//			notify(MT_OPEN_FOLDER);
//		}
//
//		ImGui::Separator();
//
//		if (ImGui::Checkbox("light mode", &g_appSettings.light_mode))
//		{
//			if (g_appSettings.light_mode)
//			{
//				ImGui::StyleColorsLight();
//			}
//			else
//			{
//				ImGui::StyleColorsDark();
//			}
//		}
//		ImGui::Checkbox("gui demo", &g_appSettings.show_demo_window);
//		ImGui::EndMainMenuBar();
//	}
//}
