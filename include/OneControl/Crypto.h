#pragma once

#include <string>
#include <iostream>
#include <sstream>

#include <cryptopp/base64.h>
#include <cryptopp/modes.h>
#include <cryptopp/aes.h>
#include <cryptopp/rsa.h>
#include <cryptopp/osrng.h>

#include <OneLibrary/Input.h>

// This is used by both the client and the server to encrypt and decrypt data after the OneControl encryption handshake was successful.
namespace oc::Crypto
{
	inline CryptoPP::AutoSeededRandomPool rng;

	// These are populated after the OneControl cryptography handshake is complete.

	inline CryptoPP::AES::Encryption aesEncryption;
	inline CryptoPP::CBC_Mode_ExternalCipher::Encryption cbcEncryption;

	inline CryptoPP::AES::Decryption aesDecryption;
	inline CryptoPP::CBC_Mode_ExternalCipher::Decryption cbcDecryption;

	// This class lets you encrypt and decrypt an object of choice, as long as it has the std::istringstream and std::ostringstream operators overloaded.
	// TODO: Figure out if there is a better way of doing this.
	// Should this be split into Encryptor and Decryptor?
	// TODO: Don't forget to make this use a new IV every encryption cycle.
	// TODO: This can potentially do with a mutex.
	template <typename T>
	class EncryptorDecryptor
	{
	public:
		std::string Encrypt(const T& kPlain) const noexcept
		{
			std::ostringstream stream{};
			stream << kPlain;
			std::string encrypted;
			CryptoPP::StringSource encrypting(stream.str(), true, new CryptoPP::StreamTransformationFilter(oc::Crypto::cbcEncryption, new CryptoPP::StringSink(encrypted)));

			return encrypted;
		}

		T Decrypt(const std::string& kEncrypted) const noexcept
		{
			std::string plain;
			CryptoPP::StringSource decrypting(kEncrypted, true, new CryptoPP::StreamTransformationFilter(oc::Crypto::cbcDecryption, new CryptoPP::StringSink(plain)));

			T decrypted{};
			std::istringstream stream(plain);
			stream >> decrypted;
			return decrypted;
		}
	};
}