#include "ProjectSelectDialog.h"
#include <chrono>
#include <thread>
#include "WindowArea.h"
#include "imgui_stdlib.h"
#include "ImguiStyle.h"

static volatile bool m_threadLock = false;
static std::vector<std::string> m_selectedPaths;

static void SDLCALL _SelectProjectFolderCallback(void* userdata, const char* const* filelist, int filter)
{
	m_threadLock = true;
	if (!filelist) {
		SDL_Log("An error occured: %s", SDL_GetError());
		m_threadLock = false;
		return;
	}
	else if (!*filelist) {
		SDL_Log("The user did not select any file.");
		SDL_Log("Most likely, the dialog was canceled.");
		m_threadLock = false;
		return;
	}
	while (*filelist) {
		SDL_Log("Full path to selected file: '%s'", *filelist);
		m_selectedPaths.push_back(std::string(*filelist));
		filelist++;
	}
	m_threadLock = false;
}

void FolderSelectDialog::Show(SDLContext& context, std::vector<std::string>& paths_to_load)
{
	m_threadLock = true;
	SDL_ShowOpenFolderDialog(_SelectProjectFolderCallback, NULL, context.Window, NULL, true);
	while (m_threadLock)
	{
		SDL_PumpEvents();
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
	}
	for (auto& path : m_selectedPaths)
	{
		paths_to_load.push_back(path);
	}
	m_selectedPaths.clear();
}

std::string FolderSelectDialog::GetSinglePath(SDLContext& context)
{
	m_threadLock = true;
	SDL_ShowOpenFolderDialog(_SelectProjectFolderCallback, NULL, context.Window, NULL, true);
	while (m_threadLock)
	{
		SDL_PumpEvents();
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
	}
	if (!m_selectedPaths.empty())
	{
		std::string path = m_selectedPaths[0];
		m_selectedPaths.clear();
		return path;
	}
	return "";
}

bool ProjectSelectDialog::GetUpdatedMaps(std::vector<int>& mapIDList, int compareIndex)
{
	if (!m_bUpdateMatchingIDs)
		return false;

	if (m_hashCombinedList.empty() || compareIndex < 1 || compareIndex > m_hashCombinedList[0].second.size())
		return false;

	for (auto& item : m_hashCombinedList)
	{
		auto& hashCollection = item.second[compareIndex];
		if (hashCollection.path.extension().string() != ".lmu")
			continue;

		if (hashCollection.diff == Diff_Update)
		{
			std::string strid = hashCollection.path.filename().string().substr(3, 4);
			mapIDList.push_back(std::stoi(strid));
		}
	}
	m_bUpdateMatchingIDs = false;
	return true;
}

void ProjectSelectDialog::ShowMainWindow(SDLContext& context)
{
	bool open = true;
	rect::FRect windowArea = warea::GetArea(context, { 0.25f, 1.0f });
	ImVec2 windowPos = { windowArea.Pos().x, windowArea.Pos().y };
	ImVec2 windowSize = { windowArea.Size().x, windowArea.Size().y };
	ImGui::SetNextWindowSize(windowSize, ImGuiCond_Appearing);
	ImGui::SetNextWindowPos(windowPos, ImGuiCond_Appearing);
	if (ImGui::Begin("ProjectHashCompare", NULL, ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar))
	{
		ShowSelectProjectOptions(context, 0, "Project A (Base)");
		//ShowSelectProjectOptions(context, 1, "Project B (Working)");
		//ShowSelectProjectOptions(context, 2, "Project C (Patch)");

		PushStyleCompact();
		//if (ImGui::CollapsingHeader("Advanced", 0))
		{
			//ShowProjectChecksum(context, 0, "Project A");
			//ShowProjectChecksum(context, 1, "Project B");
			//ShowProjectChecksum(context, 2, "Project C");
			
			//ImGui::BeginDisabled(m_projects[0].hashes.map.empty() || m_projects[1].hashes.map.empty());
			//if (ImGui::Button("Run Comparison"))
			//{
			//	std::vector<FileHashMap> hmVec;
			//	hmVec.push_back(m_projects[0].hashes.map);
			//	hmVec.push_back(m_projects[1].hashes.map);

			//	if(!m_projects[2].hashes.map.empty())
			//		hmVec.push_back(m_projects[2].hashes.map);

			//	FileHasher::CombineFileHashMaps(hmVec, m_hashCombinedList, true);
			//	m_bUpdateMatchingIDs = true;
			//	m_bShowResults = true;
			//}

			//if (!m_hashCombinedList.empty())
			//{
			//	ImGui::SameLine();
			//	ImGui::Checkbox("Show Results", &m_bShowResults);
			//}
			//ImGui::EndDisabled();
			//ImGui::Separator();
		}
		PopStyleCompact();
	}
	ImGui::End();
	if (m_bShowResults)
	{
		ShowComparisonResultWindow(context);
	}
}

void ProjectSelectDialog::SortTableWithSpecs(ImGuiTableSortSpecs* sort_specs)
{
	std::sort(m_hashCombinedList.begin(), m_hashCombinedList.end(), [&](FHPathCollectionPair a, FHPathCollectionPair b)
	{
		for (int n = 0; n < sort_specs->SpecsCount; n++)
		{
			const ImGuiTableColumnSortSpecs* sort_spec = &sort_specs->Specs[n];
			if (sort_spec)
			{
				bool result = false;
				if (sort_spec->SortDirection == ImGuiSortDirection_Ascending)
				{
					switch (sort_spec->ColumnUserID)
					{
					case ColumnID_Path:          result = a.first < b.first; break;
					case ColumnID_Hash1:		 result = a.second[0].hash < a.second[0].hash; break;
					case ColumnID_Hash2:		 result = a.second[1].hash < a.second[1].hash; break;
					default:
					case ColumnID_Status:        result = a.second[1].diff < b.second[1].diff; break;
					}
				}
				else
				{
					switch (sort_spec->ColumnUserID)
					{
					case ColumnID_Path:          result = a.first > b.first; break;
					case ColumnID_Hash1:		 result = a.second[0].hash > a.second[0].hash; break;
					case ColumnID_Hash2:		 result = a.second[1].hash > a.second[1].hash; break;
					default:
					case ColumnID_Status:        result = a.second[1].diff > b.second[1].diff; break;
					}
				}
				return result;
			}
		}
		return false;
	});
}

void ProjectSelectDialog::CalculateHashes(int project_index, bool ignoreCache)
{
	m_projects[project_index].hashes.root_path = m_projects[project_index].path;
	m_projects[project_index].hashes.display_name = m_projects[project_index].display_name;
	if (ignoreCache || !FileHasher::ReadCache(m_projects[project_index].hashes))
	{
		FileHasher::CalculateHashes(m_projects[project_index].hashes, false, true);
	}
}

void ProjectSelectDialog::ShowComparisonResultWindow(SDLContext& context)
{
	bool open = true;
	rect::FRect windowArea = warea::GetArea(context, { 0.25f, 1.0f });
	windowArea.x += windowArea.w;
	//windowArea.w *= 3.0f;
	ImVec2 windowPos = { windowArea.Pos().x, windowArea.Pos().y };
	ImVec2 windowSize = { windowArea.Size().x, windowArea.Size().y };
	ImGui::SetNextWindowSize(windowSize, ImGuiCond_Appearing);
	ImGui::SetNextWindowPos(windowPos, ImGuiCond_Appearing);
	if (ImGui::Begin("ProjectHashCompare2", NULL, ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar))
	{
		if (!m_hashCombinedList.empty())
		{
			if (ImGui::BeginTable("Hash Table", 3, ImGuiTableFlags_Resizable | ImGuiTableFlags_NoSavedSettings | ImGuiTableFlags_Sortable))
			{
				ImGui::TableSetupColumn("Path", ImGuiTableColumnFlags_WidthStretch | ImGuiTableColumnFlags_DefaultSort, 0.0f, 0);
				ImGui::TableSetupColumn("A-B Diff", ImGuiTableColumnFlags_WidthFixed, 0.0f, 1);
				ImGui::TableSetupColumn("A-C Diff", ImGuiTableColumnFlags_WidthFixed, 0.0f, 2);
				//ImGui::TableSetupColumn("Hash A", ImGuiTableColumnFlags_WidthStretch | ImGuiTableColumnFlags_DefaultHide, 0.0f, 2);
				//ImGui::TableSetupColumn("Hash B", ImGuiTableColumnFlags_WidthStretch | ImGuiTableColumnFlags_DefaultHide, 0.0f, 3);
				ImGui::TableHeadersRow();
				
				// Sort our data if sort specs have been changed!
				ImGuiTableSortSpecs* sort_specs = ImGui::TableGetSortSpecs();
				if (sort_specs && sort_specs->SpecsDirty)
				{
					SortTableWithSpecs(sort_specs);
					sort_specs->SpecsDirty = false;
				}

				ImGuiListClipper clipper;
				clipper.Begin(m_hashCombinedList.size());
				while (clipper.Step())
				{
					int offset = 0;
					for (int row_n = clipper.DisplayStart; row_n < clipper.DisplayEnd; row_n++)
					{
						ImGui::PushID(row_n);
						ImGui::TableNextRow(ImGuiTableRowFlags_None, 0.0f);
					
						std::string abStatus;
						std::string acStatus;
						ImU32 ab_cell_bg_color;
						ImU32 ac_cell_bg_color;

						switch (m_hashCombinedList[row_n].second[1].diff)
						{
						case Diff_Add:
							abStatus = "added";
							ab_cell_bg_color = ImGui::GetColorU32(ImVec4(0.2f, 0.8f, 0.2f, 0.80f));
							break;
						case Diff_Delete:
							abStatus = "deleted";
							ab_cell_bg_color = ImGui::GetColorU32(ImVec4(0.8f, 0.2f, 0.2f, 0.80f));
							break;
						case Diff_Update:
							abStatus = "updated";
							ab_cell_bg_color = ImGui::GetColorU32(ImVec4(0.8f, 0.8f, 0.2f, 0.80f));
							break;
						default:
						case Diff_Same:
							abStatus = "";
							ab_cell_bg_color = ImGui::GetColorU32(ImVec4(0.1f, 0.1f, 0.1f, 0.80f));
							break;
						}

						switch (m_hashCombinedList[row_n].second[2].diff)
						{
						case Diff_Add:
							acStatus = "added";
							ac_cell_bg_color = ImGui::GetColorU32(ImVec4(0.2f, 0.8f, 0.2f, 0.80f));
							break;
						case Diff_Delete:
							acStatus = "deleted";
							ac_cell_bg_color = ImGui::GetColorU32(ImVec4(0.8f, 0.2f, 0.2f, 0.80f));
							break;
						case Diff_Update:
							acStatus = "updated";
							ac_cell_bg_color = ImGui::GetColorU32(ImVec4(0.8f, 0.8f, 0.2f, 0.80f));
							break;
						default:
						case Diff_Same:
							acStatus = "";
							ac_cell_bg_color = ImGui::GetColorU32(ImVec4(0.1f, 0.1f, 0.1f, 0.80f));
							break;
						}

						ImGui::TableSetBgColor(ImGuiTableBgTarget_CellBg, ab_cell_bg_color, 1);
						ImGui::TableSetBgColor(ImGuiTableBgTarget_CellBg, ac_cell_bg_color, 2);

						ImGui::TableSetColumnIndex(0);
						ImGui::TextUnformatted(m_hashCombinedList[row_n].first.c_str());
						ImGui::TableSetColumnIndex(1);
						ImGui::TextUnformatted(abStatus.c_str());
						ImGui::TableSetColumnIndex(2);
						ImGui::TextUnformatted(acStatus.c_str());
						//ImGui::TableSetColumnIndex(2);
						//ImGui::TextUnformatted(m_hashCombinedList[row_n].second[0].hash.c_str());
						//ImGui::TableSetColumnIndex(3);
						//ImGui::TextUnformatted(m_hashCombinedList[row_n].second[1].hash.c_str());
						ImGui::PopID();
					}
				}
				ImGui::EndTable();
			}
		}
	}
	ImGui::End();
}

void ProjectSelectDialog::ShowSelectProjectOptions(SDLContext& context, int projIndex, std::string uiName)
{
	ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 5.0f);
	if (ImGui::BeginChild((uiName + "sf").c_str(), ImVec2(304.0f, 74.0f), ImGuiChildFlags_Borders | ImGuiChildFlags_AlwaysAutoResize | ImGuiChildFlags_AutoResizeX | ImGuiChildFlags_AutoResizeY, 0))
	{
		std::string pushid = "selpjopt" + uiName;
		ImGui::PushID(pushid.c_str());
		bool open = true;
		bool updatePath = false;

		ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.5f, 0.5f, 0.5f, 1.0f));
		ImGui::TextUnformatted(uiName.c_str());
		ImGui::PopStyleColor();

		if (ImGui::InputText("", &m_projects[projIndex].path))
		{
			updatePath = true;
		}
		ImGui::SameLine();
		if (ImGui::Button("...", { 30, 18 }))
		{
			m_projects[projIndex].path = m_folderSelectDlg.GetSinglePath(context);
			updatePath = true;
		}
		
		if (updatePath)//!m_projects[projIndex].display_name.empty())
		{
			fs::path fspath = m_projects[projIndex].path;
			m_projects[projIndex].display_name = fmt::format("{}_{}", fspath.parent_path().filename().string(), fspath.filename().string());
			m_projects[projIndex].hashes.root_path = m_projects[projIndex].path;
			m_projects[projIndex].hashes.display_name = m_projects[projIndex].display_name;
			FileHasher::ReadCache(m_projects[projIndex].hashes);

			if (m_projectGui)
			{
				m_projectGui->LoadProject(m_projects[projIndex].path);
			}
		}

		ImGui::TextUnformatted(m_projects[projIndex].display_name.c_str());
		ImGui::PopID();
	}
	ImGui::EndChild();
	ImGui::Separator();
	ImGui::PopStyleVar(1);
}

void ProjectSelectDialog::ShowProjectChecksum(SDLContext& context, int projIndex, std::string uiName)
{
	ImGui::PushID(std::hash<std::string>{}("selpjcheck" + uiName));

	ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.5f, 0.5f, 0.5f, 1.0f));
	ImGui::TextUnformatted(uiName.c_str());
	ImGui::PopStyleColor();
	if (m_projects[projIndex].path.empty())
	{
		ImGui::SameLine();
		ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.8f, 0.2f, 0.2f, 1.0f));
		ImGui::TextUnformatted("(not loaded)");
		ImGui::PopStyleColor();
	}

	ImGui::BeginDisabled(m_projects[projIndex].path.empty());

	if (!m_projects[projIndex].hashes.map.empty())
	{
		ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.2f, 0.8f, 0.2f, 1.0f));
		ImGui::TextUnformatted("checksum: ok");
		ImGui::PopStyleColor();
	}
	else
	{
		ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.8f, 0.2f, 0.2f, 1.0f));
		ImGui::TextUnformatted("checksum: not found");
		ImGui::PopStyleColor();
	}

	if (ImGui::Button("get checksum"))
		CalculateHashes(projIndex);

	ImGui::SameLine();
	if (ImGui::Button("delete cache"))
		FileHasher::DeleteCache(m_projects[projIndex].hashes);

	ImGui::EndDisabled();
	ImGui::Separator();
	ImGui::PopID();
}



//std::string comboOptions = "\0";
//for (auto& rmPath : m_rememberPaths)
//{
//	comboOptions += rmPath.projectNickname + "\0";
//}
//ImGui::PushID(std::hash<std::string>{}("combo"));
//if (ImGui::Combo("", (int*)&m_rememberIndex, comboOptions.c_str()))
//{
//	if (m_rememberIndex != 0)
//	{
//		m_projects[projIndex].path = m_rememberPaths[m_rememberIndex - 1].projectPath;
//		fs::path fspath = m_projects[projIndex].path;
//		m_projects[projIndex].display_name = fmt::format("{}_{}", fspath.parent_path().filename().string(), fspath.filename().string());
//	}
//}
//ImGui::PopID();