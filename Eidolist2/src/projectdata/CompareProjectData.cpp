#include "CompareProjectData.h"
#include "Rect.h"
#include "WindowArea.h"



void CompareDataGui::ShowMainWindow(SDLContext& context, PProjectGui& project)
{
	bool open = true;
	rect::FRect windowArea = warea::GetArea(context, { 1.0f, 1.0f });
	ImVec2 windowPos = { windowArea.Pos().x, windowArea.Pos().y };
	ImVec2 windowSize = { windowArea.Size().x, windowArea.Size().y };
	ImGui::SetNextWindowSize(windowSize, ImGuiCond_Appearing);
	ImGui::SetNextWindowPos(windowPos, ImGuiCond_Appearing);
	if (ImGui::Begin("CompareDataGui", NULL, ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar))
	{
		if (ImGui::BeginChild("CompGuiCh1", ImVec2(windowSize.x / 2.0f, 700.0f), ImGuiChildFlags_Borders | ImGuiChildFlags_AlwaysAutoResize | ImGuiChildFlags_AutoResizeX | ImGuiChildFlags_AutoResizeY, 0))
		{
			list_data::DisplayList(project->Data(0)->commonEvents);
		}
		ImGui::EndChild();
	}
	ImGui::End();
}
