#pragma once
#include "../StdIncludes.h"
#include "../../external/nlohmann/json.hpp"
using namespace nlohmann;

namespace wiki
{
	struct LocationData
	{
		int locationId = -1;
		std::vector<int> mapIds;
		std::string name;
		std::string primaryAuthor;
		std::string bgColor;
		std::string fgColor;
		std::string addedVer;
		std::set<int> connectedLocations;
		std::set<int> connectionIds;
		json js;
	};

	struct ConnectionData
	{
		int originId = 0;
		int dstId = 0;
		std::string originName;
		std::string dstName;
		std::vector<std::string> effectsNeeded;
		std::vector<std::string> attributes;
	};

	struct WikiData
	{
		std::vector<ConnectionData> connections;
		std::vector<LocationData> locations;
		std::map<int, int> mapToLocationId;
		std::map<std::string, int> nameToLocationId;

		std::map<int, std::string> mapReservationName;
	};

	const std::string LocationsURL = R"(https://wrapper.yume.wiki/locations?game=unconscious)";
	const std::string ConnectionsURL = R"(https://wrapper.yume.wiki/connections?game=unconscious)";

	void GetWikiData(WikiData& data, bool requestData = false);
	void GetLocations(WikiData& data, bool requestData = false);
	void GetConnections(WikiData& data, bool requestData = false);

	bool CheckCacheExists(WikiData& data);
}

namespace googlesheets
{
	void GetMapReservations(wiki::WikiData& data); // <- wikidata param is temporary



}







