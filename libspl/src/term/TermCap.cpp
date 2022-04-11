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
#include <spl/types.h>
#include <spl/Debug.h>
#include <spl/Int32.h>
#include <spl/term/TermCap.h>

#include <ctype.h>
#ifdef HAVE_STRING_H
#include <string.h>
#endif
#ifdef HAVE_IO_H
#include <io.h>
#endif
#ifdef HAVE_FCNTL_H
#include <fcntl.h>
#endif
#ifdef HAVE_SYS_FCNTL_H
#include <sys/fcntl.h>
#endif

using namespace spl;

#ifndef HAVE_BCOPY
/* Do this after the include, in case string.h prototypes bcopy.  */
/*#if (defined(HAVE_STRING_H) || defined(STDC_HEADERS))	&& !defined(bcopy)*/
#define	bcopy(s, d,	n) memcpy ((d),	(s), (n))
/*#endif*/
#endif

#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif
#ifdef _POSIX_VERSION
#include <fcntl.h>
#endif

#ifndef	NULL
#define	NULL (char *) 0
#endif

#ifndef	O_RDONLY
#define	O_RDONLY 0
#endif

/* Table, indexed by a character in	range 0100 to 0140 with	0100 subtracted,
gives meaning of character following \,	or a space if no special meaning.
Eight characters per line within the string.  */

static char	esctab[]
= "	\007\010  \033\014 \
  \012 \
  \015 \011	\013 \
  ";

extern const char *termcap_src_new;

extern char *tparam (const char *ctlstring, char *buffer, int size, ...);
extern char *tgoto (const char *cm, int hpos, int vpos);

static bool _parseEntry( Hashtable<String, String>& caps, String& tcfile, int *pos );

bool _parseTerm( Hashtable<String, String>& caps, String& tcfile, const char *termname)
{
	char ch;
	bool isesc = false;
	bool linecont = false;
	int pos = 0;
	char term_name[128];
	int term_name_len;
	int term_desc_pos = 0;
	char *tcenv	= NULL;		/* TERMCAP value, if it	contains :tc=.	*/
	char *indirect = NULL;	/* Terminal	type in	:tc= in	TERMCAP	value.	*/

	StrCpyLen( term_name, termname, 128 );
	term_name_len = (int)strlen(term_name);
	while ( term_name_len > 0 )
	{
		/*DEBUG_CLEAR_MEM_CHECK_POINTS();
		termcapCheckMem( capsbuf);
		DEBUG_NOTE_MEM_ALLOCATION(tcfile);
		stringCheckMem( tcfile );
		DEBUG_DUMP_MEM_LEAKS();
		*/
		if ( 0 > (pos = tcfile.IndexOf(term_name, pos)) )
		{
			return false;
		}
		ASSERT( tcfile.CharAt( pos ) == term_name[0] );
		if ( tcfile.CharAt( pos+term_name_len ) != '|' )
		{
			pos++;
			continue;
		}
		if ( tcfile.CharAt( pos-1 ) == '#' )
		{
			pos++;
			continue;
		}
		if ( 0 == term_desc_pos )
		{
			while ( '\n' != (ch = tcfile.CharAt(pos++)) )
			{
			}
		}
		/*
		 *  We've located the terminal entry, load the colin delimited entries.
		 *  The terminal entry is a single line, using line contiueation chars '\'
		 */
		while ( pos < tcfile.Length() )
		{
			if ( ! _parseEntry( caps, tcfile, &pos ) )
			{
				return false;
			}
			linecont = false;
			ch = tcfile.CharAt(pos);
			if ( !isalnum(ch) )
			{
				while ( pos < tcfile.Length() && '\n' != ch && ':' != ch )
				{
					if ( '\\' == ch )
					{
						linecont = true;
					}
					ch = tcfile.CharAt(pos++);
				}
				if ( !linecont && ':' != ch )
				{
					pos = tcfile.Length();
				}
			}
			else if ( ':' != ch )
			{
				ch = tcfile.CharAt(--pos);
				ASSERT( ':' == ch );
			}
		}
		if ( pos == tcfile.Length() )
		{
			break;
		}
	}
	return true;
}

static const int _base10 = 10;
static const char _cHexa[] = {'A','B','C','D','E','F'};
static const int _iHexaNumeric[] = {10,11,12,13,14,15};
static const int _iHexaIndices[] = {0,1,2,3,4,5};
static const int _asciiDiff = 48;

static int _baseToInt(char *sbase, const int numbase)
{
	int i;
    int dec = 0;
    int b;
    int iProduct = 1;
	int sbaselen = (int)strlen(sbase);

    for ( i = sbaselen - 1; i >= 0; i--, iProduct *= numbase )
    {
        char ch = sbase[i];
	    if ( numbase > _base10 && isalpha(ch) )
		{
			if ( ch >= 'A' )
			{
				ASSERT( 'A' > 'a' );
				b = _iHexaNumeric[ch-'A'];
			}
			else
			{
				ASSERT( 'A' > 'a' );
				b = _iHexaNumeric[ch-'a'];
			}
		}
        else 
		{
            b = (int) sbase[i] - _asciiDiff;
		}
        dec += (b * iProduct);
    } 
    return dec; 
}

static bool _parseEntry( Hashtable<String, String>& caps, String& tcfile, int *pos )
{
	#define CAPBUFSIZE 256
	#define CAPKEYBUFSIZE 8

	char _keybuf[CAPKEYBUFSIZE];
	char _capbuf[CAPBUFSIZE];

	bool isesc = false;
	int capbufpos = 0;
	int keybufpos = 0;
	char ch = tcfile.CharAt((*pos)++);

	/* advance to ':', or eof */
	while ( ':' != ch && *pos < tcfile.Length() )
	{
		ch = tcfile.CharAt((*pos)++);
	}
	if ( *pos >= tcfile.Length() )
	{
		return false;
	}
	ASSERT( ':' == ch );

	/* load the entry's key */
	ch = tcfile.CharAt((*pos)++);
	while ( ':' != ch && '#' != ch && '=' != ch && *pos < tcfile.Length() )
	{
		_keybuf[keybufpos++] = ch;
		ASSERT(keybufpos < CAPKEYBUFSIZE);
		ch = tcfile.CharAt((*pos)++);
	}
	_keybuf[keybufpos] = '\0';
	if ( ':' == ch )
	{
		/* flag */
		caps.Set( String(&_keybuf[0]), String(&_keybuf[0]) );
	}

	/* load the value */
	while ( ':' != (ch = tcfile.CharAt((*pos)++)) )
	{
		if ( isesc )
		{
			switch ( ch )
			{
			case 'E':
				_capbuf[capbufpos++] = 27;
				break;
			case '\\':
				_capbuf[capbufpos++] = '\\';
				break;
			default:
				if ( isdigit(ch) )
				{
					/* three digit octal */
					char buf[4];
					buf[0] = ch;
					buf[1] = '\0';
					ch = tcfile.CharAt(*pos);
					if ( isdigit(ch) )
					{
						buf[1] = ch;
						buf[2] = '\0';
						(*pos)++;
						ch = tcfile.CharAt(*pos);
						if ( isdigit( ch ) )
						{
							buf[2] = ch;
							buf[3] = '\0';
							(*pos)++;
						}
					}
					_capbuf[capbufpos++] = _baseToInt(buf, 8);
				}
				else
				{
					_capbuf[capbufpos++] = '\\';
					_capbuf[capbufpos++] = ch;
				}
			}
			isesc = false;
		}
		else
		{
			if ( '\\' == ch )
			{
				isesc = true;
			}
			else if ( '\r' == ch || '\n' == ch || '\t' == ch )
			{
				isesc = false;
			}
			else
			{
				_capbuf[capbufpos++] = ch;
			}
		}
		ASSERT( CAPBUFSIZE > capbufpos );
	}
	_capbuf[capbufpos] = '\0';
	if ( strcmp(_keybuf, "tc") == 0 )
	{
		_parseTerm(caps, tcfile, &_capbuf[0]);
	}
	else
	{
		String key(&_keybuf[0]);
		if ( caps.ContainsKey( key ) )
		{
			caps.Remove( key );
		}
		String val = String(&_capbuf[0]);
		caps.Set( key, val );
	}
	capbufpos = 0;
	return true;
}

TermCap::TermCap() 
: m_termname(), m_capex(), m_caps()
{
	m_eos = 0xcd;
	SetTerm("ansi");
}

TermCap::~TermCap()
{
}

void TermCap::SetTerm(const String termName)
{
	if ( m_termname.Equals( termName ) )
	{
		return;
	}
	m_capex.Lock();

	m_caps.Clear();

	String tcfile(termcap_src_new);

	if ( ! _parseTerm(m_caps, tcfile, termName.GetChars()) )
	{
		throw new Exception("Term not found");
	}
	ASSERT(0xcd == m_eos);

	if ( termName.IndexOf("ansi.sys") > -1 || termName.IndexOf("pcansi") > -1 )
	{
		m_acsmap[ULCORNER] = DOS_ULCORNER;
		m_acsmap[LLCORNER] = DOS_LLCORNER;
		m_acsmap[URCORNER] = DOS_URCORNER;
		m_acsmap[LRCORNER] = DOS_LRCORNER;
		m_acsmap[LTEE] = DOS_LTEE;
		m_acsmap[RTEE] = DOS_RTEE;
		m_acsmap[BTEE] = DOS_BTEE;
		m_acsmap[TTEE] = DOS_TTEE;
		m_acsmap[HLINE] = DOS_HLINE;
		m_acsmap[VLINE] = DOS_VLINE;
		m_acsmap[PLUS] = DOS_PLUS;
		m_acsmap[S1] = DOS_S1;
		m_acsmap[S9] = DOS_S9;
		m_acsmap[DIAMOND] = DOS_DIAMOND; 
		m_acsmap[CKBOARD] = DOS_CKBOARD;
		m_acsmap[DEGREE] = DOS_DEGREE;
		m_acsmap[PLMINUS] = DOS_PLMINUS;
		m_acsmap[BULLET] = DOS_BULLET;
		m_acsmap[LARROW] = DOS_LARROW;
		m_acsmap[RARROW] = DOS_RARROW;
		m_acsmap[DARROW] = DOS_DARROW;
		m_acsmap[UARROW] = DOS_UARROW;
		m_acsmap[BOARD] = DOS_BOARD;
		m_acsmap[LANTERN] = DOS_LANTERN;
		m_acsmap[BLOCK] = DOS_BLOCK;
		m_acsmap[S3] = DOS_S3;
		m_acsmap[S7] = DOS_S7;
		m_acsmap[LEQUAL] = DOS_LEQUAL;
		m_acsmap[GEQUAL] = DOS_GEQUAL;
		m_acsmap[PI] = DOS_PI;
		m_acsmap[NEQUAL] = DOS_NEQUAL;
		m_acsmap[STERLING] = DOS_STERLING;
		m_isdos = true;
		m_hasacs = false;
		m_hasS0S1 = false;
	}
	else if (termName.IndexOf("cygwin") > -1)
	{
		m_acsmap[ULCORNER] = '*';
		m_acsmap[LLCORNER] = '*';
		m_acsmap[URCORNER] = '*';
		m_acsmap[LRCORNER] = '*';
		m_acsmap[LTEE] = '+';
		m_acsmap[RTEE] = '+';
		m_acsmap[BTEE] = '+';
		m_acsmap[TTEE] = '+';
		m_acsmap[HLINE] = '-';
		m_acsmap[VLINE] = '|';
		m_acsmap[PLUS] = '+';
		m_acsmap[S1] = '-';
		m_acsmap[S9] = '_';
		m_acsmap[DIAMOND] = (char)14; 
		m_acsmap[CKBOARD] = (char)23;
		m_acsmap[DEGREE] = '.';
		m_acsmap[PLMINUS] = ' ';
		m_acsmap[BULLET] = '>';
		m_acsmap[LARROW] = (char)17;
		m_acsmap[RARROW] = (char)16;
		m_acsmap[DARROW] = (char)31;
		m_acsmap[UARROW] = (char)30;
		m_acsmap[BOARD] = DOS_BOARD;
		m_acsmap[LANTERN] = DOS_LANTERN;
		m_acsmap[BLOCK] = (char)25;
		m_acsmap[S3] = (char)22;
		m_acsmap[S7] = (char)13;
		m_acsmap[LEQUAL] = (char)28;
		m_acsmap[GEQUAL] = (char)29;
		m_acsmap[PI] = (char)30;
		m_acsmap[NEQUAL] = DOS_NEQUAL;
		m_acsmap[STERLING] = (char)32;
		m_isdos = false;
		m_hasacs = true;
		m_hasS0S1 = false;
	}
	else if (termName.IndexOf("linux") > -1 || termName.IndexOf("xterm") > -1)
	{
		m_acsmap[ULCORNER] = (char)108;
		m_acsmap[LLCORNER] = (char)109;
		m_acsmap[URCORNER] = (char)107;
		m_acsmap[LRCORNER] = (char)106;
		m_acsmap[LTEE] = (char)116;
		m_acsmap[RTEE] = (char)117;
		m_acsmap[BTEE] = (char)118;
		m_acsmap[TTEE] = (char)119;
		m_acsmap[HLINE] = (char)113;/*'-';*/
		m_acsmap[VLINE] = (char)120;/*'|';*/
		m_acsmap[PLUS] = (char)110;/*'+';*/
		m_acsmap[S1] = (char)113;
		m_acsmap[S9] = (char)115;
		m_acsmap[DIAMOND] = (char)96; 
		m_acsmap[CKBOARD] = (char)97;
		m_acsmap[DEGREE] = '.';
		m_acsmap[PLMINUS] = ' ';
		m_acsmap[BULLET] = '>';
		m_acsmap[LARROW] = '<'; /*(char)60;*/
		m_acsmap[RARROW] = (char)62;
		m_acsmap[DARROW] = (char)19;
		m_acsmap[UARROW] = (char)94;
		m_acsmap[BOARD] = DOS_BOARD;
		m_acsmap[LANTERN] = DOS_LANTERN;
		m_acsmap[BLOCK] = (char)25;
		m_acsmap[S3] = (char)22;
		m_acsmap[S7] = (char)13;
		m_acsmap[LEQUAL] = (char)28;
		m_acsmap[GEQUAL] = (char)29;
		m_acsmap[PI] = (char)30;
		m_acsmap[NEQUAL] = DOS_NEQUAL;
		m_acsmap[STERLING] = (char)32;
		m_isdos = false;
		m_hasacs = false;
		m_hasS0S1 = true;
		ASSERT( m_hasS0S1 );
	}
	else if (termName.IndexOf("vt100") > -1)
	{
		m_acsmap[ULCORNER] = ACS_ULCORNER;
		m_acsmap[LLCORNER] = ACS_LLCORNER;
		m_acsmap[URCORNER] = ACS_URCORNER;
		m_acsmap[LRCORNER] = ACS_LRCORNER;
		m_acsmap[LTEE] = ACS_LTEE;
		m_acsmap[RTEE] = ACS_RTEE;
		m_acsmap[BTEE] = ACS_BTEE;
		m_acsmap[TTEE] = ACS_TTEE;
		m_acsmap[HLINE] = ACS_HLINE;
		m_acsmap[VLINE] = ACS_VLINE;
		m_acsmap[PLUS] = ACS_PLUS;
		m_acsmap[S1] = ACS_S1;
		m_acsmap[S9] = ACS_S9;
		m_acsmap[DIAMOND] = ACS_DIAMOND; 
		m_acsmap[CKBOARD] = ACS_CKBOARD;
		m_acsmap[DEGREE] = ACS_DEGREE;
		m_acsmap[PLMINUS] = ACS_PLMINUS;
		m_acsmap[BULLET] = ACS_BULLET;
		m_acsmap[LARROW] = ACS_LARROW;
		m_acsmap[RARROW] = ACS_RARROW;
		m_acsmap[DARROW] = ACS_DARROW;
		m_acsmap[UARROW] = ACS_UARROW;
		m_acsmap[BOARD] = ACS_BOARD;
		m_acsmap[LANTERN] = ACS_LANTERN;
		m_acsmap[BLOCK] = ACS_BLOCK;
		m_acsmap[S3] = ACS_S3;
		m_acsmap[S7] = ACS_S7;
		m_acsmap[LEQUAL] = ACS_LEQUAL;
		m_acsmap[GEQUAL] = ACS_GEQUAL;
		m_acsmap[PI] = ACS_PI;
		m_acsmap[NEQUAL] = ACS_NEQUAL;
		m_acsmap[STERLING] = ACS_STERLING;
		m_isdos = false;
		m_hasacs = true;
		m_hasS0S1 = false;
	}
	else
	{
		m_acsmap[ULCORNER] = ANSI_ULCORNER;
		m_acsmap[LLCORNER] = ANSI_LLCORNER;
		m_acsmap[URCORNER] = ANSI_URCORNER;
		m_acsmap[LRCORNER] = ANSI_LRCORNER;
		m_acsmap[LTEE] = ANSI_LTEE;
		m_acsmap[RTEE] = ANSI_RTEE;
		m_acsmap[BTEE] = ANSI_BTEE;
		m_acsmap[TTEE] = ANSI_TTEE;
		m_acsmap[HLINE] = ANSI_HLINE;
		m_acsmap[VLINE] = ANSI_VLINE;
		m_acsmap[PLUS] = ANSI_PLUS;
		m_acsmap[S1] = ANSI_S1;
		m_acsmap[S9] = ANSI_S9;
		m_acsmap[DIAMOND] = ANSI_DIAMOND; 
		m_acsmap[CKBOARD] = ANSI_CKBOARD;
		m_acsmap[DEGREE] = ANSI_DEGREE;
		m_acsmap[PLMINUS] = ANSI_PLMINUS;
		m_acsmap[BULLET] = ANSI_BULLET;
		m_acsmap[LARROW] = ANSI_LARROW;
		m_acsmap[RARROW] = ANSI_RARROW;
		m_acsmap[DARROW] = ANSI_DARROW;
		m_acsmap[UARROW] = ANSI_UARROW;
		m_acsmap[BOARD] = ANSI_BOARD;
		m_acsmap[LANTERN] = ANSI_LANTERN;
		m_acsmap[BLOCK] = ANSI_BLOCK;
		m_acsmap[S3] = ANSI_S3;
		m_acsmap[S7] = ANSI_S7;
		m_acsmap[LEQUAL] = ANSI_LEQUAL;
		m_acsmap[GEQUAL] = ANSI_GEQUAL;
		m_acsmap[PI] = ANSI_PI;
		m_acsmap[NEQUAL] = ANSI_NEQUAL;
		m_acsmap[STERLING] = ANSI_STERLING;
		m_isdos = false;
		m_hasacs = true;
		m_hasS0S1 = false;
	}
	ASSERT(0xcd == m_eos);

	m_termname.Set( termName );
	m_capex.Unlock();
}

int TermCap::GetCapNum(TemCapCode code)
{
	String capname = String(m_capCodes[(int)code]);
	if ( ! m_caps.ContainsKey( capname ) )
	{
		return -1;
	}
	String num = m_caps.Get(capname);
	return Int32::Parse(num);
}

bool TermCap::GetCapFlag(TemCapCode code)
{
	String capname = String(m_capCodes[(int)code]);
	if ( ! m_caps.ContainsKey( capname ) )
	{
		return false;
	}
	return true;
}

bool tgets( String *capstr, char *buf, const int buflen )
{
	int bufpos = 0;
	int cappos = 0;
	while ( cappos < capstr->Length() && bufpos < buflen )
	{
		char ch = capstr->CharAt(cappos++);
		if ( '^' == ch )
		{
			// control
			ch = capstr->CharAt(cappos++);
			if ( isalpha(ch) )
			{
				if ( islower(ch) )
				{
					buf[bufpos++] = (ch - 'a') + 1;
				}
				else
				{
					buf[bufpos++] = (ch - 'A') + 1;
				}
			}
			else if ( isdigit(ch) )
			{
				if ( '2' == ch )
				{
					buf[bufpos++] = 0;
				}
				else if ( 6 == ch )
				{
					buf[bufpos++] = 30;
				}
				else
				{
					return false;
				}
			}
			else if ( '[' == ch )
			{
				buf[bufpos++] = 27;
			}
			else if ( ']' == ch )
			{
				buf[bufpos++] = 29;
			}
			else
			{
				return false;
			}
		}
		else if ( '\\' == buf[0] )
		{
			// escape
			ch = capstr->CharAt(cappos++);
			switch ( ch )
			{
			case 'E':
				buf[bufpos++] = 27;
				break;
			case 't':
				buf[bufpos++] = '\t';
				break;
			case '\\':
				buf[bufpos++] = '\\';
				break;
			case '^':
				buf[bufpos++] = '^';
				break;
			default:
				return false;
			}
		}
		else
		{
			buf[bufpos++] = ch;
		}
	}
	ASSERT( bufpos < buflen );
	buf[bufpos] = '\0';
	return true;
}

bool TermCap::GetCapString(TemCapCode code, char *buf, int buflen)
{
	String capname = String(m_capCodes[(int)code]);
	if ( ! m_caps.ContainsKey( capname ) )
	{
		return false;
	}
	String str = m_caps.Get(capname);
	return tgets(&str, buf, buflen);
}

bool TermCap::GetCapString(TemCapCode code, StringBuffer *sb)
{
	char buf[80];
	bool ret = GetCapString(code, buf, 80);
	if ( ret )
	{
		sb->Append(buf);
	}
	return ret;
}

bool TermCap::FormatParams(const char *cmdstr, char *buf, int buflen, int param1)
{
	char *result = tparam(cmdstr, buf, buflen, param1);
	if ( result != buf )
	{
		free(result);
		return false;
	}
	return true;
}

bool TermCap::FormatGoto(const char *cmstr, char *buf, int buflen, int x, int y)
{
	char *sbuf = tgoto(cmstr, x, y);
	if ( (int)strlen(sbuf) >= buflen )
	{
		return false;
	}
	strcpy(buf, sbuf);
	return true;
}

bool TermCap::FormatGoto(const char *cmstr, StringBuffer *sb, int x, int y)
{
	char *sbuf = tgoto(cmstr, x, y);
	sb->Append(sbuf);
	return true;
}

bool TermCap::PrintParams(const char *cmdstr, char *buf, int buflen, int param1, int param2)
{
	char *result = tparam(cmdstr, buf, buflen, param1, param2);
	if ( result != buf )
	{
		free(result);
		return false;
	}
	return true;
}

bool TermCap::PrintParams(const char *cmdstr, char *buf, int buflen, int param1, int param2, int param3, int param4)
{
	char *result = tparam(cmdstr, buf, buflen, param1, param2, param3, param4);
	if ( result != buf )
	{
		free(result);
		return false;
	}
	return true;
}

bool TermCap::HasTerminalDef( const String& termName )
{
	String tcfile(termcap_src_new);
	return tcfile.IndexOf(termName) >= 0;
}

#ifdef DEBUG
void TermCap::ValidateMem() const
{
	//m_capex.Lock();
	m_termname.ValidateMem();
	m_caps.ValidateMem();
	ASSERT( m_eos == 0xcd );
	//m_capex.Unlock();
}

void TermCap::CheckMem() const
{
	//m_capex.Lock();
	m_termname.CheckMem();
	m_caps.CheckMem();
	ASSERT( m_eos == 0xcd );
	//m_capex.Unlock();
}
#endif

const char *TermCap::m_capCodes[] = {
 "ae", "al",
 "AL", "am",
 "as", "bc",
 "bl", "bs",
 "bt", "bw",
 "CC", "cd",
 "ce", "ch",
 "cl", "cm",
 "CM", "co",
 "cr", "cs",
 "cS", "ct",
 "cv", "da",
 "db", "dB",
 "dc", "dC",
 "DC", "dF",
 "dl", "DL",
 "dm", "dN",
 "do", "DO",
 "ds", "dT",
 "ec", "ed",
 "ei", "eo",
 "es", "ff",
 "fs", "gn",
 "hc", "hd",
 "ho", "hs",
 "hu", "hz",
 "i1", "i3",
 "ic", "IC",
 "if", "im",
 "in", "ip",
 "is", "it",
 "k0", "k1",
 "k2", "k3",
 "k4", "k5",
 "k6", "k7",
 "k8", "k9",
 "K1", "K2",
 "K3", "K4",
 "K5", "ka",
 "kA", "kb",
 "kC", "kd",
 "kD", "ke",
 "kE", "kF",
 "kh", "kH",
 "kI", "kl",
 "kL", "km",
 "kM", "kn",
 "kN", "ko",
 "kP", "kr",
 "kR", "ks",
 "kS", "kt",
 "kT", "ku",
 "l0", "l1",
 "l2", "l3",
 "l4", "l5",
 "l6", "l7",
 "l8", "l9",
 "le", "LE",
 "li", "ll",
 "lm", "LP",
 "mb", "md",
 "me", "mh",
 "mi", "mk",
 "mm", "mo",
 "mp", "mr",
 "ms", "nc",
 "nd", "NF",
 "nl", "ns",
 "nw", "os",
 "pb", "pc",
 "pf", "po",
 "pO", "ps",
 "rc", "RI",
 "rp", "rs",
 "sa", "sc",
 "se", "sf",
 "SF", "sg", 
 "so", "sr",
 "SR", "st", 
 "ta", "te",
 "ti", "ts",
 "uc", "ue",
 "ug", "ul",
 "up", "UP",
 "us", "vb",
 "ve", "vi",
 "vs", "wi",
 "ws", "xb",
 "xn", "xs",
 "xt"
};

const char *TermCap::m_capDefs[] = {
      "String to turn off alternate character set mode.",
      "String to insert a blank line before the cursor.",
      "String to insert N blank lines before the cursor.",
      "Flag: output to last column wraps cursor to next line.",
      "String to turn on alternate character set mode.like.",
      "Very obsolete alternative name for the `le' capability.",
      "String to sound the bell.",
      "Obsolete flag: ASCII backspace may be used for leftward motion.",
      "String to move the cursor left to the previous hardware tab stop column.",
      "Flag: `le' at left margin wraps to end of previous line.",
      "String to change terminal's command character.",
      "String to clear the line the cursor is on, and following lines.",
      "String to clear from the cursor to the end of the line.",
      "String to position the cursor at column C in the same line.",
      "String to clear the entire screen and put cursor at upper left corner.",
      "String to position the cursor at line L, column C.",
      "String to position the cursor at line L, column C, relative to display memory.",
      "Number: width of the screen.",
      "String to move cursor sideways to left margin.",
      "String to set the scroll region.",
      "Alternate form of string to set the scroll region.",
      "String to clear all tab stops.",
      "String to position the cursor at line L in the same column.",
      "Flag: data scrolled off top of screen may be scrolled back.",
      "Flag: data scrolled off bottom of screen may be scrolled back.",
      "Obsolete number: msec of padding needed for the backspace character.",
      "String to delete one character position at the cursor.",
      "Obsolete number: msec of padding needed for the carriage-return character.",
      "String to delete N characters starting at the cursor.",
      "Obsolete number: msec of padding needed for the formfeed character.",
      "String to delete the line the cursor is on.",
      "String to delete N lines starting with the cursor's line.",
      "String to enter delete mode.",
      "Obsolete number: msec of padding needed for the newline character.",
      "String to move the cursor vertically down one line.",
      "String to move cursor vertically down N lines.",
      "String to disable the display of the status line.",
      "Obsolete number: msec of padding needed for the tab character.",
      "String of commands to clear N characters at cursor.",
      "String to exit delete mode.",
      "String to leave insert mode.",
      "Flag: output of a space can erase an overstrike.",
      "Flag: other display commands work while writing the status line.",
      "String to advance to the next page, for a hardcopy terminal.",
      "String to move the cursor back from the status line to its previous position (outside the status line).",
      "Flag: this terminal type is generic, not real.",
      "Flag: hardcopy terminal.",
      "String to move the cursor down half a line.",
      "String to position cursor at upper left corner.",
      "Flag: the terminal has a status line.",
      "String to move the cursor up half a line.",
      "Flag: terminal cannot accept `~' as output.",
      "String to initialize the terminal for each login session.",
      "String to initialize the terminal for each login session.",
      "String to insert one character position at the cursor.",
      "String to insert N character positions at the cursor.",
      "String naming a file of commands to initialize the terminal.",
      "String to enter insert mode.",
      "Flag: outputting a space is different from moving over empty positions.",
      "String to output following an inserted character in insert mode.",
      "String to initialize the terminal for each login session.",
      "Number: initial spacing between hardware tab stop columns.",
      "String of input sent by function key 0 or 10.",
      "Strings of input sent by function keys 1 through 9.",
      "Strings of input sent by function keys 1 through 9.",
      "Strings of input sent by function keys 1 through 9.",
      "Strings of input sent by function keys 1 through 9.",
      "Strings of input sent by function keys 1 through 9.",
      "Strings of input sent by function keys 1 through 9.",
      "Strings of input sent by function keys 1 through 9.",
      "Strings of input sent by function keys 1 through 9.",
      "Strings of input sent by function keys 1 through 9.",
      "Strings sent by the five other keys in 3-by-3 array with arrows.",
      "Strings sent by the five other keys in 3-by-3 array with arrows.",
      "Strings sent by the five other keys in 3-by-3 array with arrows.",
      "Strings sent by the five other keys in 3-by-3 array with arrows.",
      "Strings sent by the five other keys in 3-by-3 array with arrows.",
      "String of input sent by the 'clear all tabs' key.",
      "String of input sent by the 'insert line' key.",
      "String of input sent by the 'backspace' key.",
      "String of input sent by the 'clear screen' key.",
      "String of input sent by typing the down-arrow key.",
      "String of input sent by the 'delete character' key.",
      "String to make the function keys work locally.",
      "String of input sent by the 'clear to end of line' key.",
      "String of input sent by the 'scroll forward' key.",
      "String of input sent by typing the 'home-position' key.",
      "String of input sent by the 'home down' key.",
      "String of input sent by the 'insert character' or 'enter insert mode' key.",
      "String of input sent by typing the left-arrow key.",
      "String of input sent by the 'delete line' key.",
      "Flag: the terminal has a Meta key.",
      "String of input sent by the 'exit insert mode' key.",
      "Numeric value, the number of numbered function keys.",
      "String of input sent by the 'next page' key.",
      "Very obsolete string listing the terminal's named function keys.",
      "String of input sent by the 'previous page' key.",
      "String of input sent by typing the right-arrow key.",
      "String of input sent by the 'scroll reverse' key.",
      "String to make the function keys transmit.",
      "String of input sent by the 'clear to end of screen' key.",
      "String of input sent by the 'clear tab stop this column' key.",
      "String of input sent by the 'set tab stop in this column' key.",
      "String of input sent by typing the up-arrow key.",
      "String on keyboard labelling function key 0 or 10.",
      "Strings on keyboard labelling function keys 1 through 9.",
      "Strings on keyboard labelling function keys 1 through 9.",
      "Strings on keyboard labelling function keys 1 through 9.",
      "Strings on keyboard labelling function keys 1 through 9.",
      "Strings on keyboard labelling function keys 1 through 9.",
      "Strings on keyboard labelling function keys 1 through 9.",
      "Strings on keyboard labelling function keys 1 through 9.",
      "Strings on keyboard labelling function keys 1 through 9.",
      "Strings on keyboard labelling function keys 1 through 9.",
      "String to move the cursor left one column.",
      "String to move cursor left N columns.",
      "Number: height of the screen.",
      "String to position cursor at lower left corner.",
      "Number: lines of display memory.",
      "Flag: writing to last column of last line will not scroll.",
      "String to enter blinking mode.",
      "String to enter double-bright mode.",
      "String to turn off all appearance modes",
      "String to enter half-bright mode.",
      "Flag: cursor motion in insert mode is safe.",
      "String to enter invisible mode.",
      "String to enable the functioning of the Meta key.",
      "String to disable the functioning of the Meta key.",
      "String to enter protected mode.",
      "String to enter reverse-video mode.",
      "Flag: cursor motion in standout mode is safe.",
      "Obsolete flag: do not use ASCII carriage-return on this terminal.",
      "String to move the cursor right one column.",
      "Flag: do not use XON/XOFF flow control.",
      "Obsolete alternative name for the `do' and `sf' capabilities.",
      "Flag: the terminal does not normally scroll for sequential output.",
      "String to move to start of next line, possibly clearing rest of old line.",
      "Flag: terminal can overstrike.",
      "Number: the lowest baud rate at which padding is actually needed.",
      "String containing character for padding.",
      "String to terminate redirection of output to the printer.",
      "String to redirect further output to the printer.",
      "String to redirect N characters ofoutput to the printer.",
      "String to print the screen on the attached printer.",
      "String to move to last saved cursor position.",
      "String to move cursor right N columns.",
      "String to output character C repeated N times.",
      "String to reset the terminal from any strange modes.",
      "String to turn on an arbitrary combination of appearance modes.",
      "String to save the current cursor position.",
      "String to leave standout mode.",
      "String to scroll the screen one line up.",
      "String to scroll the screen N lines up.",
      "Number: width of magic standout cookie.  Absent if magic cookies are not used.",
      "String to enter standout mode.",
      "String to scroll the screen one line down.",
      "String to scroll the screen N line down.",
      "String to set tab stop at current cursor column on all lines. programs.",
      "String to move the cursor right to the next hardware tab stop column.",
      "String to return terminal to settings for sequential output.",
      "String to initialize terminal for random cursor motion.",
      "String to move the terminal cursor into the status line.",
      "String to underline one character and move cursor right.",
      "String to turn off underline mode",
      "Number: width of underlining magic cookie.  Absent if underlining doesn't use magic cookies.",
      "Flag: underline by overstriking with an underscore.",
      "String to move the cursor vertically up one line.",
      "String to move cursor vertically up N lines.",
      "String to turn on underline mode",
      "String to make the screen flash.",
      "String to return the cursor to normal.",
      "String to make the cursor invisible.",
      "String to enhance the cursor.",
      "String to set the terminal output screen window.",
      "Number: the width of the status line.",
      "Flag: superbee terminal.",
      "Flag: cursor wraps in a strange way.",
      "Flag: clearing a line is the only way to clear the appearance modes of positions in that line (or, only way to remove magic cookies on that line).",
      "Flag: Teleray 1061; several strange characteristics."   /*`xt'*/
 
};

