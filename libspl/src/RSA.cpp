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
#include <spl/Int32.h>
#include <spl/io/log/Log.h>
#include <spl/math/Math.h>
#include <spl/math/Random.h>
#include <spl/math/RSA.h>

using namespace spl;

//static int32 FindPrime(int32 ignore_prime)
//{
//	int32 odd,tmp_odd;
//	
//	Random rand;
//	
//	while(1)
//	{
//		odd = rand.Next();
//		if(odd %2 == 0)
//		{
//			odd++;
//		}
//
//		if(ignore_prime == odd) 
//		{
//			continue;
//		}
//
//		int i = 2;
//
//		//test whether 'odd' is prime number or not 
//		for(; i <= odd / 2; i++)
//		{
//			tmp_odd = odd % i;
//			if(tmp_odd == 0) 
//			{
//				i = -1;
//				break;
//			}
//		}//x of 'for' loop 
//		if(i != -1) 
//		{
//			break;
//		}
//	}//x of 'while' loop 
//	return odd;
//}

RSAKey::~RSAKey()
{
}

#if defined(DEBUG) || defined(_DEBUG)
void RSAKey::CheckMem() const
{
	m_modulus.CheckMem();
	m_exponent.CheckMem();
}

void RSAKey::ValidateMem() const
{
	m_modulus.ValidateMem();
	m_exponent.ValidateMem();
}
#endif

RSAPrivateKey::RSAPrivateKey
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
 )
 : m_bitSize(bitSize), m_modulus(modulus), m_privateExponent(privateExponent), m_publicExponent(publicExponent), m_p(p), m_q(q), m_dP(dP), m_dQ(dQ), m_qInv(qInv)
{
}

RSAPrivateKey::RSAPrivateKey(int strength, int certainty)
:	m_bitSize(strength), 
	m_modulus(), 
	m_privateExponent(), 
	m_publicExponent(), 
	m_p(), 
	m_q(), 
	m_dP(), 
	m_dQ(), 
	m_qInv()
{
	BigInteger pSub1, qSub1, phi;
	Random rand;

	int pbitlength = (strength + 1) / 2;
	int qbitlength = (strength - pbitlength);
	int mindiffbits = strength / 3;

	m_publicExponent = BigInteger(24, certainty, rand);
	//m_publicExponent = *BigInteger::ValueOf(0x10001);

	//
	// Generate p, prime and (p-1) relatively prime to e
	//
	for (;;)
	{
		m_p = BigInteger(pbitlength, certainty, rand);
		ASSERT(m_p.GetBitLength() == pbitlength);

		if (m_p.Mod(m_publicExponent)->Equals(1))
		{
			continue;
		}
		if (!m_p.IsProbablePrime(certainty))
		{
			continue;
		}
		if (m_publicExponent.Gcd(m_p.Subtract(BigInteger::One()))->Equals(1)) 
		{
			break;
		}
	}

	//
	// Generate a modulus of the required length
	//
	for (;;)
	{
		// Generate q, prime and (q-1) relatively prime to e,
		// and not equal to p
		//
		for (;;)
		{
			m_q = BigInteger(qbitlength, certainty, rand);
			ASSERT(m_q.GetBitLength() == qbitlength);

			if (m_q.Subtract(m_p)->Abs()->GetBitLength() < mindiffbits)
			{
				continue;
			}
			if (m_q.Mod(m_publicExponent)->Equals(BigInteger::One()))
			{
				continue;
			}
			if (! m_q.IsProbablePrime(certainty))
			{
				continue;
			}
			if (m_publicExponent.Gcd(m_q.Subtract(BigInteger::One()))->Equals(1)) 
			{
				break;
			}
		}

		//
		// calculate the modulus
		//
		m_modulus = *m_p.Multiply(m_q);

		if (m_modulus.GetBitLength() >= strength)
		{
			break;
		}

		//
		// if we Get here our primes aren't big enough, make the largest
		// of the two p and try again
		//
		m_p = m_p > m_q ? m_p : m_q;
	}

	if (m_p.Compare(m_q) < 0)
	{
		phi = m_p;
		m_p = m_q;
		m_q = phi;
	}
	ASSERT(m_p > m_q);

	pSub1 = m_p - 1;
	qSub1 = m_q - 1;
	phi = pSub1 * qSub1;

	//
	// calculate the private exponent
	//
	m_privateExponent = *m_publicExponent.ModInverse(phi);

	m_dP = *m_privateExponent.Remainder(pSub1);
	m_dQ = *m_privateExponent.Remainder(qSub1);
	m_qInv = *m_q.ModInverse(m_p);
}

RSAPrivateKey::~RSAPrivateKey()
{
}

#if defined(DEBUG) || defined(_DEBUG)
void RSAPrivateKey::CheckMem() const
{
	m_privateExponent.CheckMem();
	m_publicExponent.CheckMem();
	m_modulus.CheckMem();
	m_p.CheckMem();
	m_q.CheckMem();
	m_dP.CheckMem();
	m_dQ.CheckMem();
	m_qInv.CheckMem();
}

void RSAPrivateKey::ValidateMem() const
{
	m_privateExponent.ValidateMem();
	m_publicExponent.ValidateMem();
	m_modulus.ValidateMem();
	m_p.ValidateMem();
	m_q.ValidateMem();
	m_dP.ValidateMem();
	m_dQ.ValidateMem();
	m_qInv.ValidateMem();
}
#endif

RSAPublic::RSAPublic(int bitSize, BigInteger& mondulus, BigInteger& exponent)
: m_bitSize(bitSize), m_publicKey(bitSize, mondulus, exponent)
{
}

RSAPublic::RSAPublic(int bitSize)
: m_bitSize(bitSize), m_publicKey()
{
}

RSAPublic::RSAPublic(const RSAPublic& key)
: m_publicKey(key.m_publicKey), m_bitSize(key.m_bitSize)
{
}

RSAPublic::RSAPublic(const RSAKey& key, int bitSize)
: m_publicKey(key), m_bitSize(bitSize)
{
}

RSAPublic::~RSAPublic()
{
}

RefCountPtr<Array<byte> > RSAPublic::EncryptBinary(Array<byte>& plainText)
{
	Vector<byte> encBytes;
	RefCountPtr<BigInteger> enc;
	Array<byte> bytes;
	int byteCount = m_bitSize / 8 + (( m_bitSize % 8 ) == 0 ? 0 : 1 );
	int inputByteCount = GetInputBlockSize(true);

	for(int ptPos = 0; ptPos < plainText.Length(); ptPos += inputByteCount)
	{
		BigInteger txt(1, plainText, ptPos, inputByteCount);

		enc = txt.ModPow(m_publicKey.Exponent(), m_publicKey.Modulus());

		enc->ToByteArrayUnsigned(bytes);
		//if (bytes.Length() != byteCount)
		//{
		//	Log::WriteInfo("bytes.Length() != byteCount %d", bytes.Length());
		//}

		encBytes.AddRangeWithLeadingPad(bytes, byteCount);

		ASSERT((encBytes.Count() % byteCount) == 0);
	}

	return encBytes.ToArray();
}

RefCountPtr<String> RSAPublic::EncryptText(Array<byte>& plainText)
{
	RefCountPtr<Array<byte> > encBytes = EncryptBinary(plainText);
	return String::Base64Encode(encBytes);
}

RefCountPtr<String> RSAPublic::EncryptText(const String& plainText)
{
	Array<byte> bytes = plainText.ToByteArray();
	return EncryptText(bytes);
}

/**
* Return the maximum size for an input block to this engine.
* For RSA this is always one byte less than the key size on
* encryption, and the same length as the key size on decryption.
*
* @return maximum size for an input block.
*/
int RSAPublic::GetInputBlockSize(bool forEncryption) const
{
	if (forEncryption)
	{
		return (m_bitSize - 1) / 8;
	}

	return (m_bitSize + 7) / 8;
}

void RSAPublic::PadInputBuffer(Array<byte>& plainText) const
{
	int blockSize = GetInputBlockSize(true);
	int mod = blockSize - (plainText.Length() % blockSize);
	if (0 == mod)
	{
		return;
	}
	Array<byte> buf(plainText.Length() + mod);
	plainText.CopyToBinary(buf);
	plainText = buf;
}

/**
* Return the maximum size for an output block to this engine.
* For RSA this is always one byte less than the key size on
* decryption, and the same length as the key size on encryption.
*
* @return maximum size for an output block.
*/
int RSAPublic::GetOutputBlockSize(bool forEncryption) const
{
	if (forEncryption)
	{
		return (m_bitSize + 7) / 8;
	}

	return (m_bitSize - 1) / 8;
}

//void RSAPublic::ConvertInput
//(
//	Array<byte>& inBuf,
//	int		inOff,
//	int		inLen,
//	BigInteger& output
//)
//{
//	int maxLength = (m_bitSize + 7) / 8;
//
//	if (inLen > maxLength)
//	{
//		throw new InvalidArgumentException("input too large for RSA cipher.");
//	}
//
//	output = BigInteger(1, inBuf, inOff, inLen);
//
//	if (output.Compare(m_publicKey.Modulus()) >= 0)
//	{
//		throw new InvalidArgumentException("input too large for RSA cipher.");
//	}
//}

//void RSAPublic::ConvertOutput
//(
//	bool forEncryption,
//	BigInteger& result,
//	Array<byte>& output
//)
//{
//	output = result.ToByteArrayUnsigned();
//
//	if (forEncryption)
//	{
//		int outSize = GetOutputBlockSize(forEncryption);
//
//		// TODO To avoid this, create version of BigInteger.ToByteArray that
//		// writes to an existing array
//		if (output.Length() < outSize) // have ended up with less bytes than normal, lengthen
//		{
//			Array<byte> tmp(outSize);
//			output.CopyTo(tmp, tmp.Length() - output.Length());
//			output = tmp;
//		}
//	}
//}

#if defined(DEBUG) || defined(_DEBUG)
void RSAPublic::CheckMem() const
{
	m_publicKey.CheckMem();
}

void RSAPublic::ValidateMem() const
{
	m_publicKey.ValidateMem();
}
#endif

RSA::RSA(int bitSize, int certainty)
: RSAPublic(bitSize), m_key(bitSize, certainty)
{
	m_publicKey = m_key.GetPublicKey();
}

RefCountPtr<Array<byte> > RSA::DecryptBinary(Array<byte>& encText)
{
	Vector<byte> ptBytes;
	BigInteger enc;
	Array<byte> bytes;
	int byteCount = m_bitSize / 8 + (( m_bitSize % 8 ) == 0 ? 0 : 1 );
	int outputByteCount = GetOutputBlockSize(false);

	ASSERT(m_publicKey.Exponent() != m_key.PrivateExponent());
	ASSERT(m_publicKey.Modulus() == m_key.Modulus());
	ASSERT((encText.Length() % byteCount) == 0);

	for(int etPos = 0; etPos < encText.Length(); etPos += byteCount)
	{
		enc = BigInteger(encText, etPos, byteCount);

		//BigInteger mP, mQ, h;

		//// mP = ((input Mod p) ^ dP)) Mod p
		//mP = (enc.Remainder(m_key.P())).ModPow(m_key.DP(), m_key.P());

		//// mQ = ((input Mod q) ^ dQ)) Mod q
		//mQ = (enc.Remainder(m_key.Q())).ModPow(m_key.DQ(), m_key.Q());

		//// h = qInv * (mP - mQ) Mod p
		//h = mP.Subtract(mQ);
		//h = h.Multiply(m_key.QInv());
		//h = h.Mod(m_key.P());               // Mod (in Java) returns the positive residual

		//// m = h * q + mQ
		//txt = h.Multiply(m_key.Q());
		//txt = txt.Add(mQ);

		BigIntegerPtr txt = enc.ModPow(m_key.PrivateExponent(), m_key.Modulus());

		txt->ToByteArrayUnsigned(bytes);

		if (bytes.Length() > byteCount)
		{
			ASSERT(bytes.Length() == byteCount);
		}

		ptBytes.AddRangeWithPad(bytes, outputByteCount);
	}

	return ptBytes.ToArray();
}

RefCountPtr<String> RSA::DecryptText(const String& encText)
{
	RefCountPtr<Array<byte> > ptBytes = DecryptBinary(*String::Base64Decode(encText));
	return RefCountPtr<String>(new String((const char *)ptBytes->Data(), ptBytes->Length()));
}

#if defined(DEBUG)
void RSA::CheckMem() const
{
	RSAPublic::CheckMem();
	m_key.CheckMem();
}

void RSA::ValidateMem() const
{
	RSAPublic::ValidateMem();
	m_key.ValidateMem();
}
#endif

