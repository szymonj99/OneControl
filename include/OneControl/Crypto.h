#pragma once

#include <cryptopp/base64.h>
#include <cryptopp/modes.h>
#include <cryptopp/aes.h>
#include <cryptopp/rsa.h>

// This is used by both the client and the server to encrypt and decrypt data after the OneControl encryption handshake was successful.
namespace oc::Crypto
{
	// These are populated after the OneControl cryptography handshake is complete.

	inline CryptoPP::AES::Encryption aesEncryption;
	inline CryptoPP::CBC_Mode_ExternalCipher::Encryption cbcEncryption;

	inline CryptoPP::AES::Decryption aesDecryption;
	inline CryptoPP::CBC_Mode_ExternalCipher::Decryption cbcDecryption;
}