#include <spl/Exception.h>
#include <spl/Int32.h>
#include <spl/math/SimpleBlockCypher.h>
#include <spl/text/StringBuffer.h>

using namespace spl;

int SimpleBlockCypher::m_cypherBlocksFrom[] = {14081, 32173, 18493, 41387, 55823, 43411, 48593, 11423};
int SimpleBlockCypher::m_cypherBlocksTo[] = {57859, 49031, 49199, 20011, 51803, 4973,  57787, 40093};

StringPtr SimpleBlockCypher::EncryptString(String plainText, String password)
{
	int pwHash = (int)password.HashCode();
	int len = plainText.Length()/2 + plainText.Length() % 2;
	int* enc = new int[len];
	
	// Encrypt
	int pos = 0;
	for ( int x = 0; x < plainText.Length(); x += 2 )
	{
		char ch1 = plainText[x];
		char ch2 = '\0';
		if ( x + 1 < plainText.Length() )
		{
			ch2 = plainText[x + 1];
		}
		enc[pos++] = Encrypt16Bits((int)ch1 | (((int)ch2) << 8), pwHash);
	}
	
	// Bin to string
	StringBuffer encTextBuf;
	char buf[5];

	for ( int x = 0; x < len; x++ )
	{
		Bin2Text16(enc[x], buf);
		encTextBuf.Append(buf);
	}

	delete[] enc;

	return encTextBuf.ToString();
}

StringPtr SimpleBlockCypher::DecryptString(String encText, String password)
{
	int pwHash = (int)password.HashCode();
	int len = encText.Length()/4;
	// String to bin
	int* enc = new int[len];
	int pos = 0;
	
	for ( int x = 0; x < len; x += 4 )
	{
		enc[pos++] = Text2Bin16(&encText.GetChars()[x]);
	}
	
	// Decrypt
	StringBuffer output;

	for ( int x = 0; x < len; x++ )
	{
		int data = Decrypt16Bits(enc[x], pwHash);
		char ch1 = (char)(data & 0xFF);
		char ch2 = (char)((data >> 8) & 0xFF);
		output.Append(ch1);
		output.Append(ch2);
	}
	
	delete[] enc;

	return output.ToString();
}

int SimpleBlockCypher::Encrypt16Bits(int data, int pwhash)
{
	return RotateBits(data, pwhash, 1);
}

int SimpleBlockCypher::Decrypt16Bits(int data, int pwhash)
{
	return RotateBits(data, pwhash, -1);
}

void SimpleBlockCypher::Bin2Text16(int val, char buf[5])
{
	buf[0] = NibbleToHex((val >> 12) & 0xF);
	buf[1] = NibbleToHex((val >> 8) & 0xF);
	buf[2] = NibbleToHex((val >> 4) & 0xF);
	buf[3] = NibbleToHex(val & 0xF);
	buf[4] = '\0';
}

int SimpleBlockCypher::Text2Bin16(const char* str)
{
	char buf[5];
	buf[0] = str[0];
	buf[1] = str[1];
	buf[2] = str[2];
	buf[3] = str[3];
	buf[4] = '\0';

	return Int32::Parse(buf);
}

void SimpleBlockCypher::Bin2Text8(int val, char buf[3])
{
	buf[0] = NibbleToHex((val >> 4) & 0xF);
	buf[1] = NibbleToHex(val & 0xF);
	buf[2] = '\0';
}

int SimpleBlockCypher::Text2Bin8(const char* str)
{
	return Int32::Parse(str, 4, 16);
}

int SimpleBlockCypher::BitPos(int val, int pos, int dir)
{
	for ( int x = pos+dir; x >= 0 && x < 32; x += dir )
	{
		if ( (val & (1<<x)) != 0 )
		{
			return x;
		}
	}
	throw Exception("bitPos error");
}

int SimpleBlockCypher::CountBits(int val, int bitlen)
{
	int count = 0;
	for ( int x = 0; x < bitlen; x++ )
	{
		if ( (val & (1<<x)) != 0 )
		{
			count++;
		}
	}
	return count;
}

int SimpleBlockCypher::SwapBits(int val, int blockf, int blockt, int dir)
{
	int bitcount = CountBits(blockf, 16);
	int fpos;
	int tpos;
	
	if ( 1 != dir && -1 != dir )
	{
		throw Exception("swap bits dir must be 1 or -1");
	}
	if ( dir > 0 )
	{
		fpos = -1;
		tpos = -1;
	}
	else
	{
		fpos = 16;
		tpos = 16;
	}
	for ( int x = 0; x < bitcount; x++ )
	{
		fpos = BitPos(blockf, fpos, dir);
		tpos = BitPos(blockt, tpos, dir);
		int fmask = (1<<fpos);
		int tmask = (1 << tpos);
		int fbit = val & fmask;
		int tbit = val & tmask;
		if ( fbit != 0 )
		{
			val |= tmask;
		}
		else
		{
			val &= ~tmask;
		}
		if ( tbit != 0 )
		{
			val |= fmask;
		}
		else
		{
			val &= ~fmask;
		}
	}
	return val;
}

int SimpleBlockCypher::RotateBits(int val, int hash, int dir)
{
	int mask = 1 | (1<<1) | (1<<2);
	int x;
	
	if ( 0 > dir )
	{
		x = 4;
	}
	else
	{
		x = 0;
	}
	while ( x >= 0 && x < 5 )
	{
		int bits = (hash & (mask << (x*3))) >> (x*3);
		if ( bits >= 8 )
		{
			throw Exception("bit problem");
		}
		int blockf = m_cypherBlocksFrom[bits];
		int blockt = m_cypherBlocksTo[bits];
		
		if ( CountBits(blockf, 16) != CountBits(blockt, 16) )
		{
			throw Exception("bits not equal at " + bits);
		}
		val = SwapBits(val, blockf, blockt, dir);
		
		x += dir;
	}
	return val;
}

char SimpleBlockCypher::NibbleToHex(int nibble)
{
	if ( nibble > 0xF )
	{
		throw Exception("internal error");
	}
	switch ( nibble )
	{
	case 0:
		return '0';
	case 1:
		return '1';
	case 2:
		return '2';
	case 3:
		return '3';
	case 4:
		return '4';
	case 5:
		return '5';
	case 6:
		return '6';
	case 7:
		return '7';
	case 8:
		return '8';
	case 9:
		return '9';
	case 10:
		return 'A';
	case 11:
		return 'B';
	case 12:
		return 'C';
	case 13:
		return 'D';
	case 14:
		return 'E';
	case 15:
		return 'F';
	default:
		throw Exception("NIBBLE error");
	}
}
