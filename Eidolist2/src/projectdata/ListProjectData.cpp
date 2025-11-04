#include "ListProjectData.h"
#include "ImguiStyle.h"


list_data::ListCommand::ListCommand(const lcf::rpg::EventCommand& _data)
{
	data = _data;
	commandName = data.kCodeTags[data.code];
	string = data.string;

	std::string params;
	for (auto& param : data.parameters)
	{
		params += fmt::format("{}, ", param);
	}
	// trim the trailing comma and space
	if (!params.empty())
		params = params.substr(0, params.length() - 2);

	switch ((lcf::rpg::EventCommand::Code)data.code)
	{
	case lcf::rpg::EventCommand::Code::Wait:
		commandDisplayStr = fmt::format("{:<{}} {} : {}", "", data.indent * 5, commandName, params[0]);
		break;
		//case lcf::rpg::EventCommand::Code::HaltAllMovement:
		//commandDisplayStr = fmt::format("[{}]{:<{}} {}", data.code, "", data.indent, commandName);
		//break;
	case lcf::rpg::EventCommand::Code::Comment:
	case lcf::rpg::EventCommand::Code::Comment_2:
		commandDisplayStr = fmt::format("{:<{}} {} : {}", "", data.indent * 5, commandName, string);
		break;
	default:
		commandDisplayStr = fmt::format("{:<{}} {} : [{}] [\"{}\"]", "", data.indent * 5, commandName, params, string);
		break;
	}

}

void list_data::List(ListItem& listItem, const lcf::rpg::CommonEvent& lcfData)
{
	listItem.name = fmt::format("{}: {}", lcfData.ID, std::string(lcfData.name));
	listItem.UpdateDisplayStr();
	listItem.children.push_back(ListItem("id", lcfData.ID));
	listItem.children.push_back(ListItem("name", lcfData.name));
	listItem.children.push_back(ListItem("trigger", lcf::rpg::CommonEvent::kTriggerTags[lcfData.trigger]));
	listItem.children.push_back(ListItem("switch id", lcfData.switch_id));
	listItem.children.push_back(ListItem("commands", ""));
	for (auto command : lcfData.event_commands)
	{
		auto& commands_item = listItem.children.back();
		commands_item.commands.push_back(ListCommand(command));
	}
}

void list_data::DisplayList(std::vector<ListCommand>& list)
{
	color::PushStyleColor(ImGuiCol_Text, EStyleColor::PastelGreen);
	for (auto item : list)
	{
		//ImGui::TextUnformatted(item.commandDisplayStr.c_str());
	}
	ImGui::PopStyleColor();
}

void list_data::DisplayList(std::vector<ListItem>& list, int level)
{
	int id = 0;
	for (auto& item : list)
	{
		if (level >= 1)
			color::PushStyleColor(ImGuiCol_Text, EStyleColor::Lilac);

		if (level == 0)
			color::PushStyleColor(ImGuiCol_Text, EStyleColor::PaleYellow);

		ImGuiTreeNodeFlags flags = 0;
		if (item.children.empty() && item.commands.empty())
			flags |= ImGuiTreeNodeFlags_Leaf;

		item.open = ImGui::TreeNodeEx((void*)(intptr_t)id, flags, item.name.c_str());

		if (!item.value.empty())
		{
			ImGui::SameLine();
			ImGui::TextUnformatted((" : " + item.value).c_str());
		}

		if (level == 0 || level >= 1)
			ImGui::PopStyleColor();

		if (item.open)
			DisplayList(item.children, level + 1);

		DisplayList(item.commands);

		if (item.open)
			ImGui::TreePop();

		id++;
	}
}