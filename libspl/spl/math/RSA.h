/*
 *   This file is part of the Standard Portable Library (SPL).
 *
 *   SPL is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   SPL is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with SPL.  If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef _rsa_h
#define _rsa_h

#include <spl/types.h>
#include <spl/Debug.h>
#include <spl/collection/Array.h>
#include <spl/BigInteger.h>
#include <spl/RefCountPtr.h>
#include <spl/String.h>

namespace spl
{
	/** 
	 * @defgroup math Math
	 * @{
	 */

	/** @brief RSA public key
	 */
	class RSAKey : public IMemoryValidate
	{
	protected:
		BigInteger m_modulus;
		BigInteger m_exponent;
		int m_bitSize;

	public:
		inline RSAKey()
		: m_bitSize(0), m_modulus(), m_exponent()
		{
		}

		virtual ~RSAKey();

		inline RSAKey(int bitSize, const BigInteger& mondulus, const BigInteger& exponent)
		: m_bitSize(bitSize), m_modulus(mondulus), m_exponent(exponent)
		{
		}

		inline RSAKey(const RSAKey& k)
		: m_bitSize(k.m_bitSize), m_modulus(k.m_modulus), m_exponent(k.m_exponent)
		{
		}

		inline RSAKey& operator =(const RSAKey& k)
		{
			m_bitSize = k.m_bitSize;
			m_modulus = k.m_modulus;
			m_exponent = k.m_exponent;

			return *this;
		}

		inline BigInteger& Modulus() { return m_modulus; }
		inline BigInteger& Exponent() { return m_exponent; }
		inline int GetBitSize() const { return m_bitSize; }

	#if defined(DEBUG) || defined(_DEBUG)
		virtual void CheckMem() const;
		virtual void ValidateMem() const;
	#endif
	};

	/** @brief RSA private key
	 */
	class RSAPrivateKey
	{
	private:
		BigInteger m_p;
		BigInteger m_q;
		BigInteger m_dP;
		BigInteger m_dQ;
		BigInteger m_qInv;

		BigInteger m_modulus;
		BigInteger m_privateExponent;
		BigInteger m_publicExponent;

		int m_bitSize;

	public:
		RSAPrivateKey(int strength, int certainty);
		RSAPrivateKey
		(
			int bitSize,
			BigInteger	modulus,
			BigInteger	publicExponent,
			BigInteger	privateExponent,
			BigInteger	p,
			BigInteger	q,
			BigInteger	dP,
			BigInteger	dQ,
			BigInteger	qInv
		);

		virtual ~RSAPrivateKey();

		inline RSAKey GetPublicKey() const { return RSAKey(m_bitSize, m_modulus, m_publicExponent); }

		BigInteger& PublicExponent()
		{
			return m_publicExponent;
		}

		BigInteger& PrivateExponent()
		{
			return m_privateExponent;
		}

		BigInteger& Modulus()
		{
			return m_modulus;
		}

		BigInteger& P()
		{
			return m_p;
		}

		BigInteger& Q()
		{
			return m_q;
		}

		BigInteger& DP()
		{
			return m_dP;
		}

		BigInteger& DQ()
		{
			return m_dQ;
		}

		BigInteger& QInv()
		{
			return m_qInv;
		}

	#if defined(DEBUG) || defined(_DEBUG)
		virtual void CheckMem() const;
		virtual void ValidateMem() const;
	#endif
	};

	/** @brief RSA public cypher
	 */
	class RSAPublic : public IMemoryValidate
	{
	protected:
		RSAKey m_publicKey;
		int m_bitSize;

		//void ConvertInput
		//(
		//	Array<byte>& inBuf,
		//	int		inOff,
		//	int		inLen,
		//	BigInteger& output
		//);

		//void ConvertOutput
		//(
		//	bool forEncryption,
		//	BigInteger& result,
		//	Array<byte>& output
		//);

		/**
		* Return the maximum size for an input block to this engine.
		* For RSA this is always one byte less than the key size on
		* encryption, and the same length as the key size on decryption.
		*
		* @return maximum size for an input block.
		*/
		int GetInputBlockSize(bool forEncryption) const;

		/**
		* Return the maximum size for an output block to this engine.
		* For RSA this is always one byte less than the key size on
		* decryption, and the same length as the key size on encryption.
		*
		* @return maximum size for an output block.
		*/
		int GetOutputBlockSize(bool forEncryption) const;

	public:
		RSAPublic(int bitSize, BigInteger& mondulus, BigInteger& exponent);
		RSAPublic(int bitSize);
		RSAPublic(const RSAPublic& key);
		RSAPublic(const RSAKey& key, int bitSize);
		virtual ~RSAPublic();

		inline RSAKey& GetPublicKey() { return m_publicKey; }
		inline int GetBitSize() const { return m_bitSize; }

		RefCountPtr<Array<byte> > EncryptBinary(Array<byte>& plainText);
		RefCountPtr<String> EncryptText(Array<byte>& plainText);
		RefCountPtr<String> EncryptText(const String& plainText);

		void PadInputBuffer(Array<byte>& plainText) const;

	#if defined(DEBUG) || defined(_DEBUG)
		virtual void CheckMem() const;
		virtual void ValidateMem() const;
	#endif
	};

	/** @brief RSA public and private cypher.
	 */
	class RSA : public RSAPublic
	{
	private:
		RSAPrivateKey m_key;

	public:
		RSA(int bitSize, int certainty = 4);

		inline RSAPublic& GetPublic() const { return (RSAPublic&)*this; }
		inline RSAPrivateKey GetPrivateKey() { return m_key; }

		RefCountPtr<Array<byte> > DecryptBinary(Array<byte>& encText);
		StringPtr DecryptText(const String& encText);

	#if defined(DEBUG) || defined(_DEBUG)
		virtual void CheckMem() const;
		virtual void ValidateMem() const;
	#endif
	};

	/** @} */
}
#endif
