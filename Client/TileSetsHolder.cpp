#include "TileSetsHolder.h"
#include "JsonLoader.h"
#include "../Shared/Utils.h"


TileSetsHolder::TileSetsHolder()
{
}


TileSetsHolder::~TileSetsHolder()
{
}


TileSet TileSetsHolder::getTileSet(std::string filename) {
	auto found = loadedTileSets.find(filename);
	if (found != loadedTileSets.end()) {
		return found->second;
	}

	json tileSetData = JsonLoader::instance()->loadJson("Tilesets/" + filename);

	std::string path = tileSetData["image"].get<json::string_t>();

	TileSet tileSet;
	tileSet.columns = (int)tileSetData["columns"].get<json::number_integer_t>();
	tileSet.count = (int)tileSetData["tilecount"].get<json::number_integer_t>();

	tileSet.valid = true;

	std::vector<std::string> splitedPath = Utils::splitPath(path, { '/' });
	tileSet.path = "Graphics/Tilesets/" + splitedPath.back();
	loadedTileSets.insert(std::make_pair(filename, tileSet));
	return tileSet;
}
