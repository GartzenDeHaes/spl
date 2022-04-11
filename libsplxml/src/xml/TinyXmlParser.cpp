/* Modified for the SPL project. */
/*
www.sourceforge.net/projects/tinyxml
Original code (2.0 and earlier )copyright (c) 2000-2002 Lee Thomason (www.grinninglizard.com)

This software is provided 'as-is', without any express or implied 
warranty. In no event will the authors be held liable for any 
damages arising from the use of this software.

Permission is granted to anyone to use this software for any 
purpose, including commercial applications, and to alter it and 
redistribute it freely, subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must 
not claim that you wrote the original software. If you use this
software in a product, an acknowledgment in the product documentation
would be appreciated but is not required.

2. Altered source versions must be plainly marked as such, and 
must not be misrepresented as being the original software.

3. This notice may not be removed or altered from any source 
distribution.
*/

#include <ctype.h>
#include <stdio.h>
#include <stddef.h>

#include <spl/xml/XmlAttribute.h>
#include <spl/xml/XmlComment.h>
#include <spl/xml/XmlDeclaration.h>
#include <spl/xml/XmlDocument.h>
#include <spl/xml/XmlElement.h>
#include <spl/xml/XmlNode.h>
#include <spl/xml/XmlText.h>
#include <spl/xml/XmlUnknown.h>
#include <spl/xml/XmlParsingData.h>

using namespace spl;

#ifdef TIXML_SAFE
#if defined(_MSC_VER) && (_MSC_VER >= 1400 )
// Microsoft visual studio, version 2005 and higher.
#define TIXML_SNPRINTF _snprintf_s
#define TIXML_SNSCANF  _snscanf_s
#define TIXML_SSCANF   sscanf_s
#elif defined(_MSC_VER) && (_MSC_VER >= 1200 )
// Microsoft visual studio, version 6 and higher.
//#pragma message( "Using _sn* functions." )
#define TIXML_SNPRINTF _snprintf
#define TIXML_SNSCANF  _snscanf
#define TIXML_SSCANF   sscanf
#elif defined(__GNUC__) && (__GNUC__ >= 3 )
// GCC version 3 and higher.s
//#warning( "Using sn* functions." )
#define TIXML_SNPRINTF snprintf
#define TIXML_SNSCANF  snscanf
#define TIXML_SSCANF   sscanf
#else
#define TIXML_SSCANF   sscanf
#endif
#endif	

struct Entity
{
	const char*     str;
	unsigned int	strLength;
	char		    chr;
};

#define NUM_ENTITY 5
#define MAX_ENTITY_LENGTH 6

// Note tha "PutString" hardcodes the same list. This
// is less flexible than it appears. Changing the entries
// or order will break putstring.	
static struct Entity entity[ NUM_ENTITY ] = 
{
	{ "&amp;",  5, '&' },
	{ "&lt;",   4, '<' },
	{ "&gt;",   4, '>' },
	{ "&quot;", 6, '\"' },
	{ "&apos;", 6, '\'' }
};

// Bunch of unicode info at:
//		http://www.unicode.org/faq/utf_bom.html
// Including the basic of this table, which determines the #bytes in the
// sequence from the lead byte. 1 placed for invalid sequences --
// although the result will be junk, pass it through as much as possible.
// Beware of the non-characters in UTF-8:	
//				ef bb bf (Microsoft "lead bytes")
//				ef bf be
//				ef bf bf 

const unsigned char TIXML_UTF_LEAD_0 = 0xefU;
const unsigned char TIXML_UTF_LEAD_1 = 0xbbU;
const unsigned char TIXML_UTF_LEAD_2 = 0xbfU;

// Table that returs, for a given lead byte, the total number of bytes
// in the UTF-8 sequence.
const int utf8ByteTable[256] = 
{
	//	0	1	2	3	4	5	6	7	8	9	a	b	c	d	e	f
	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	// 0x00
	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	// 0x10
	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	// 0x20
	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	// 0x30
	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	// 0x40
	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	// 0x50
	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	// 0x60
	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	// 0x70	End of ASCII range
	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	// 0x80 0x80 to 0xc1 invalid
	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	// 0x90 
	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	// 0xa0 
	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	// 0xb0 
	1,	1,	2,	2,	2,	2,	2,	2,	2,	2,	2,	2,	2,	2,	2,	2,	// 0xc0 0xc2 to 0xdf 2 byte
	2,	2,	2,	2,	2,	2,	2,	2,	2,	2,	2,	2,	2,	2,	2,	2,	// 0xd0
	3,	3,	3,	3,	3,	3,	3,	3,	3,	3,	3,	3,	3,	3,	3,	3,	// 0xe0 0xe0 to 0xef 3 byte
	4,	4,	4,	4,	4,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1	// 0xf0 0xf0 to 0xf4 4 byte, 0xf5 and higher invalid
};

void ConvertUTF32ToUTF8( unsigned long input, char* output, int* length )
{
	const unsigned long BYTE_MASK = 0xBF;
	const unsigned long BYTE_MARK = 0x80;
	const unsigned long FIRST_BYTE_MARK[7] = { 0x00, 0x00, 0xC0, 0xE0, 0xF0, 0xF8, 0xFC };

	if (input < 0x80) 
		*length = 1;
	else if ( input < 0x800 )
		*length = 2;
	else if ( input < 0x10000 )
		*length = 3;
	else if ( input < 0x200000 )
		*length = 4;
	else
	{ *length = 0; return; }	// This code won't covert this correctly anyway.

	output += *length;

	// Scary scary fall throughs.
	switch (*length) 
	{
	case 4:
		--output; 
		*output = (char)((input | BYTE_MARK) & BYTE_MASK); 
		input >>= 6;
	case 3:
		--output; 
		*output = (char)((input | BYTE_MARK) & BYTE_MASK); 
		input >>= 6;
	case 2:
		--output; 
		*output = (char)((input | BYTE_MARK) & BYTE_MASK); 
		input >>= 6;
	case 1:
		--output; 
		*output = (char)(input | FIRST_BYTE_MARK[*length]);
	}
}

// Return true if the next characters in the stream are any of the endTag sequences.
// Ignore case only works for english, and should only be relied on when comparing
// to English words: StringEqual( p, "version", true ) is fine.
static bool StringEqual(	const char* p,
						const char* endTag,
						bool ignoreCase,
						XmlEncoding encoding );

const char* spl::_xmlErrorStrings[ XmlNode::TIXML_ERROR_STRING_COUNT ] =
{
	"No error",
	"Error",
	"Failed to open file",
	"Memory allocation failed.",
	"Error parsing Element.",
	"Failed to read Element name",
	"Error reading Element value.",
	"Error reading Attributes.",
	"Error: empty tag.",
	"Error reading end tag.",
	"Error parsing Unknown.",
	"Error parsing Comment.",
	"Error parsing Declaration.",
	"Error document empty.",
	"Error null (0) or unexpected EOF found in input stream.",
	"Error parsing CDATA.",
	"Error when TiXmlDocument added to document, because TiXmlDocument can only be at the root.",
};

// None of these methods are reliable for any language except English.
// Good for approximation, not great for accuracy.
static int IsAlpha( unsigned char anyByte, XmlEncoding encoding );

static int IsAlphaNum( unsigned char anyByte, XmlEncoding encoding );

inline static int ToLower( int v, XmlEncoding encoding )
{
	if ( encoding == TIXML_ENCODING_UTF8 )
	{
		if ( v < 128 ) 
			return tolower( v );
		return v;
	}
	else
	{
		return tolower( v );
	}
}

void spl::_XmlEncodeString( const String& str, StringBuffer& outString )
{
	int i = 0;

	while( i < str.Length() )
	{
		unsigned char c = (unsigned char) str[i];

		if (    c == '&' 
			&& i < ( (int)str.Length() - 2 )
			&& str[i+1] == '#'
			&& str[i+2] == 'x' )
		{
			// Hexadecimal character reference.
			// Pass through unchanged.
			// &#xA9;	-- copyright symbol, for example.
			//
			// The -1 is a bug fix from Rob Laveaux. It keeps
			// an overflow from happening if there is no ';'.
			// There are actually 2 ways to exit this loop -
			// while fails (error case) and break (semicolon found).
			// However, there is no mechanism (currently) for
			// this function to return an error.
			while ( i < str.Length()-1 )
			{
				outString.Append( str.GetChars() + i, 1 );
				++i;
				if ( str[i] == ';' )
					break;
			}
		}
		else if ( c == '&' )
		{
			outString.Append( entity[0].str, entity[0].strLength );
			++i;
		}
		else if ( c == '<' )
		{
			outString.Append( entity[1].str, entity[1].strLength );
			++i;
		}
		else if ( c == '>' )
		{
			outString.Append( entity[2].str, entity[2].strLength );
			++i;
		}
		else if ( c == '\"' )
		{
			outString.Append( entity[3].str, entity[3].strLength );
			++i;
		}
		else if ( c == '\'' )
		{
			outString.Append( entity[4].str, entity[4].strLength );
			++i;
		}
		else if ( c < 32 )
		{
			// Easy pass at non-alpha/numeric/symbol
			// Below 32 is symbolic.
			char buf[ 32 ];

#if defined(TIXML_SNPRINTF)		
			TIXML_SNPRINTF( buf, sizeof(buf), "&#x%02X;", (unsigned) ( c & 0xff ) );
#else
			sprintf( buf, "&#x%02X;", (unsigned) ( c & 0xff ) );
#endif		

			//*ME:	warning C4267: convert 'size_t' to 'int'
			//*ME:	Int-Cast to make compiler happy ...
			outString.Append( buf, (int)strlen( buf ) );
			++i;
		}
		else
		{
			//char realc = (char) c;
			//outString->append( &realc, 1 );
			outString.Append((char) c);	// somewhat more efficient function call.
			++i;
		}
	}
}

static const char* SkipWhiteSpace( const char*, XmlEncoding encoding );

inline static bool IsWhiteSpace( char c )		
{ 
	return ( isspace( (unsigned char) c ) || c == '\n' || c == '\r' ); 
}

inline static bool IsWhiteSpace( int c )
{
	if ( c < 256 )
		return IsWhiteSpace( (char) c );
	return false;	// Again, only truly correct for English/Latin...but usually works.
}

/*	Reads an XML name into the string provided. Returns
a pointer just past the last character of the name,
or 0 if the function has an error.
*/
static const char* ReadName( const char* p, String* name, XmlEncoding encoding );

/*	Reads text. Returns a pointer past the given end tag.
Wickedly complex options, but it keeps the (sensitive) code in one place.
*/
static const char* ReadText(	const char* in,				// where to start
							StringBuffer& text,			// the string read
							bool ignoreWhiteSpace,		// whether to keep the white space
							const char* endTag,			// what ends this text
							bool ignoreCase,			// whether to ignore case in the end tag
							XmlEncoding encoding );	// the current encoding

// If an entity has been found, transform it into a character.
static const char* GetEntity( const char* in, char* value, int* length, XmlEncoding encoding );

// Get a character, while interpreting entities.
// The length can be from 0 to 4 bytes.
inline static const char* GetChar( const char* p, char* _value, int* length, XmlEncoding encoding )
{
	ASSERT( NULL != p );
	if ( encoding == TIXML_ENCODING_UTF8 )
	{
		*length = utf8ByteTable[ *((const unsigned char*)p) ];
		assert( *length >= 0 && *length < 5 );
	}
	else
	{
		*length = 1;
	}

	if ( *length == 1 )
	{
		if ( *p == '&' )
			return GetEntity( p, _value, length, encoding );
		*_value = *p;
		return p+1;
	}
	else if ( *length )
	{
		//strncpy( _value, p, *length );	// lots of compilers don't like this function (unsafe),
		// and the null terminator isn't needed
		for( int i=0; p[i] && i<*length; ++i ) {
			_value[i] = p[i];
		}
		return p + (*length);
	}
	else
	{
		// Not valid text.
		return 0;
	}
}

/*static*/ int IsAlpha( unsigned char anyByte, XmlEncoding /*encoding*/ )
{
	// This will only work for low-ascii, everything else is assumed to be a valid
	// letter. I'm not sure this is the best approach, but it is quite tricky trying
	// to figure out alhabetical vs. not across encoding. So take a very 
	// conservative approach.

	//	if ( encoding == TIXML_ENCODING_UTF8 )
	//	{
	if ( anyByte < 127 )
		return isalpha( anyByte );
	else
		return 1;	// What else to do? The unicode set is huge...get the english ones right.
	//	}
	//	else
	//	{
	//		return isalpha( anyByte );
	//	}
}


/*static*/ int IsAlphaNum( unsigned char anyByte, XmlEncoding /*encoding*/ )
{
	// This will only work for low-ascii, everything else is assumed to be a valid
	// letter. I'm not sure this is the best approach, but it is quite tricky trying
	// to figure out alhabetical vs. not across encoding. So take a very 
	// conservative approach.

	//	if ( encoding == TIXML_ENCODING_UTF8 )
	//	{
	if ( anyByte < 127 )
		return isalnum( anyByte );
	else
		return 1;	// What else to do? The unicode set is huge...get the english ones right.
	//	}
	//	else
	//	{
	//		return isalnum( anyByte );
	//	}
}

void XmlParsingData::Stamp( const char* now, XmlEncoding encoding )
{
	assert( now );

	// Do nothing if the tabsize is 0.
	if ( tabsize < 1 )
	{
		return;
	}

	// Get the current row, column.
	int row = cursor.row;
	int col = cursor.col;
	const char* p = stamp;
	assert( p );

	while ( p < now )
	{
		// Treat p as unsigned, so we have a happy compiler.
		const unsigned char* pU = (const unsigned char*)p;

		// Code contributed by Fletcher Dunn: (modified by lee)
		switch (*pU) {
case 0:
	// We *should* never get here, but in case we do, don't
	// advance past the terminating null character, ever
	return;

case '\r':
	// bump down to the next line
	++row;
	col = 0;				
	// Eat the character
	++p;

	// Check for \r\n sequence, and treat this as a single character
	if (*p == '\n') 
	{
		++p;
	}
	break;

case '\n':
	// bump down to the next line
	++row;
	col = 0;

	// Eat the character
	++p;

	// Check for \n\r sequence, and treat this as a single
	// character.  (Yes, this bizarre thing does occur still
	// on some arcane platforms...)
	if (*p == '\r') 
	{
		++p;
	}
	break;

case '\t':
	// Eat the character
	++p;

	// Skip to next tab stop
	col = (col / tabsize + 1) * tabsize;
	break;

case TIXML_UTF_LEAD_0:
	if ( encoding == TIXML_ENCODING_UTF8 )
	{
		if ( *(p+1) && *(p+2) )
		{
			// In these cases, don't advance the column. These are
			// 0-width spaces.
			if ( *(pU+1)==TIXML_UTF_LEAD_1 && *(pU+2)==TIXML_UTF_LEAD_2 )
				p += 3;	
			else if ( *(pU+1)==0xbfU && *(pU+2)==0xbeU )
				p += 3;	
			else if ( *(pU+1)==0xbfU && *(pU+2)==0xbfU )
				p += 3;	
			else
			{ p +=3; ++col; }	// A normal character.
		}
	}
	else
	{
		++p;
		++col;
	}
	break;

default:
	if ( encoding == TIXML_ENCODING_UTF8 )
	{
		// Eat the 1 to 4 byte utf8 character.
		int step = utf8ByteTable[*((const unsigned char*)p)];
		if ( step == 0 )
			step = 1;		// Error case from bad encoding, but handle gracefully.
		p += step;

		// Just advance one column, of course.
		++col;
	}
	else
	{
		++p;
		++col;
	}
	break;
		}
	}
	cursor.row = row;
	cursor.col = col;
	assert( cursor.row >= -1 );
	assert( cursor.col >= -1 );
	stamp = p;
	assert( stamp );
}


const char* SkipWhiteSpace( const char* p, XmlEncoding encoding )
{
	if ( !p || !*p )
	{
		return 0;
	}
	if ( encoding == TIXML_ENCODING_UTF8 )
	{
		while ( *p )
		{
			const unsigned char* pU = (const unsigned char*)p;

			// Skip the stupid Microsoft UTF-8 Byte order marks
			if (	*(pU+0)==TIXML_UTF_LEAD_0
				&& *(pU+1)==TIXML_UTF_LEAD_1 
				&& *(pU+2)==TIXML_UTF_LEAD_2 )
			{
				p += 3;
				continue;
			}
			else if(*(pU+0)==TIXML_UTF_LEAD_0
				&& *(pU+1)==0xbfU
				&& *(pU+2)==0xbeU )
			{
				p += 3;
				continue;
			}
			else if(*(pU+0)==TIXML_UTF_LEAD_0
				&& *(pU+1)==0xbfU
				&& *(pU+2)==0xbfU )
			{
				p += 3;
				continue;
			}

			if ( IsWhiteSpace( *p ) || *p == '\n' || *p =='\r' )		// Still using old rules for white space.
				++p;
			else
				break;
		}
	}
	else
	{
		while ( *p && IsWhiteSpace( *p ) || *p == '\n' || *p =='\r' )
			++p;
	}

	return p;
}

// One of TinyXML's more performance demanding functions. Try to keep the memory overhead down. The
// "assign" optimization removes over 10% of the execution time.
//
const char* ReadName( const char* p, String * name, XmlEncoding encoding )
{
	ASSERT(NULL != p);
	name->Set("");

	// Names start with letters or underscores.
	// Of course, in unicode, tinyxml has no idea what a letter *is*. The
	// algorithm is generous.
	//
	// After that, they can be letters, underscores, numbers,
	// hyphens, or colons. (Colons are valid ony for namespaces,
	// but tinyxml can't tell namespaces from names.)
	if ( NULL != p && *p && ( IsAlpha( (unsigned char) *p, encoding ) || *p == '_' ) )
	{
		int len = 0;
		const char *start = p;
		while( p && *p	&&	
			(		
			IsAlphaNum( (unsigned char ) *p, encoding ) 
			|| *p == '_'
			|| *p == '-'
			|| *p == '.'
			|| *p == ':' ) 
			)
		{
			++p;
			len++;
		}
		if ( len > 0 ) 
		{
			name->Set( String(start, len) );
		}
		return p;
	}
	return NULL;
}

const char* GetEntity( const char* p, char* value, int* length, XmlEncoding encoding )
{
	// Presume an entity, and pull it out.
	String ent;
	int i;
	*length = 0;

	if ( *(p+1) && *(p+1) == '#' && *(p+2) )
	{
		unsigned long ucs = 0;
		ptrdiff_t delta = 0;
		unsigned mult = 1;

		if ( *(p+2) == 'x' )
		{
			// Hexadecimal.
			if ( !*(p+3) ) 
				return 0;

			const char* q = p+3;
			q = strchr( q, ';' );

			if ( !q || !*q ) 
				return 0;

			delta = q-p;
			--q;

			while ( *q != 'x' )
			{
				if ( *q >= '0' && *q <= '9' )
					ucs += mult * (*q - '0');
				else if ( *q >= 'a' && *q <= 'f' )
					ucs += mult * (*q - 'a' + 10);
				else if ( *q >= 'A' && *q <= 'F' )
					ucs += mult * (*q - 'A' + 10 );
				else 
					return 0;
				mult *= 16;
				--q;
			}
		}
		else
		{
			// Decimal.
			if ( !*(p+2) ) 
				return 0;

			const char* q = p+2;
			q = strchr( q, ';' );

			if ( !q || !*q ) 
				return 0;

			delta = q-p;
			--q;

			while ( *q != '#' )
			{
				if ( *q >= '0' && *q <= '9' )
					ucs += mult * (*q - '0');
				else 
					return 0;
				mult *= 10;
				--q;
			}
		}
		if ( encoding == TIXML_ENCODING_UTF8 )
		{
			// convert the UCS to UTF-8
			ConvertUTF32ToUTF8( ucs, value, length );
		}
		else
		{
			*value = (char)ucs;
			*length = 1;
		}
		return p + delta + 1;
	}

	// Now try to match it.
	for( i=0; i<NUM_ENTITY; ++i )
	{
		if ( strncmp( entity[i].str, p, entity[i].strLength ) == 0 )
		{
			assert( strlen( entity[i].str ) == entity[i].strLength );
			*value = entity[i].chr;
			*length = 1;
			return ( p + entity[i].strLength );
		}
	}

	// So it wasn't an entity, its unrecognized, or something like that.
	*value = *p;	// Don't put back the last one, since we return it!
	//*length = 1;	// Leave unrecognized entities - this doesn't really work.
	// Just writes strange XML.
	return p+1;
}


bool StringEqual( const char* p,
				 const char* tag,
				 bool ignoreCase,
				 XmlEncoding encoding )
{
	assert( p );
	assert( tag );
	if ( !p || !*p )
	{
		assert( 0 );
		return false;
	}

	const char* q = p;

	if ( ignoreCase )
	{
		while ( *q && *tag && ToLower( *q, encoding ) == ToLower( *tag, encoding ) )
		{
			++q;
			++tag;
		}

		if ( *tag == 0 )
			return true;
	}
	else
	{
		while ( *q && *tag && *q == *tag )
		{
			++q;
			++tag;
		}

		if ( *tag == 0 )		// Have we found the end of the tag, and everything equal?
			return true;
	}
	return false;
}

const char* ReadText(	const char* p, 
					 StringBuffer& text, 
					 bool trimWhiteSpace, 
					 const char* endTag, 
					 bool caseInsensitive,
					 XmlEncoding encoding )
{
	text.SetLength(0);

	if (    !trimWhiteSpace			// certain tags always keep whitespace
		|| !XmlDocument::IsWhiteSpaceCondensed() )	// if true, whitespace is always kept
	{
		// Keep all the white space.
		while (	   p && *p
			&& !StringEqual( p, endTag, caseInsensitive, encoding )
			)
		{
			int len;
			char cArr[4] = { 0, 0, 0, 0 };
			p = GetChar( p, cArr, &len, encoding );
			text.Append( cArr, len );
		}
	}
	else
	{
		bool whitespace = false;

		// Remove leading white space:
		p = SkipWhiteSpace( p, encoding );
		while (	   p && *p
			&& !StringEqual( p, endTag, caseInsensitive, encoding ) )
		{
			if ( *p == '\r' || *p == '\n' )
			{
				whitespace = true;
				++p;
			}
			else if ( IsWhiteSpace( *p ) )
			{
				whitespace = true;
				++p;
			}
			else
			{
				// If we've found whitespace, add it before the
				// new character. Any whitespace just becomes a space.
				if ( whitespace )
				{
					text.Append(' ');
					whitespace = false;
				}
				int len;
				char cArr[4] = { 0, 0, 0, 0 };
				p = GetChar( p, cArr, &len, encoding );
				if ( len == 1 )
					text.Append(cArr[0]);	
				else
					text.Append( cArr, len );
			}
		}
	}
	if ( p ) 
		p += strlen( endTag );
	return p;
}

const char *XmlDocument::_Parse( const char *p, XmlParsingData* prevData, XmlEncoding encoding )
{
	if (NULL != prevData)
	{
		m_condenseWhiteSpace = prevData->condenseWs;
		m_tabsize = prevData->tabsize;
	}

	// Parse away, at the document level. Since a document
	// contains nothing but other tags, most of what happens
	// here is skipping white space.
	if ( NULL == p || '\0' == p[0] )
	{
		throw new XmlException( _xmlErrorStrings[TIXML_ERROR_DOCUMENT_EMPTY], 0, 0 );
	}

	// Note that, for a document, this needs to come
	// before the while space skip, so that parsing
	// starts from the pointer we are given.
	m_location.Clear();
	if ( prevData )
	{
		m_location.row = prevData->Cursor().row;
		m_location.col = prevData->Cursor().col;
	}
	else
	{
		m_location.row = 0;
		m_location.col = 0;
	}
	
	XmlParsingData data( p, TabSize(), m_location.row, m_location.col, m_condenseWhiteSpace);
	m_location = data.Cursor();

	if ( encoding == TIXML_ENCODING_UNKNOWN )
	{
		// Check for the Microsoft UTF-8 lead bytes.
		const unsigned char* pU = (const unsigned char*)p;
		if (	*(pU+0) && *(pU+0) == TIXML_UTF_LEAD_0
			&& *(pU+1) && *(pU+1) == TIXML_UTF_LEAD_1
			&& *(pU+2) && *(pU+2) == TIXML_UTF_LEAD_2 )
		{
			encoding = TIXML_ENCODING_UTF8;
			m_useMicrosoftBOM = true;
		}
	}

	p = SkipWhiteSpace( p, encoding );
	if ( !p )
	{
		throw new XmlException( _xmlErrorStrings[TIXML_ERROR_DOCUMENT_EMPTY], m_location.row, m_location.col );
	}

	while ( p && *p )
	{
		XmlNodePtr node = Identify( p, encoding );
        node.ValidateMem();

		if ( node.IsNotNull() )
		{
			p = node->_Parse( p, &data, encoding );
            node.ValidateMem();
			AppendChild( node );
		}
		else
		{
			break;
		}
		// Did we get encoding info?
		if (encoding == TIXML_ENCODING_UNKNOWN && node->IsDeclaration() )
		{
			XmlDeclarationPtr dec = node->ToDeclaration();
			const String& enc = dec->Encoding();

			if ( enc.Length() == 0 )
				encoding = TIXML_ENCODING_UTF8;
			else if ( StringEqual( enc.GetChars(), "UTF-8", true, TIXML_ENCODING_UNKNOWN ) )
				encoding = TIXML_ENCODING_UTF8;
			else if ( StringEqual( enc.GetChars(), "UTF8", true, TIXML_ENCODING_UNKNOWN ) )
				encoding = TIXML_ENCODING_UTF8;	// incorrect, but be nice
			else 
				encoding = TIXML_ENCODING_LEGACY;
		}
		p = SkipWhiteSpace( p, encoding );
	}
	// Was this empty?
	if ( m_firstChild.IsNull() ) 
	{
		throw new XmlException( _xmlErrorStrings[TIXML_ERROR_DOCUMENT_EMPTY], 0, 0 );
	}
	// All is well.
	return p;
}

//void XmlDocument::SetError( int err, const char* pError, XmlParsingData* data, XmlEncoding encoding )
//{	
//	// The first error in a chain is more accurate - don't set again!
//	if ( m_error )
//		return;
//
//	assert( err > 0 && err < TIXML_ERROR_STRING_COUNT );
//	m_error   = true;
//	m_errorId = err;
//	m_errorDesc = errorString[ m_errorId ];
//
//	m_errorLocation.Clear();
//	if ( pError && data )
//	{
//		data->Stamp( pError, encoding );
//		m_errorLocation = data->Cursor();
//	}
//}
//
static const char* _xmlHeader = { "<?xml" };
static const char* _commentHeader = { "<!--" };
static const char* _dtdHeader = { "<!" };
static const char* _cdataHeader = { "<![CDATA[" };

XmlNodePtr XmlNode::Identify( const char* p, XmlEncoding encoding )
{
	XmlNodePtr returnNode;

	p = SkipWhiteSpace( p, encoding );
	if( !p || !*p || *p != '<' )
	{
		return returnNode;
	}

	p = SkipWhiteSpace( p, encoding );

	if ( !p || !*p )
	{
		return returnNode;
	}

	// What is this thing? 
	// - Elements start with a letter or underscore, but xml is reserved.
	// - Comments: <!--
	// - Decleration: <?xml
	// - Everthing else is unknown to tinyxml.
	//

	if ( StringEqual( p, _xmlHeader, true, encoding ) )
	{
#ifdef DEBUG_PARSER
		TIXML_LOG( "XML parsing Declaration\n" );
#endif
		returnNode = (XmlNodePtr)XmlDeclarationPtr(new XmlDeclaration());
	}
	else if ( StringEqual( p, _commentHeader, false, encoding ) )
	{
#ifdef DEBUG_PARSER
		TIXML_LOG( "XML parsing Comment\n" );
#endif
		returnNode = (XmlNodePtr)XmlCommentPtr(new XmlComment());
	}
	else if ( StringEqual( p, _cdataHeader, false, encoding ) )
	{
#ifdef DEBUG_PARSER
		TIXML_LOG( "XML parsing CDATA\n" );
#endif
		XmlTextPtr text = XmlTextPtr(new XmlText( "" ));
		text->SetCDATA( true );
		returnNode = (XmlNodePtr)text;
	}
	else if ( StringEqual( p, _dtdHeader, false, encoding ) )
	{
#ifdef DEBUG_PARSER
		TIXML_LOG( "XML parsing Unknown(1)\n" );
#endif
		returnNode = (XmlNodePtr)XmlUnknownPtr(new XmlUnknown());
	}
	else if (    IsAlpha( *(p+1), encoding )
		|| *(p+1) == '_' )
	{
#ifdef DEBUG_PARSER
		TIXML_LOG( "XML parsing Element\n" );
#endif
		returnNode = (XmlNodePtr)XmlElementPtr(new XmlElement( "" ));
	}
	else
	{
#ifdef DEBUG_PARSER
		TIXML_LOG( "XML parsing Unknown(2)\n" );
#endif
		returnNode = (XmlNodePtr)XmlUnknownPtr(new XmlUnknown());
	}

	if ( returnNode.IsNotNull() )
	{
		// Set the parent, so it can report errors
		returnNode->m_parent = this;

		returnNode->m_self = returnNode;
	}
	return returnNode;
}

const char* XmlElement::_Parse( const char* p, XmlParsingData* data, XmlEncoding encoding )
{
	p = SkipWhiteSpace( p, encoding );

	if ( !p || !*p )
	{
		throw new XmlException( _xmlErrorStrings[TIXML_ERROR_PARSING_ELEMENT], NULL == data ? 0 : data->Cursor().row, NULL == data ? 0 : data->Cursor().col );
	}

	if ( data )
	{
		data->Stamp( p, encoding );
		m_location = data->Cursor();
	}

	if ( *p != '<' )
	{
		throw new XmlException( _xmlErrorStrings[TIXML_ERROR_PARSING_ELEMENT], NULL == data ? 0 : data->Cursor().row, NULL == data ? 0 : data->Cursor().col );
	}
	p = SkipWhiteSpace( p+1, encoding );

	// Read the name.
	const char* pErr = p;

	p = ReadName( p, &m_name, encoding );
	if ( !p || !*p )
	{
		throw new XmlException( _xmlErrorStrings[TIXML_ERROR_FAILED_TO_READ_ELEMENT_NAME], NULL == data ? 0 : data->Cursor().row, NULL == data ? 0 : data->Cursor().col );
	}

	StringBuffer endTag ("</");
	endTag.Append( m_name );
	endTag.Append( ">" );

	// Check for and read attributes. Also look for an empty
	// tag or an end tag.
	while ( p && *p )
	{
		pErr = p;
		p = SkipWhiteSpace( p, encoding );
		if ( !p || !*p )
		{
			throw new XmlException( _xmlErrorStrings[TIXML_ERROR_READING_ATTRIBUTES], NULL == data ? 0 : data->Cursor().row, NULL == data ? 0 : data->Cursor().col );
		}
		if ( *p == '/' )
		{
			++p;
			// Empty tag.
			if ( *p  != '>' )
			{
				throw new XmlException( _xmlErrorStrings[TIXML_ERROR_PARSING_EMPTY], NULL == data ? 0 : data->Cursor().row, NULL == data ? 0 : data->Cursor().col );
			}
			return (p+1);
		}
		else if ( *p == '>' )
		{
			// Done with attributes (if there were any.)
			// Read the value -- which can include other
			// elements -- read the end tag, and return.
			++p;
			p = ReadValue( p, data, encoding );		// Note this is an Element method, and will set the error if one happens.
			if ( !p || !*p ) 
			{
				// We were looking for the end tag, but found nothing.
				// Fix for [ 1663758 ] Failure to report error on bad XML
				throw new XmlException( _xmlErrorStrings[TIXML_ERROR_READING_END_TAG], NULL == data ? 0 : data->Cursor().row, NULL == data ? 0 : data->Cursor().col );
			}
			// We should find the end tag now
			if ( StringEqual( p, endTag.GetChars(), false, encoding ) )
			{
				p += endTag.Length();
				return p;
			}
			else
			{
				throw new XmlException( _xmlErrorStrings[TIXML_ERROR_READING_END_TAG], NULL == data ? 0 : data->Cursor().row, NULL == data ? 0 : data->Cursor().col );
			}
		}
		else
		{
			// Try to read an attribute:
			XmlAttributePtr attrib = XmlAttributePtr(new XmlAttribute());
			XmlDocumentPtr document = GetDocument();
			attrib->SetDocument( document );
			pErr = p;
			p = attrib->_Parse( p, data, encoding );

			if ( !p || !*p )
			{
				throw new XmlException( _xmlErrorStrings[TIXML_ERROR_PARSING_ELEMENT], NULL == data ? 0 : data->Cursor().row, NULL == data ? 0 : data->Cursor().col );
			}

			if ( m_attribs.IsNull() )
			{
				m_attribs = XmlAttributeCollectionPtr(new XmlAttributeCollection());
			}

			// Handle the strange case of double attributes:
			XmlAttributePtr node = m_attribs->Find( attrib->Name() );
			if ( node.IsNotNull() )
			{
				node.ValidateMem();
				node->SetValue( *attrib->Value() );
				return NULL;
			}

			m_attribs->Add( attrib );
		}
	}
	return p;
}

const char* XmlElement::ReadValue( const char* p, XmlParsingData* data, XmlEncoding encoding )
{
	// Read in text and elements in any order.
	const char* pWithWhiteSpace = p;
	p = SkipWhiteSpace( p, encoding );

	while ( p && *p )
	{
		if ( *p != '<' )
		{
			// Take what we have, make a text element.
			XmlTextPtr textNode = XmlTextPtr(new XmlText( "" ));
			XmlNodePtr xtextNode = (XmlNodePtr)textNode;
			textNode->m_self = xtextNode;
			textNode.ValidateMem();
			xtextNode.ValidateMem();

			if ( XmlDocument::IsWhiteSpaceCondensed() )
			{
				p = textNode->_Parse( p, data, encoding );
				textNode.ValidateMem();
			}
			else
			{
				// Special case: we want to keep the white space
				// so that leading spaces aren't removed.
				p = textNode->_Parse( pWithWhiteSpace, data, encoding );
				textNode.ValidateMem();
			}

			if ( !textNode->IsBlank() )
			{
				AppendChild( textNode );
			}
		} 
		else 
		{
			// We hit a '<'
			// Have we hit a new element or an end tag? This could also be
			// a TiXmlText in the "CDATA" style.
			if ( StringEqual( p, "</", false, encoding ) )
			{
				return p;
			}
			else
			{
				XmlNodePtr node = Identify( p, encoding );
				if ( node.IsNotNull() )
				{
					p = node->_Parse( p, data, encoding );
					AppendChild( node );
				}				
				else
				{
					return NULL;
				}
			}
		}
		pWithWhiteSpace = p;
		p = SkipWhiteSpace( p, encoding );
	}

	if ( !p )
	{
		throw new XmlException( _xmlErrorStrings[TIXML_ERROR_READING_ELEMENT_VALUE], 0, 0 );
	}	
	return p;
}

const char* XmlUnknown::_Parse( const char* p, XmlParsingData* data, XmlEncoding encoding )
{
	p = SkipWhiteSpace( p, encoding );

	if ( data )
	{
		data->Stamp( p, encoding );
		m_location = data->Cursor();
	}
	if ( !p || !*p || *p != '<' )
	{
		throw new XmlException( _xmlErrorStrings[TIXML_ERROR_PARSING_UNKNOWN], data->Cursor().row, data->Cursor().col );
	}

	++p;
	m_value = "";

	while ( p && *p && *p != '>' )
	{
		m_value.Append( *p );
		++p;
	}

	if ( !p )
	{
		throw new XmlException( _xmlErrorStrings[TIXML_ERROR_PARSING_UNKNOWN], data->Cursor().row, data->Cursor().col );
	}
	if ( *p == '>' )
		return p+1;
	return p;
}

const char* XmlComment::_Parse( const char* p, XmlParsingData* data, XmlEncoding encoding )
{
	m_value = "";

	p = SkipWhiteSpace( p, encoding );

	if ( data )
	{
		data->Stamp( p, encoding );
		m_location = data->Cursor();
	}
	const char* startTag = "<!--";
	const char* endTag   = "-->";

	if ( !StringEqual( p, startTag, false, encoding ) )
	{
		throw new XmlException( _xmlErrorStrings[TIXML_ERROR_PARSING_COMMENT], data->Cursor().row, data->Cursor().col );
	}
	p += strlen( startTag );

	// [ 1475201 ] TinyXML parses entities in comments
	// Oops - ReadText doesn't work, because we don't want to parse the entities.
	// p = ReadText( p, &value, false, endTag, false, encoding );
	//
	// from the XML spec:
	/*
	[Definition: Comments may appear anywhere in a document outside other markup; in addition, 
	they may appear within the document type declaration at places allowed by the grammar. 
	They are not part of the document's character data; an XML processor MAY, but need not, 
	make it possible for an application to retrieve the text of comments. For compatibility, 
	the string "--" (double-hyphen) MUST NOT occur within comments.] Parameter entity 
	references MUST NOT be recognized within comments.

	An example of a comment:

	<!-- declarations for <head> & <body> -->
	*/

	m_value = "";
	// Keep all the white space.
	while (	p && *p && !StringEqual( p, endTag, false, encoding ) )
	{
		m_value.Append( p, 1 );
		++p;
	}
	if ( p ) 
		p += strlen( endTag );

	return p;
}

static const char SINGLE_QUOTE = '\'';
static const char DOUBLE_QUOTE = '\"';

const char* XmlAttribute::_Parse( const char* p, XmlParsingData* data, XmlEncoding encoding )
{
	ASSERT(NULL != p);

	p = SkipWhiteSpace( p, encoding );

	if ( !p || !*p ) 
		return NULL;

	//	int tabsize = 4;
	//	if ( document )
	//		tabsize = document->TabSize();

	if ( data )
	{
		data->Stamp( p, encoding );
		m_location = data->Cursor();
	}

	// Read the name, the '=' and the value.
	const char* pErr = p;
	p = ReadName( p, &m_name, encoding );
	if ( !p || !*p )
	{
		throw new XmlException( _xmlErrorStrings[TIXML_ERROR_READING_ATTRIBUTES], data->Cursor().row, data->Cursor().col );
	}

	p = SkipWhiteSpace( p, encoding );
	if ( !p || !*p || *p != '=' )
	{
		throw new XmlException( _xmlErrorStrings[TIXML_ERROR_READING_ATTRIBUTES], data->Cursor().row, data->Cursor().col );
	}

	++p;	// skip '='
	p = SkipWhiteSpace( p, encoding );
	if ( !p || !*p )
	{
		throw new XmlException( _xmlErrorStrings[TIXML_ERROR_READING_ATTRIBUTES], data->Cursor().row, data->Cursor().col );
	}

	const char* end;

	if ( *p == SINGLE_QUOTE )
	{
		++p;
		end = "\'";		// single quote in string
		p = ReadText( p, m_value, false, end, false, encoding );
	}
	else if ( *p == DOUBLE_QUOTE )
	{
		++p;
		end = "\"";		// double quote in string
		p = ReadText( p, m_value, false, end, false, encoding );
	}
	else
	{
		// All attribute values should be in single or double quotes.
		// But this is such a common error that the parser will try
		// its best, even without them.
		m_value = "";
		while (    p && *p											// existence
			&& !IsWhiteSpace( *p ) && *p != '\n' && *p != '\r'	// whitespace
			&& *p != '/' && *p != '>' )							// tag end
		{
			if ( *p == SINGLE_QUOTE || *p == DOUBLE_QUOTE ) 
			{
				// [ 1451649 ] Attribute values with trailing quotes not handled correctly
				// We did not have an opening quote but seem to have a 
				// closing one. Give up and throw an error.
				throw new XmlException( _xmlErrorStrings[TIXML_ERROR_READING_ATTRIBUTES], data->Cursor().row, data->Cursor().col );
			}
			m_value.Append( *p );
			++p;
		}
	}
	return p;
}

const char* XmlText::_Parse( const char* p, XmlParsingData* data, XmlEncoding encoding )
{
	m_value = "";

	if ( data )
	{
		data->Stamp( p, encoding );
		m_location = data->Cursor();
	}

	const char* const startTag = "<![CDATA[";
	const char* const endTag   = "]]>";

	if ( m_cdata || StringEqual( p, startTag, false, encoding ) )
	{
		m_cdata = true;

		if ( !StringEqual( p, startTag, false, encoding ) )
		{
			throw new XmlException( _xmlErrorStrings[TIXML_ERROR_PARSING_CDATA], data->Cursor().row, data->Cursor().col );
		}
		p += strlen( startTag );

		// Keep all the white space, ignore the encoding, etc.
		while (	p && *p	&& !StringEqual( p, endTag, false, encoding ) )
		{
			m_value.Append( *p );
			++p;
		}

		StringBuffer dummy;
		p = ReadText( p, dummy, false, endTag, false, encoding );
		return p;
	}
	else
	{
		bool ignoreWhite = true;

		const char* end = "<";
		p = ReadText( p, m_value, ignoreWhite, end, false, encoding );
		if ( p )
			return p-1;	// don't truncate the '<'
		return 0;
	}
}

const char* XmlDeclaration::_Parse( const char* p, XmlParsingData* data, XmlEncoding _encoding )
{
	p = SkipWhiteSpace( p, _encoding );
	// Find the beginning, find the end, and look for
	// the stuff in-between.

	if ( !p || !*p || !StringEqual( p, "<?xml", true, _encoding ) )
	{
		throw new XmlException( _xmlErrorStrings[TIXML_ERROR_PARSING_DECLARATION], data->Cursor().row, data->Cursor().col );
	}
	if ( data )
	{
		data->Stamp( p, _encoding );
		m_location = data->Cursor();
	}
	p += 5;

	version = "";
	encoding = "";
	standalone = "";

	while ( p && *p )
	{
		if ( *p == '>' )
		{
			++p;
			return p;
		}

		p = SkipWhiteSpace( p, _encoding );
		if ( StringEqual( p, "version", true, _encoding ) )
		{
			XmlAttribute attrib;
			p = attrib._Parse( p, data, _encoding );		
			version = *attrib.Value();
		}
		else if ( StringEqual( p, "encoding", true, _encoding ) )
		{
			XmlAttribute attrib;
			p = attrib._Parse( p, data, _encoding );		
			encoding = *attrib.Value();
		}
		else if ( StringEqual( p, "standalone", true, _encoding ) )
		{
			XmlAttribute attrib;
			p = attrib._Parse( p, data, _encoding );		
			standalone = *attrib.Value();
		}
		else
		{
			// Read over whatever it is.
			while( p && *p && *p != '>' && !IsWhiteSpace( *p ) )
				++p;
		}
		ValidateMem();
	}
	return NULL;
}

bool XmlText::IsBlank() const
{
	for ( int i=0; i<m_value.Length(); i++ )
	{
		if ( !IsWhiteSpace( m_value[i] ) )
		{
			return false;
		}
	}
	return true;
}

