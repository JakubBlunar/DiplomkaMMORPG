#ifndef JSON_LOADER_H
#define JSON_LOADER_H

#include "json.hpp"
using json = nlohmann::json;

class JsonLoader {
public:
	json loadJson(std::string filename);
	std::map<std::string, json> loadedFiles;

	JsonLoader(const JsonLoader&) = delete;
	JsonLoader& operator=(const JsonLoader) = delete;

	static JsonLoader* instance() {
		static JsonLoader instance;
		return &instance;
	}

private:
	JsonLoader();
	~JsonLoader();
};

#endif
