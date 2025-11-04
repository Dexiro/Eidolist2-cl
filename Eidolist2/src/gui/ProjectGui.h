#pragma once
#include "StdIncludes.h"
#include "SDLTypes.h"

#include "Project.h"
#include "MapObj.h"

static std::string to_lower(std::string str)
{
	std::transform(str.begin(), str.end(), str.begin(), [](unsigned char c) { return std::tolower(c); });
	return str;
}

class ProjectGui : public Project
{
public:
	ProjectGui(int id);
	virtual void Show(SDLContext& context);

protected:
	enum ColumnID
	{
		ColumnID_ID,
		ColumnID_Name,
		ColumnID_World,
		ColumnID_Author,
		ColumnID_Action,
	};

	ImVector<int> m_selection;

	//std::array<bool, 3> m_selectedTab;

	bool m_bOpen;
	int m_id = 0;
	bool m_initFilter = true;

	std::vector<int> m_filteredList;
	std::string m_currentFilter = "";

	int m_expansionLevel = 0;
};
using PProjectGui = std::shared_ptr<ProjectGui>;