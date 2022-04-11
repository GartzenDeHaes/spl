#ifndef _simpleblockcypher_h
#define _simpleblockcypher_h

#include <spl/Debug.h>
#include <spl/Memory.h>
#include <spl/String.h>

namespace spl
{
	class SimpleBlockCypher
	{
	private:
		static int m_cypherBlocksFrom[];
		static int m_cypherBlocksTo[];

		inline SimpleBlockCypher() {}
		inline ~SimpleBlockCypher() {}
		inline void operator =(SimpleBlockCypher& sbc) {}
	
		static int BitPos(int val, int pos, int dir);
		static int CountBits(int val, int bitlen);
		static int SwapBits(int val, int blockf, int blockt, int dir);
		static int RotateBits(int val, int hash, int dir);
		static char NibbleToHex(int nibble);
		static void Bin2Text16(int val, char buf[5]);
		static int Text2Bin16(const char* str);
		static void Bin2Text8(int val, char buf[3]);
		static int Text2Bin8(const char* str);

	public:
		static StringPtr EncryptString(String plainText, String password);
		static StringPtr DecryptString(String encText, String password);
		static int Encrypt16Bits(int data, int pwhash);
		static int Decrypt16Bits(int data, int pwhash);
	};
};

#endif
