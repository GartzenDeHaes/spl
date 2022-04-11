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
#ifndef _arguments_h
#define _arguments_h

#include <spl/types.h>
#include <spl/Debug.h>
#include <spl/String.h>
#include <spl/collection/Hashtable.h>
#include <spl/Memory.h>
#include <spl/RefCountPtr.h>
#include <spl/collection/Vector.h>

namespace spl
{
/** 
 * @defgroup config Configuration
 * @{
 */

class CommandLine;
typedef RefCountPtr<CommandLine> CommandLinePtr;

/** @brief Command parses command line arguments in a variety of formats.
  *	Command format is either:
  *	<ul>
  *		<li>program /i /dos:style arg1 arg2</li>
  *		<li>program -i --uxix=style arg1 arg2</li>
  *	</ul>
  */
class CommandLine : public IMemoryValidate
{
protected:
	Vector<String> m_switches;
	Hashtable<String, String> m_switchIdx;
	Vector<String> m_args;

	static String ParseValue(const String& str, const bool unixStyle);

public:
	CommandLine();
	CommandLine(const CommandLine& cl);
	virtual ~CommandLine();

	CommandLine& operator =(const CommandLine& cl);
	 
	void Clear();

	inline bool HasSwitch(const String& argNoDashOrSlash) const { return m_switchIdx.ContainsKey(argNoDashOrSlash); }
	inline String& GetSwitch(const String argNoDashOrSlash) const { return m_switchIdx.GetRef(argNoDashOrSlash); }
	String& GetSwitch(const String& argNoDashOrSlash, const String& defValue);
	inline String& GetSwitch(const int idx) const { return m_switches.ElementAtRef(idx); }
	inline String& GetArg(int idx) const { ASSERT(idx < m_args.Count()); return m_args.ElementAtRef(idx); }
	inline int GetArgCount() const { return m_args.Count(); }
	inline int GetSwitchCount() const { return m_switchIdx.Count(); }
	void SetSwitch(const String& key, const String& value);

	static CommandLinePtr Parse(const char *args);
	static CommandLinePtr Parse(const int argc, const char **argv);
	static CommandLinePtr Parse(Vector<StringPtr>& args);
#ifdef DEBUG
	void ValidateMem() const;
	void CheckMem() const;
#endif
};

REGISTER_TYPEOF( 408, CommandLine );

/** @} */
}
#endif
