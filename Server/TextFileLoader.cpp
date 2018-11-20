#include "TextFileLoader.h"
#include <fstream>
#include <SFML/System/Lock.hpp>


s::TextFileLoader::TextFileLoader()
{
}


s::TextFileLoader::~TextFileLoader()
{
}

std::string s::TextFileLoader::loadFile(std::string filename) {
	sf::Lock lock(mutex);
	auto found = loadedFiles.find(filename);
	if (found == loadedFiles.end()) {
		std::ifstream ifs("Data/" + filename);
		if (ifs.is_open()) {
			 std::string content{std::istreambuf_iterator<char>(ifs) , std::istreambuf_iterator<char>()};

			loadedFiles.insert(std::make_pair(filename, content));
			return content;
		} 
		throw filename + " not exists";
	}

	return found->second;
}
