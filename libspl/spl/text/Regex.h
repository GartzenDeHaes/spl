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
#ifndef _regex_h
#define _regex_h

#include <spl/types.h>
#include <spl/Debug.h>

#include <spl/text/pcreposix.h>
#include <spl/Memory.h>
#include <spl/RefCountPtr.h>
#include <spl/text/RegexOptions.h>
#include <spl/String.h>
#include <spl/WeakReference.h>

namespace spl
{
/** 
 * @defgroup text Text
 * @{
 */

class Regex;
typedef RefCountPtr<Regex> RegexPtr;
typedef WeakReference<Regex, RegexPtr> RegexRef;

REGISTER_TYPEOF( 480, RegexPtr );
REGISTER_TYPEOF( 482, RegexRef );

/** @brief Regular expression using PCRE.
 *	Expressions are compiled when the class is constructed.
 */
class Regex : public IMemoryValidate
{
private:
	String m_expr;
	RegexOptions::Options m_flags;
	regex_t m_regex;
	int m_lastMatch;
	int m_maxMatches;

	void Compile();

public:
	Regex(int maxMatches = 20);
	Regex(const String& expr, RegexOptions::Options flags = RegexOptions::NONE, int maxMatches = 20);
	Regex(const Regex& re);
	virtual ~Regex();

	Regex& operator =(const Regex& re);

	bool IsMatch(const String& txt) const;
	RefCountPtr<Array<StringPtr> > Matches(const String& txt) const;
	StringPtr Match(const String& txt, int offset = 0);
	
	inline const String Expression() const { return m_expr; }
	inline int LastMatch() const { return m_lastMatch; }
	
	inline bool IsGlobalSet() const { return true;/*0 != (m_flags ^ RegexOptions::EXPLICIT_CAPTURE);*/ }
	inline bool IsIgnoreCaseSet() const { return 0 != (m_flags & RegexOptions::IGNORE_CASE); }
	inline bool IsMultilineSet() const { return 0 == (m_flags & RegexOptions::MULTILINE); }

#if defined(DEBUG)
	virtual void CheckMem() const;
	virtual void ValidateMem() const;
#endif
};

REGISTER_TYPEOF( 484, Regex );

/** @} */
}
#endif
