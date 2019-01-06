#include "Crypto.h"

#include <cryptopp/hex.h>
#include <cryptopp/sha.h>
#include <cryptopp/base64.h>

Crypto* Crypto::s_instance = nullptr;

Crypto::Crypto() {

}

std::string Crypto::hashWithSha256(std::string text) const {
	std::string passwordHash;
	CryptoPP::SHA256 hash;
	CryptoPP::StringSource stringSource(text, true,
	                                    new CryptoPP::HashFilter(hash,
	                                                             new CryptoPP::HexEncoder(
		                                                             new CryptoPP::StringSink(passwordHash)
	                                                             )
	                                    )
	);
	return passwordHash;
}
