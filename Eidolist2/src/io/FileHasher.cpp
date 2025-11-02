#include "FileHasher.h"
#include "../../external/hash-library/sha1.h"
#include "../../external/hash-library/md5.h"
#include <fmt/format.h>
#include <fmt/chrono.h>

bool get_contents(const std::string& path, std::vector<std::byte>& fbytes)
{
	std::ifstream source_file{ path, std::ios::binary };
	if (!source_file.good()) {
		fmt::print(stderr, "Unable to correctly open file\n");
	}
	// Get length of source file,
	// Create vector the size of the file
	// Read source file into vector
	auto length{ std::filesystem::file_size(path) };
	fbytes = std::vector<std::byte>(length); // Use parentheses so that we construct a vector of a certain size
	source_file.read(reinterpret_cast<char*>(fbytes.data()), static_cast<long>(length));

	return !fbytes.empty();
}

static std::string GetCachePath(const DirectoryHashes& dirHashes, bool createFolder)
{
	if (createFolder)
		fs::create_directories(fmt::format("{}{}\\", CACHEPATH, dirHashes.display_name));

	return fmt::format("{}{}\\checksum.txt", CACHEPATH, dirHashes.display_name);
}

bool FileHasher::WriteCache(const DirectoryHashes& dirHashes)
{
	std::ofstream csvFile;
	csvFile.open(GetCachePath(dirHashes, true));

	if (csvFile.is_open())
	{
		//csvFile << dirHashes.timestamp << std::endl << fmt::format("{}", dirHashes.dtime) << std::endl;

		for (auto& [key, val] : dirHashes.map)
			csvFile << val.relative_path << ',' << val.hash << std::endl;
		csvFile.close();

		return true;
	}
	return false;
}

bool FileHasher::ReadCache(DirectoryHashes& dirHashes)
{
	dirHashes.map.clear();

	std::ifstream csvFile;
	csvFile.open(GetCachePath(dirHashes, false));

	if (csvFile.is_open())
	{
		std::string line;
		while (std::getline(csvFile, line))
		{
			if (line.empty())
				continue;

			std::istringstream iss(line);
			std::string lineStream, path, hash;
			if (getline(iss, lineStream, ','))
				path = lineStream;
			if (getline(iss, lineStream, ','))
				hash = lineStream;
			if (!path.empty() && !hash.empty())
			{
				dirHashes.map[path].path = fs::path(fmt::format("{}\\{}", dirHashes.root_path, path));
				dirHashes.map[path].relative_path = path;
				dirHashes.map[path].hash = hash;
			}
		}
		return true;
	}
	return false;
}

void FileHasher::DeleteCache(const DirectoryHashes& dirHashes)
{
	std::remove(GetCachePath(dirHashes, false).c_str());
}

bool FileHasher::CalculateHashes(DirectoryHashes& dirHashes, bool recursive, bool cache)
{
	MD5 md5;
	auto calculate = [&](auto const& dir_entry)
	{ 
		if (dir_entry.is_regular_file())
		{
			if (dir_entry.path().extension() == E2FILETYPE)
				return;

			FileHash fileHash;
			fileHash.path = dir_entry.path();

			std::vector<std::byte> fbytes;
			get_contents(fileHash.path.string(), fbytes);
			for (int i = 0; i < fbytes.size(); i++)
			{
				const char* b = (char*)fbytes.data();
				md5.add(b + i, 1);
			}
			fileHash.hash = md5.getHash();
			md5.reset();
			fileHash.relative_path = std::filesystem::relative(fileHash.path, dirHashes.root_path).string();
			dirHashes.map[fileHash.relative_path] = fileHash;
		}
	};

	if (recursive)
	{
		for (auto const& dir_entry : fs::recursive_directory_iterator(dirHashes.root_path))
			calculate(dir_entry);
	}
	else
	{
		for (auto const& dir_entry : fs::directory_iterator(dirHashes.root_path))
			calculate(dir_entry);
	}

	bool success = !dirHashes.map.empty();

	if(success)
	{
		// Get timestamp
		auto now = std::chrono::system_clock::now();
		dirHashes.dtime = std::chrono::duration_cast<std::chrono::seconds>(now.time_since_epoch()).count();
		dirHashes.timestamp = fmt::format("{:%Y%m%d-%H%M%S}", now);

		if (cache)
			WriteCache(dirHashes);
	}

	return success;
}

// Currently supports a max of 3 hash maps
void FileHasher::CombineFileHashMaps(const std::vector<FileHashMap>& hashMaps, FileHashMapCombined& combined, bool compare)
{
	if (hashMaps.empty())
		return;

	combined.clear();
	for (int i = 0; i < std::min((int)hashMaps.size(), _colSize); i++)
	{
		for (auto& [key, val] : hashMaps[i])
		{
			combined[key][i] = val;
		}
	}

	if (compare)
	{
		for (auto& [key, val] : combined)
		{
			val[0].diff = DiffResult::Diff_BASE;
			for (int i = 1; i < _colSize; i++)
			{
				if (val[0].hash == val[i].hash)
				{
					val[i].diff = DiffResult::Diff_Same;
				}
				else if (val[0].hash.empty())
				{
					val[i].diff = DiffResult::Diff_Add;
				}
				else if (val[i].hash.empty())
				{
					val[i].diff = DiffResult::Diff_Delete;
				}
				else
				{
					val[i].diff = DiffResult::Diff_Update;
				}
			}
		}
	}
}

void FileHasher::CombineFileHashMaps(const std::vector<FileHashMap>& hashMaps, FileHashListCombined& combined, bool compare)
{
	FileHashMapCombined combinedMap;
	FileHasher::CombineFileHashMaps(hashMaps, combinedMap, compare);
	FileHasher::ConvertMapToList(combinedMap, combined);
}

void FileHasher::ConvertMapToList(const FileHashMap& hashMap, FileHashList& hashList)
{
	for (auto& [key, val] : hashMap)
		hashList.push_back(val);
}

void FileHasher::ConvertMapToList(const FileHashMapCombined& hashMap, FileHashListCombined& hashList)
{
	for (auto& [key, val] : hashMap)
		hashList.push_back(std::make_pair(key, val));
}