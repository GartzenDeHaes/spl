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
#include <spl/text/Regex.h>
#include <spl/collection/Vector.h>

using namespace spl;

void Regex::Compile()
{
	int er = regcomp(&m_regex, m_expr.GetChars(), m_flags);
	if (0 != er)
	{
		char buf[256];
		regerror(er, &m_regex, buf, 256);
		throw new InvalidArgumentException(buf);
	}
}

Regex::Regex(int maxMatches)
: m_expr(""), m_flags(RegexOptions::NONE), m_lastMatch(0), m_maxMatches(maxMatches)
{
	Compile();
}

Regex::Regex(const String& expr, RegexOptions::Options flags, int maxMatches)
: m_expr(expr), m_flags(flags), m_lastMatch(0), m_maxMatches(maxMatches)
{
	Compile();
}

Regex::Regex(const Regex& re)
: m_expr(re.m_expr), m_flags(re.m_flags), m_lastMatch(0), m_maxMatches(re.m_maxMatches)
{
	Compile();
}

Regex::~Regex()
{
	regfree(&m_regex);
}

Regex& Regex::operator =(const Regex& re)
{
	regfree(&m_regex);
	m_expr = re.m_expr;
	m_flags = re.m_flags;
	m_lastMatch = re.m_lastMatch;
	m_maxMatches = re.m_maxMatches;
	Compile();

	return *this;
}

bool Regex::IsMatch(const String& txt) const
{
	return 0 == regexec(&m_regex, txt.GetChars(), 0, NULL, 0);
}

RefCountPtr<Array<StringPtr> > Regex::Matches(const String& txt) const
{
	Vector<StringPtr> matches;
	regmatch_t *match = new regmatch_t[m_maxMatches];

	int ok = regexec(&m_regex, txt.GetChars(), m_maxMatches, match, 0);
	if (REG_NOMATCH == ok)
	{
		delete match;
		return matches.ToArray();
	}
	if (0 != ok)
	{
		delete match;
		char buf[256];
		regerror(ok, &m_regex, buf, 256);
		throw new InvalidArgumentException(buf);
	}

	for (int x = 0; x < m_maxMatches && match[x].rm_so >= 0; x++ )
	{
		matches.Add(StringPtr(new String(txt.GetChars(), match[x].rm_so, match[x].rm_eo - match[x].rm_so)));		
	}

	//int pos = 0;
	//do
	//{
	//	Debug::Assert(match.rm_so >= 0 && match.rm_eo >= 0);
	//	pos += match.rm_so;
	//	int len = match.rm_eo - match.rm_so;
	//	matches.Add(StringPtr(new String(txt.GetChars(), pos, len)));
	//	pos += len;
	//	memset(&match, 0, sizeof(regmatch_t));
	//}
	//while (0 == (ok = regexec(&m_regex, txt.GetChars() + pos, 1, &match, REG_NOTBOL)));

	delete match;
	return matches.ToArray();
}

StringPtr Regex::Match(const String& txt, int offset)
{
	regmatch_t match;
	int ok = regexec(&m_regex, txt.GetChars() + offset, 1, &match, 0);
	if (REG_NOMATCH == ok)
	{
		return StringPtr(new String());
	}
	if (0 != ok)
	{
		char buf[256];
		regerror(ok, &m_regex, buf, 256);
		throw new InvalidArgumentException(buf);
	}

	m_lastMatch = match.rm_so;
	return StringPtr(new String(txt.GetChars() + match.rm_so, match.rm_eo - match.rm_so));
}

#if defined(DEBUG)
void Regex::CheckMem() const
{
	m_expr.CheckMem();
}

void Regex::ValidateMem() const
{
	m_expr.ValidateMem();
}
#endif

