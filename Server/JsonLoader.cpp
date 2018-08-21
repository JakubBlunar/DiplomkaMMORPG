#include "JsonLoader.h"
#include <fstream>
#include <iostream>

s::JsonLoader::JsonLoader()
{
}


s::JsonLoader::~JsonLoader()
{
}

json s::JsonLoader::loadJson(std::string filename)
{
	auto found = loadedFiles.find(filename);
	if (found == loadedFiles.end())
	{
		std::ifstream jsonFile("Data/" + filename + ".json");
		if (jsonFile.is_open())
		{
			json json;
			jsonFile >> json;
		
			std::cout << json << std::endl;

			loadedFiles.insert(std::make_pair(filename, json));
			return json;
		}
		throw filename + " not exists";		
	}

	return found->second;
}
