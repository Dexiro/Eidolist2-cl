#include "WikiData.h"
#include <cpr/cpr.h>
#include "../io/Cache.h"

//"locations": [
//{
//    "backgroundColor": "#4681E1",
//        "bgms" : [
//    {
//        "path": "https://yume.wiki/images/1/14/Cu_noahrav_music_40a_100.ogg",
//            "title" : "noahrav_music_40a"
//    }
//        ] ,
//        "fontColor": "#D9E6FF",
//        "game" : "Collective Unconscious",
//        "locationImage" : "https://yume.wiki/images/7/77/Cu_a_long_journey.png",
//        "locationMaps" : [] ,
//        "mapIds" : [
//            515
//        ] ,
//        "originalName" : "長旅",
//        "primaryAuthor" : "NacreMoon",
//        "title" : "A Long Journey",
//        "versionAdded" : "0.1.0",
//        "versionGaps" : [] ,
//        "versionsUpdated" : []
//},

std::string GetCachePath(std::string name, bool createFolder)
{
	fs::path absolutePath = fs::absolute(fs::path(fmt::format("{}", CACHEPATH)));

	if (createFolder)
		fs::create_directories(absolutePath);

	return fmt::format("{}{}.txt", absolutePath.string(), name);
}

bool WriteCache(const std::string& name, const std::string& output)
{
	std::ofstream csvFile;
	csvFile.open(GetCachePath(name, true));

	if (csvFile.is_open())
	{
		csvFile << output;
		csvFile.close();
		return true;
	}
	return false;
}

bool ReadCache(const std::string& name, std::string& input)
{
	std::ifstream csvFile;
	csvFile.open(GetCachePath(name, false));

	if (csvFile.is_open())
	{
		std::string line;
		while (std::getline(csvFile, line))
		{
			if (line.empty())
				continue;
			input.append(line);
		}
		return true;
	}
	return false;
}

bool CacheExists(const std::string& name)
{
	std::ifstream csvFile;
	csvFile.open(GetCachePath(name, false));
	return csvFile.is_open();
}

void wiki::GetLocations(WikiData& data, bool requestData)
{
	data.locations.clear();
	data.mapToLocationId.clear();

	if (requestData)
	{
		std::string game = "unconscious";
		std::string continueKey = "0";

		std::string locationsURL = R"(https://wrapper.yume.wiki/locations)";
		cpr::Url url = { locationsURL };

		json fullJson;

		bool error = false;
		while (!error && !continueKey.empty())
		{
			cpr::Parameters parameters = { {"game", game }, {"continueKey", continueKey } };
			cpr::Response response = cpr::Get(url, parameters);
			if (!response.error)
			{
				json js = json::parse(response.text);
				continueKey = (js.count("continueKey") > 0) ? js["continueKey"] : "";

				for (auto& locationElem : js["locations"])
				{
					fullJson["locations"].push_back(locationElem);
				}
			}
			else
			{
				error = true;
			}
		}

		if (!fullJson.empty())
		{
			WriteCache("WikiLocations", fullJson.dump());
		}
	}

	if (!CacheExists("WikiLocations"))
		return;

	std::string cacheInput;
	if (ReadCache("WikiLocations", cacheInput))
	{
		json js = json::parse(cacheInput);

		for (auto& locationElem : js["locations"])
		{
			LocationData ld;
			ld.bgColor = locationElem["backgroundColor"];
			ld.fgColor = locationElem["fontColor"];
			ld.addedVer = locationElem["versionAdded"];
			if (locationElem.contains("primaryAuthor"))
				ld.primaryAuthor = locationElem["primaryAuthor"];
			ld.name = locationElem["title"];
			ld.js = locationElem;
			for (int mapId : locationElem["mapIds"])
			{
				ld.mapIds.push_back(mapId);
				data.mapToLocationId[mapId] = (int)data.locations.size(); // kinda hacky but should be ok for now...
				data.nameToLocationId[ld.name] = (int)data.locations.size();
			}
			data.locations.push_back(ld);
		}
	}
}

void wiki::GetConnections(WikiData& data, bool requestData)
{
	if (requestData)
	{
		std::string game = "unconscious";
		std::string continueKey = "0";

		std::string locationsURL = R"(https://wrapper.yume.wiki/connections)";
		cpr::Url url = { locationsURL };

		json fullJson;

		bool error = false;
		while (!error && !continueKey.empty())
		{
			cpr::Parameters parameters = { {"game", game }, {"continueKey", continueKey } };
			cpr::Response response = cpr::Get(url, parameters);
			if (!response.error)
			{
				json js = json::parse(response.text);
				continueKey = (js.count("continueKey") > 0) ? js["continueKey"] : "";

				for (auto& connectionElem : js["connections"])
				{
					fullJson["connections"].push_back(connectionElem);
				}
			}
			else
			{
				error = true;
			}
		}

		if (!fullJson.empty())
		{
			WriteCache("WikiConnections", fullJson.dump());
		}
	}

	if (!CacheExists("WikiConnections"))
		return;
		
	std::string cacheInput;
	if (ReadCache("WikiConnections", cacheInput))
	{
		json js = json::parse(cacheInput);

		for (auto& connectionElem : js["connections"])
		{
			ConnectionData cd;
			cd.originName = connectionElem["origin"];
			cd.dstName = connectionElem["destination"];

			for (auto& attribute : js["attributes"])
				cd.attributes.push_back(attribute);

			for (auto& effect : js["effectsNeeded"])
				cd.effectsNeeded.push_back(effect);

			cd.originId = data.nameToLocationId[cd.originName];
			cd.dstId = data.nameToLocationId[cd.dstName];

			LocationData& origin = data.locations[cd.originId];
			origin.connectedLocations.insert(cd.dstId);
			origin.connectionIds.insert(data.connections.size());

			LocationData& dst = data.locations[cd.dstId];
			dst.connectedLocations.insert(cd.dstId);
			dst.connectionIds.insert(data.connections.size());

			data.connections.push_back(cd);
		}
	}
}

bool wiki::CheckCacheExists(WikiData& data)
{
	if (CacheExists("WikiConnections") && CacheExists("WikiLocations"))
	{
		return true;
	}
	return false;
}

void wiki::GetWikiData(WikiData& data, bool requestData)
{
	wiki::GetLocations(data, requestData);
	wiki::GetConnections(data, requestData);
}

void googlesheets::GetMapReservations(wiki::WikiData& data)
{
	// Doesn't seem to connect (authentication issue?)
	std::string baseurl = R"(https://sheets.googleapis.com/v4/spreadsheets/1H-f9iXSCZMGhCLny914neOCTLkf89xB_SwjTWR7UfJo/values/MAPS!A2:B2500)";

	cpr::Response response = cpr::Get(baseurl);
	if (!response.error)
	{
		json js = json::parse(response.text);
		for (auto& values : js["values"])
		{
			if(values.size() >= 2)
			{
				std::string mapID = values[0];
				std::string name = values[1];
				data.mapReservationName.emplace(stoi(mapID), name);
			}
		}
	}

}
