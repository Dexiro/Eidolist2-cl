#include "ProjectGui.h"
#include "imgui.h"
#include "imgui_stdlib.h"
#include "ImguiStyle.h"
#include "MapLayers.h"

ProjectGui::ProjectGui(int id)
{
	m_bOpen = true;
	m_id = id;
	m_expansionLevel = 0;
	//m_selectedTab = { true, false, false };
}

void ProjectGui::Show(SDLContext& context)
{
	if (!m_data) return;


	bool open = true;
	std::string windowName = "Map Tree";

	rect::FRect windowArea;
	if (m_expansionLevel == 0)
		windowArea = warea::GetArea(context, { 0.25f, 1.0f });
	else if (m_expansionLevel >= 0)
		windowArea = warea::GetArea(context, { 0.50f, 1.0f });

	ImVec2 windowPos = { windowArea.Pos().x, windowArea.Pos().y };
	ImVec2 windowSize = { windowArea.Size().x, windowArea.Size().y };
	ImGui::SetNextWindowSize(windowSize);
	ImGui::SetNextWindowPos(windowPos);
	if (!ImGui::Begin(windowName.c_str(), NULL, ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar))
	{
		// Early out if the window is collapsed, as an optimization.
		ImGui::End();
		return;
	}

	ImGui::TextUnformatted("Map Tree");
	ImGui::SameLine();
	ImGui::Spacing();
	ImGui::SameLine();

	if(m_expansionLevel > 0)
		if (ImGui::Button("< Shrink"))
			m_expansionLevel--;

	if (m_expansionLevel < 1)
		if (ImGui::Button("Expand >"))
			m_expansionLevel++;

	m_expansionLevel = std::clamp(m_expansionLevel, 0, 1);


	bool updateFilter = false;
	if (ImGui::InputText("Filter", &m_currentFilter))
		updateFilter = true;

	if (updateFilter || m_initFilter)
	{
		m_initFilter = false;
		std::string filterLowerCase = to_lower(m_currentFilter);
		m_filteredList.clear();
		for (int row_n = 0; row_n < NMaps(); row_n++)
		{
			if (m_currentFilter.empty())
			{
				m_filteredList.push_back(row_n);
			}
			else
			{
				MapData* map_data = GetMapData(row_n);
				if (!map_data)
					continue;

				std::string lcName = to_lower(map_data->map_name);
				if (lcName.find(filterLowerCase, 0) != std::string::npos)
				{
					m_filteredList.push_back(row_n);
					continue;
				}

				lcName = to_lower(map_data->wikiLocationData.primaryAuthor);
				if (!lcName.empty() && lcName.find(filterLowerCase, 0) != std::string::npos)
				{
					m_filteredList.push_back(row_n);
					continue;
				}

				lcName = to_lower(map_data->wikiLocationData.name);
				if (!lcName.empty() && lcName.find(filterLowerCase, 0) != std::string::npos)
				{
					m_filteredList.push_back(row_n);
					continue;
				}
			}
		}
	}


	static const ImGuiTableFlags table_flags = ImGuiTableFlags_BordersV | ImGuiTableFlags_BordersOuterH | ImGuiTableFlags_Resizable | ImGuiTableFlags_RowBg | ImGuiTableFlags_NoBordersInBody | ImGuiTableFlags_ScrollY;
	//static const ImGuiTreeNodeFlags tree_node_flags_base = ImGuiTreeNodeFlags_SpanAllColumns | ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_DrawLinesFull;

	//std::string childName = fmt::format("mtreechild{}", m_id);
	//if (ImGui::BeginChild(std::hash<std::string>{}(childName), ImGui::GetContentRegionAvail(), ImGuiChildFlags_None, 0))
	{
		PushStyleCompact();
		if (ImGui::BeginTable(windowName.c_str(), 5, table_flags))
		{
			ImGui::TableSetupColumn("ID", ImGuiTableColumnFlags_WidthFixed | ImGuiTableColumnFlags_DefaultSort | ImGuiTableColumnFlags_DefaultHide | ImGuiTableColumnFlags_Disabled, 0.0f, ColumnID_ID);
			ImGui::TableSetupColumn("Name", ImGuiTableColumnFlags_WidthStretch | ImGuiTableColumnFlags_NoHide, 0.0f, ColumnID_Name);

			ImGuiTableColumnFlags flags = 0;
			if (m_expansionLevel < 1)
				flags = ImGuiTableColumnFlags_Disabled;
			ImGui::TableSetupColumn("World", ImGuiTableColumnFlags_WidthFixed | flags, 0.0f, ColumnID_World);
			ImGui::TableSetupColumn("Author", ImGuiTableColumnFlags_WidthFixed | flags, 0.0f, ColumnID_Author);
			ImGui::TableSetupColumn("Options", ImGuiTableColumnFlags_WidthFixed, 0.0f, ColumnID_Action);
			ImGui::TableHeadersRow();

			ImGuiListClipper clipper;
			clipper.Begin(m_filteredList.size());
			//clipper.IncludeItemByIndex
			while (clipper.Step())
			{
				for (int row_n = clipper.DisplayStart; row_n < clipper.DisplayEnd; row_n++)
				{
					MapData* map_data = GetMapData(m_filteredList[row_n]);

					const bool item_is_selected = m_selection.contains(map_data->map_id);
					ImGui::PushID(map_data->map_id);

					ImGui::TableNextRow(ImGuiTableRowFlags_None, 0.0f);
					ImGui::TableSetColumnIndex(ColumnID_ID);
					char label[32];
					sprintf(label, "%04d", map_data->map_id);
					ImGui::TextUnformatted(label);

					if (ImGui::TableSetColumnIndex(ColumnID_Name))
						ImGui::TextUnformatted(map_data->map_name.c_str());

					if (m_expansionLevel > 0)
					{
						ImGui::PushStyleColor(ImGuiCol_Text, { 0.5f, 0.5f, 1.0f, 1.0f });
						if (ImGui::TableSetColumnIndex(ColumnID_World))
							ImGui::TextUnformatted(map_data->wikiLocationData.name.c_str());
						ImGui::PopStyleColor();

						ImGui::PushStyleColor(ImGuiCol_Text, { 0.5f, 1.0f, 0.5f, 1.0f });
						if (ImGui::TableSetColumnIndex(ColumnID_Author))
							ImGui::TextUnformatted(map_data->wikiLocationData.primaryAuthor.c_str());
						ImGui::PopStyleColor();
					}

					if (ImGui::TableSetColumnIndex(ColumnID_Action))
					{
						MapObj* mapObj = nullptr;
						for (int i = 0; i < NMapsLoaded(); i++)
						{
							if (map_data->map_id == m_loadedMaps[i]->m_mapData->map_id)
								mapObj = m_loadedMaps[i].get();
						}

						if (!mapObj)
						{
							if (ImGui::SmallButton("Load"))
							{
								glm::vec2 pos(0.0f, 0.0f);
								LoadMapObj(context, map_data->map_id, pos);
							}
						}
					}

					ImGui::PopID();
				}
			}
		}
		ImGui::EndTable();
		PopStyleCompact();
	}
	//ImGui::EndChild();
	ImGui::End();
}
