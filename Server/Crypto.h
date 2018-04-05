#ifndef CRYPTO_H
#define CRYPTO_H

#include <string>

using namespace std;

typedef unsigned char byte;

class Crypto
{
	static Crypto *s_instance;
	Crypto();
public:
	static Crypto *i()
	{
		if (!s_instance) {
			s_instance = new Crypto();
		}
		return s_instance;
	}

	string hashWithSha256(string text);
	
};

#endif // ! CRYPTO_H