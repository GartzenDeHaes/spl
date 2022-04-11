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
#ifndef _regexoptions_h
#define _regexoptions_h

#include <spl/text/pcreposix.h>

namespace spl
{
/** 
 * @defgroup text Text
 * @{
 */

/** @brief Options that can OR'ed together in the Regex constructor.
 *	@ref Regex
 */
class RegexOptions
{
public:
	typedef enum
	{
		NONE = 0,
		IGNORE_CASE = REG_ICASE,		//< Ignore case
		MULTILINE = REG_NEWLINE,		//< Anchor ^ and $ to lines instead of the entire
		//EXPLICIT_CAPTURE = REG_DOTALL,	//< Report only success/fail in regexec()
		SINGLE_LINE = REG_DOTALL,		//< If this modifier is set, a dot metacharacter in the pattern matches all characters, including newlines. Without it, newlines are excluded.
		NOT_BOL = REG_NOTBOL,			//< Start of string is not begining of line (BOL)
		NOT_EOL = REG_NOTEOL,			//< End of string is not the end of a line (EOL)
		NOT_EMPTY = REG_NOTEMPTY		//< Don't know what this does
	} Options;
};

/** @} */
}
#endif
