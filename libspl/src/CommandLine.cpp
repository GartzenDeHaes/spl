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
#include <spl/configuration/CommandLine.h>
#include <spl/Exception.h>

using namespace spl;

CommandLine::CommandLine()
: m_switchIdx(), m_args(), m_switches()
{
}

CommandLine::CommandLine(const CommandLine& cl)
: m_switchIdx(), m_args(), m_switches()
{
	*this = cl;
}

CommandLine::~CommandLine()
{
	Clear();
}

CommandLine& CommandLine::operator =(const CommandLine& cl)
{
	Clear();

	m_switchIdx = cl.m_switchIdx;
	m_args = cl.m_args;
	m_switches = cl.m_switches;

	return *this;
}

void CommandLine::Clear()
{
	m_args.Clear();
	m_switchIdx.Clear();
	m_switches.Clear();
}

void CommandLine::SetSwitch(const String& key, const String& value)
{
	m_switchIdx.Set(key, value);
	m_switches.Add(key);
}

static inline bool _IsUnixSwitch(const String& str)
{
	return '-' == str[0];
}

static inline bool _IsWinSwitch(const String& str)
{
	return '/' == str[0];
}

static inline bool _SwitchHasValue(const String& str, const bool unixStyle)
{
	if (unixStyle)
	{
		return str.IndexOf('=') > -1;
	}
	return str.IndexOf(':') > -1;
}

static inline bool _IsSwitch(const String& str, const bool unixStyle)
{
	if (unixStyle)
	{
		return _IsUnixSwitch(str);
	}
	return _IsWinSwitch(str);
}

static void _ParseSwitch(const String& str, const bool unixStyle, char *buf)
{
	int bufpos = 0;
	int pos = 1;
	if (unixStyle && '-' == str[1])
	{
		// --name style
		pos = 2;
	}
	while (pos < 254 && '\0' != str[pos])
	{
		if (unixStyle && '=' == str[pos])
		{
			break;
		}
		else if (':' == str[pos])
		{
			break;
		}
		buf[bufpos++] = str[pos++];
	}
	buf[bufpos] = '\0';
}

CommandLinePtr CommandLine::Parse(const char *args)
{
	StringPtr s(String::Intern(args));
	Vector<StringPtr> parts(s->Split(" "));
	
	return Parse(parts);
}

CommandLinePtr CommandLine::Parse(Vector<StringPtr>& args)
{
	bool styleSet = false;
	bool unixStyle = true;
	int x;
	char buf[255];

	CommandLinePtr cl(new CommandLine());
	
	for (x = 0; x < args.Count(); x++)
	{
		if (!styleSet && _IsUnixSwitch(args[x]))
		{
			styleSet = true;
			unixStyle = true;
		}
		if (!styleSet && _IsWinSwitch(args[x]))
		{
			if (0 != x)
			{
				// on unix, the full path name (including leading '/') is passed
				// as arg 0.
				styleSet = true;
				unixStyle = false;
			}
		}
		if (styleSet)
		{
			// might be -X or -X val or -X=val or /X:val or just a value
			if (! _IsSwitch(args[x], unixStyle))
			{
				// just a value
				cl->m_args.Add(args[x]);
			}
			else
			{
				_ParseSwitch(args[x], unixStyle, buf);
				cl->m_switches.Add(buf);
				if (_SwitchHasValue(args[x], unixStyle))
				{
					cl->m_switchIdx.Set(buf, ParseValue(args[x], unixStyle));
				}
				else
				{
					cl->m_switchIdx.Set(buf, String());
				}
			}
		}
		else
		{
			// an arg
			cl->m_args.Add(args[x]);
		}
	}
	
	cl.ValidateMem();
	
	return cl;
}

/**
  * Parse can handle unix style -X, --name, and --name=val
  * as well as windows /X:val.  The style cannot be mixed,
  * since "-X:/dir" wouldn't work as expected.  Quoted values are 
  * supported.
  *
  * NOTE: argv[0] (the program name) is also added.
  */
CommandLinePtr CommandLine::Parse(const int argc, const char **argv)
{
	 Vector<StringPtr> cla;
	 
	 for (int x = 0; x < argc; x++)
	 {
		  cla.Add(StringPtr(new String(argv[x])));
	 }
	 
	 return Parse(cla);
}

String CommandLine::ParseValue(const String& str, const bool unixStyle)
{
	int pos;
	if ( unixStyle )
	{
		ASSERT('-' == str[0]);
		pos = str.IndexOf('=');
	}
	else
	{
		ASSERT('/' == str[0]);
		pos = str.IndexOf(':');
	}
	ASSERT( pos > 1 );
	
	return *str.Substring(pos+1);
}

String& CommandLine::GetSwitch(const String& argNoDashOrSlash, const String& defValue)
{
	String& ret = GetSwitch(argNoDashOrSlash);
	if ( ret.Length() != 0 )
	{
		return ret;
	}
	SetSwitch(argNoDashOrSlash, defValue);
	return GetSwitch(argNoDashOrSlash);
}

#ifdef DEBUG
void CommandLine::ValidateMem() const
{
	m_args.ValidateMem();
	m_switchIdx.ValidateMem();
	m_switches.ValidateMem();
}

void CommandLine::CheckMem() const
{
	m_args.CheckMem();
	m_switchIdx.CheckMem();
	m_switches.CheckMem();
}
#endif
