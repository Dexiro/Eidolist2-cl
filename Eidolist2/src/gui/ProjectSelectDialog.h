#pragma once
#include "CommonIncludes.h"
#include "ProjectGui.h"
#include "sha1.h"
#include "md5.h"
#include "FileHasher.h"

class FolderSelectDialog
{
	bool m_open = true;
public:

	void Show(SDLContext& context, std::vector<std::string>& paths_to_load);
	std::string GetSinglePath(SDLContext& context);
};

struct ProjectInfo
{
	std::string path;
	std::string display_name = "_";
	DirectoryHashes hashes;
	bool refresh_cache = false;
};

class ProjectSelectDialog
{
public:
	ProjectSelectDialog(ProjectGui* projectGui) : m_projectGui(projectGui) {}
	
protected:
	ProjectGui* m_projectGui;

	bool m_bShowResults = false;
	
	bool m_open = true;
	std::array<ProjectInfo, 3> m_projects;
	FileHashListCombined m_hashCombinedList;

	bool m_bUpdateMatchingIDs = false;

	FolderSelectDialog m_folderSelectDlg;
	
	enum ColumnIDs
	{
		ColumnID_Path,
		ColumnID_Status,
		ColumnID_Hash1,
		ColumnID_Hash2
	};
	void SortTableWithSpecs(ImGuiTableSortSpecs* sort_specs);
	void CalculateHashes(int project_index, bool ignoreCache = false);

public:

	void ShowMainWindow(SDLContext& context);

	void ShowComparisonResultWindow(SDLContext& context);
	void ShowSelectProjectOptions(SDLContext& context, int projIndex, std::string uiName);
	void ShowProjectChecksum(SDLContext& context, int projIndex, std::string uiName);

	bool GetUpdatedMaps(std::vector<int>& mapIDList, int compareIndex = 1);
	

};