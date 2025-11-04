#pragma once
#include "CommonIncludes.h"

namespace fileio
{
	fs::path GetPath(std::string name, bool createFolder)
	{
		fs::path fsPath = fs::absolute(fs::path(name));
		if (createFolder)
		{
			fs::path fsFolder = (fsPath.has_stem()) ? fsPath.parent_path() : fsPath;
			fs::create_directories(fsFolder);
		}
		return fsPath;
	}

	bool Write(const std::string& name, const std::string& output)
	{
		std::ofstream csvFile;
		csvFile.open(GetPath(name, true));

		if (csvFile.is_open())
		{
			csvFile << output;
			csvFile.close();
			return true;
		}
		return false;
	}

	bool Read(const std::string& name, std::string& input)
	{
		std::ifstream csvFile;
		csvFile.open(GetPath(name, false));

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

	bool FileExists(const std::string& name)
	{
		std::ifstream csvFile;
		csvFile.open(GetPath(name, false));
		return csvFile.is_open();
	}




}