// chachapoly.h - written and placed in the public domain by Jeffrey Walton
//                RFC 8439, Section 2.8, AEAD Construction, http://tools.ietf.org/html/rfc8439

/// \file chachapoly.h
/// \brief ChaCha20/Poly1305-TLS AEAD cipher
/// \details ChaCha20Poly1305 is an authenticated encryption cipher that combines
///  ChaCha20TLS and Poly1305TLS. The cipher uses the IETF versions of ChaCha and
///  Poly1305 because it is defined in RFC 8439, section 2.8, AEAD_CHACHA20_POLY1305
///  construction.
/// \sa <A HREF="http://tools.ietf.org/html/rfc8439">RFC 8439, ChaCha20 and Poly1305
///  for IETF Protocols</A>.

#ifndef CRYPTOPP_CHACHA_POLY1305_H
#define CRYPTOPP_CHACHA_POLY1305_H

#include "cryptlib.h"
#include "authenc.h"
#include "chacha.h"
#include "poly1305.h"

NAMESPACE_BEGIN(CryptoPP)

/// \brief ChaCha20Poly1305 cipher base implementation
/// \details Base implementation of the AuthenticatedSymmetricCipher interface
/// \since Crypto++ 8.1
class ChaCha20Poly1305_Base
	: public FixedKeyLength<32, SimpleKeyingInterface::UNIQUE_IV, 12>, public AuthenticatedSymmetricCipherBase
{
public:
	virtual ~ChaCha20Poly1305_Base() {}

	virtual const MessageAuthenticationCode & GetMAC() const = 0;
	virtual MessageAuthenticationCode & AccessMAC() = 0;

public:
	// AuthenticatedSymmetricCipher
	std::string AlgorithmName() const
		{return std::string("ChaCha20/Poly1305");}
	std::string AlgorithmProvider() const
		{return "C++";}
	size_t MinKeyLength() const
		{return 32;}
	size_t MaxKeyLength() const
		{return 32;}
	size_t DefaultKeyLength() const
		{return 32;}
	size_t GetValidKeyLength(size_t n) const
		{CRYPTOPP_UNUSED(n); return 32;}
	bool IsValidKeyLength(size_t n) const
		{return n==32;}
	unsigned int OptimalDataAlignment() const
		{return GetSymmetricCipher().OptimalDataAlignment();}
	IV_Requirement IVRequirement() const
		{return UNIQUE_IV;}
	unsigned int IVSize() const
		{return 12;}
	unsigned int MinIVLength() const
		{return 12;}
	unsigned int MaxIVLength() const
		{return 12;}
	unsigned int DigestSize() const
		{return 16;}
	lword MaxHeaderLength() const
		{return LWORD_MAX;}
	lword MaxMessageLength() const
		{return LWORD_MAX;}
	lword MaxFooterLength() const
		{return 16;}

	/// \brief Encrypts and calculates a MAC in one call
	/// \param ciphertext the encryption buffer
	/// \param mac the mac buffer
	/// \param macSize the size of the MAC buffer, in bytes
	/// \param iv the iv buffer
	/// \param ivLength the size of the IV buffer, in bytes
	/// \param aad the AAD buffer
	/// \param aadLength the size of the AAD buffer, in bytes
	/// \param message the message buffer
	/// \param messageLength the size of the messagetext buffer, in bytes
	/// \details EncryptAndAuthenticate() encrypts and generates the MAC in one call. The function
	///   truncates the MAC if <tt>macSize < TagSize()</tt>.
	virtual void EncryptAndAuthenticate(byte *ciphertext, byte *mac, size_t macSize, const byte *iv, int ivLength, const byte *aad, size_t aadLength, const byte *message, size_t messageLength);

	/// \brief Decrypts and verifies a MAC in one call
	/// \param message the decryption buffer
	/// \param mac the mac buffer
	/// \param macSize the size of the MAC buffer, in bytes
	/// \param iv the iv buffer
	/// \param ivLength the size of the IV buffer, in bytes
	/// \param aad the AAD buffer
	/// \param aadLength the size of the AAD buffer, in bytes
	/// \param ciphertext the cipher buffer
	/// \param ciphertextLength the size of the ciphertext buffer, in bytes
	/// \return true if the MAC is valid and the decoding succeeded, false otherwise
	/// \details DecryptAndVerify() decrypts and verifies the MAC in one call.
	/// <tt>message</tt> is a decryption buffer and should be at least as large as the ciphertext buffer.
	/// \details The function returns true iff MAC is valid. DecryptAndVerify() assumes the MAC
	///  is truncated if <tt>macLength < TagSize()</tt>.
	virtual bool DecryptAndVerify(byte *message, const byte *mac, size_t macSize, const byte *iv, int ivLength, const byte *aad, size_t aadLength, const byte *ciphertext, size_t ciphertextLength);

protected:
	// AuthenticatedSymmetricCipherBase
	bool AuthenticationIsOnPlaintext() const {return false;}
	unsigned int AuthenticationBlockSize() const {return 1;}
	void SetKeyWithoutResync(const byte *userKey, size_t keylength, const NameValuePairs &params);
	void Resync(const byte *iv, size_t len);
	size_t AuthenticateBlocks(const byte *data, size_t len);
	void AuthenticateLastHeaderBlock();
	void AuthenticateLastConfidentialBlock();
	void AuthenticateLastFooterBlock(byte *mac, size_t macSize);

protected:
	// ChaCha20 does not stash away the user key. There's no way to restart
	// ChaCha once an encryption or decryption is performed. In fact, we
	// cannot even Resynchronize it correctly. Compensate here.
	void RekeyCipherAndMac(const byte *userKey, size_t userKeyLength, const NameValuePairs &params);

	SecByteBlock m_userKey;
};

/// \brief ChaCha20Poly1305 cipher final implementation
/// \details ChaCha20Poly1305 is an authenticated encryption cipher that combines
///  ChaCha20TLS and Poly1305TLS. The cipher uses the IETF versions of ChaCha and
///  Poly1305 because it is defined in RFC 8439, section 2.8, AEAD_CHACHA20_POLY1305
///  construction.
/// \sa <A HREF="http://tools.ietf.org/html/rfc8439">RFC 8439, ChaCha20 and Poly1305
///  for IETF Protocols</A>.
/// \since Crypto++ 8.1
class ChaCha20Poly1305_Final : public ChaCha20Poly1305_Base
{
public:
	static std::string StaticAlgorithmName()
		{return std::string("ChaCha20/Poly1305");}

protected:
	const SymmetricCipher & GetSymmetricCipher()
		{return const_cast<ChaCha20Poly1305_Final *>(this)->AccessSymmetricCipher();}
	SymmetricCipher & AccessSymmetricCipher()
		{return m_cipher;}
	bool IsForwardTransformation() const
		{return m_cipher.IsForwardTransformation();}

	const MessageAuthenticationCode & GetMAC() const
		{return const_cast<ChaCha20Poly1305_Final *>(this)->AccessMAC();}
	MessageAuthenticationCode & AccessMAC()
		{return m_mac;}

private:
	ChaChaTLS::Encryption m_cipher;
	Poly1305TLS m_mac;
};

/// \brief ChaCha20Poly1305-TLS cipher mode of operation
/// \details ChaCha20Poly1305 is an authenticated encryption cipher that combines
///  ChaCha20TLS and Poly1305TLS. The cipher uses the IETF versions of ChaCha and
///  Poly1305 because it is defined in RFC 8439, section 2.8, AEAD_CHACHA20_POLY1305
///  construction.
/// \sa <A HREF="http://tools.ietf.org/html/rfc8439">RFC 8439, ChaCha20 and Poly1305
///  for IETF Protocols</A>.
/// \since Crypto++ 8.1
struct ChaCha20Poly1305 : public AuthenticatedSymmetricCipherDocumentation
{
	typedef ChaCha20Poly1305_Final Encryption;
	typedef Encryption Decryption;
};


NAMESPACE_END

#endif  // CRYPTOPP_CHACHA_POLY1305_H
