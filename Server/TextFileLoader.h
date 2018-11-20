#ifndef TEXT_FILE_LOADER_H
#define TEXT_FILE_LOADER_H

#include <string>
#include <map>
#include <SFML/System/Mutex.hpp>

namespace s {
	class TextFileLoader {
	public:
		std::string loadFile(std::string filename);
		std::map<std::string, std::string> loadedFiles;

		sf::Mutex mutex;

		TextFileLoader(const TextFileLoader&) = delete;
		TextFileLoader& operator=(const TextFileLoader) = delete;

		static TextFileLoader* instance() {
			static TextFileLoader instance;
			return &instance;
		}

	private:
		TextFileLoader();
		~TextFileLoader();
	};
}


#endif
