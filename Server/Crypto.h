#ifndef CRYPTO_H
#define CRYPTO_H

#include <string>


class Crypto {
	static Crypto* s_instance;
	Crypto();
public:
	static Crypto* i() {
		if (!s_instance) {
			s_instance = new Crypto();
		}
		return s_instance;
	}

	std::string hashWithSha256(std::string text) const;

};

#endif // ! CRYPTO_H
