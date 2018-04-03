#include "Crypto.h"

#include <cryptopp/hex.h>
#include <cryptopp/sha.h>
#include <cryptopp/base64.h>

Crypto *Crypto::s_instance = 0;

Crypto::Crypto()
{

}

string Crypto::hashWithSha256(string text)
{
	std::string passwordHash;
	CryptoPP::SHA256 hash;
	CryptoPP::StringSource("24051994", true,
		new CryptoPP::HashFilter(hash,
			new CryptoPP::HexEncoder(
				new CryptoPP::StringSink(passwordHash)
			)
		)
	);
	return passwordHash;
}


