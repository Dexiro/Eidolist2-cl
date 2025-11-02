#pragma once
#include "../StdIncludes.h"
#include "Cache.h"

#define E2FILETYPE std::string(".e2data")
static const int _colSize = 3;

enum DiffResult
{
	Diff_BASE,
	Diff_Update,
	Diff_Add,
	Diff_Delete,
	Diff_Same,
	Diff_NULL
};

struct FileHash
{
	fs::path path;
	std::string relative_path;
	std::string hash;
	DiffResult diff = Diff_NULL;
};

using FileHashCollection = std::array<FileHash, _colSize>;
using FileHashMap = std::map<std::string, FileHash>;
using FileHashList = std::vector<FileHash>;
using FileHashMapCombined = std::map<std::string, FileHashCollection>;

using FHPathCollectionPair= std::pair<std::string, FileHashCollection>;
using FileHashListCombined = std::vector<FHPathCollectionPair>;

struct DirectoryHashes
{
	std::string display_name;
	std::string root_path;
	FileHashMap map;
	double dtime = 0.0;
	std::string timestamp;
};

namespace FileHasher
{
	bool WriteCache(const DirectoryHashes& dirHashes);
	bool ReadCache(DirectoryHashes& dirHashes);
	void DeleteCache(const DirectoryHashes& dirHashes);
	bool CalculateHashes(DirectoryHashes& dirHashes, bool recursive = false, bool cache = true);
	void CombineFileHashMaps(const std::vector<FileHashMap>& hashMaps, FileHashMapCombined& combined, bool compare = true);
	void CombineFileHashMaps(const std::vector<FileHashMap>& hashMaps, FileHashListCombined& combined, bool compare = true);
	void ConvertMapToList(const FileHashMap& hashMap, FileHashList& hashList);
	void ConvertMapToList(const FileHashMapCombined& hashMap, FileHashListCombined& hashList);
};
