#ifndef TILESET_HOLDER_H
#define TILESET_HOLDER_H

#include <string>
#include <map>

struct TileSet {
	int columns;
	std::string path;
	int count;
	bool valid;
};

class TileSetsHolder
{
public:
	TileSet getTileSet(std::string filename);
	std::map<std::string, TileSet> loadedTileSets;

	TileSetsHolder(const TileSetsHolder&) = delete;
	TileSetsHolder& operator=(const TileSetsHolder) = delete;

	static TileSetsHolder* instance() {
		static TileSetsHolder instance;
		return &instance;
	}

private:
	TileSetsHolder();
	~TileSetsHolder();
};

#endif
