#include "main.h"

namespace Crypt
{
	std::string encrypt(const std::string& str_in, const std::string& key, const std::string& iv)
	{
		std::string str_out;
		CryptoPP::CFB_Mode<CryptoPP::AES>::Encryption encryption((byte*)key.c_str(), key.length(), (byte*)iv.c_str());
		CryptoPP::StringSource encryptor(str_in, true, new CryptoPP::StreamTransformationFilter(encryption, new CryptoPP::Base64Encoder(new CryptoPP::StringSink(str_out), false)));
		return str_out;
	}

	std::string decrypt(const std::string& str_in, const std::string& key, const std::string& iv)
	{
		std::string str_out;
		CryptoPP::CFB_Mode<CryptoPP::AES>::Decryption decryption((byte*)key.c_str(), key.length(), (byte*)iv.c_str());
		CryptoPP::StringSource decryptor(str_in, true, new CryptoPP::Base64Decoder(new CryptoPP::StreamTransformationFilter(decryption, new CryptoPP::StringSink(str_out))));
		return str_out;
	}
}