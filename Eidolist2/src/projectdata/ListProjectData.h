#pragma once
#include "CommonIncludes.h"
#include "LCFTypes.h"


namespace list_data
{
	struct ListCommand
	{
		lcf::rpg::EventCommand data;
		std::string commandName;
		std::string string;
		std::string commandDisplayStr;
		int indent = 0;

		ListCommand(const lcf::rpg::EventCommand& _data);
	};

	struct ListItem
	{
		std::string displayStr;
		std::string name;
		std::string value;
		bool open = false;
		std::vector<ListItem> children;
		std::vector<ListCommand> commands; // special case used for displaying commands

		ListItem() {}
		ListItem(std::string _name, std::string _value) : name(_name), value(_value) { UpdateDisplayStr(); }
		ListItem(std::string _name, lcf::DBString _value) : name(_name), value(std::string(_value)) { UpdateDisplayStr(); }

		template<typename T>
		ListItem(std::string _name, T _value) : name(_name) { value = fmt::format("{}", _value); UpdateDisplayStr(); }

		void UpdateDisplayStr()
		{
			if(!value.empty())
				displayStr = fmt::format("{} : [{}]", name, value);
			else
				displayStr = fmt::format("{}", name);

		}
		//template<typename T, typename N>
		//ListItem(N _name, T _value) {  name = fmt::format("{}", std::string(_name)); value = fmt::format("{}", _value); }
	};

	void List(ListItem& listItem, const lcf::rpg::CommonEvent& lcfData);

	void DisplayList(std::vector<ListItem>& list, int level = 0);
	void DisplayList(std::vector<ListCommand>& list);


}