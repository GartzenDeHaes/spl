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
#ifndef _httputility_h
#define _httputility_h

#include <spl/String.h>

namespace spl
{
/** 
 * @defgroup web Web
 * @{
 */

class HttpUtility
{
private:
	HttpUtility() {}

	inline static bool NotEncoded (char c)
	{
		return (c == '!' || c == '\'' || c == '(' || c == ')' || c == '*' || c == '-' || c == '.' || c == '_');
	}

public:

	static StringPtr HtmlAttributeEncode (const char *cp, int len);
	inline static StringPtr HtmlAttributeEncode (const String& s) { return HtmlAttributeEncode(s.GetChars(), s.Length()); }

	static bool HtmlAttributeEncodeRequired (const char *cp, int len);
	inline static bool HtmlAttributeEncodeRequired (const String& s) { return HtmlAttributeEncodeRequired(s.GetChars(), s.Length()); }

	static StringPtr UrlDecode (const char *cp, int len);
	inline static StringPtr UrlDecode (const String& s) { return UrlDecode(s.GetChars(), s.Length()); }

	static StringPtr UrlEncode (const char *cp, int len);
	inline static StringPtr UrlEncode (const String& s) { return UrlEncode(s.GetChars(), s.Length()); }

	static bool UrlEncodeRequired (const char *cp, int len);
	inline static bool UrlEncodeRequired (const String& s) { return UrlEncodeRequired(s.GetChars(), s.Length()); }

	static StringPtr HtmlDecode (const char *cp, int len);
	inline static StringPtr HtmlDecode (const String& s) { return HtmlDecode(s.GetChars(), s.Length()); }

	static StringPtr HtmlEncode (const char *cp, int len);
	inline static StringPtr HtmlEncode (const String& s) { return HtmlEncode(s.GetChars(), s.Length()); }

	static bool HtmlEncodeRequired (const char *cp, int len);
	inline static bool HtmlEncodeRequired (const String& s) { return HtmlEncodeRequired(s.GetChars(), s.Length()); }
};

/** @} */
}
#endif
