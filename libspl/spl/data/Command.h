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
#ifndef _command_h
#define _command_h

#include <spl/types.h>
#include <spl/Debug.h>

#include <spl/data/CommandParameter.h>
#include <spl/Exception.h>
#include <spl/collection/Hashtable.h>
#include <spl/Memory.h>
#include <spl/data/RecordSet.h>
#include <spl/RefCountPtr.h>
#include <spl/String.h>
#include <spl/Variant.h>
#include <spl/collection/Vector.h>

namespace spl
{
/** 
 * @defgroup datamodel Data Access Classes
 * @ingroup database
 * @{
 */

class Command;
typedef RefCountPtr<Command> CommandPtr;

/** @brief Abstract base class for database commands.
 *	@ref MySqlCommand
 *	@ref SqlLiteCommand
 */
class Command : public IMemoryValidate
{
protected:
	String m_cmdtxt;
	Hashtable<String, CommandParameterPtr> m_prmIdx;
	Vector<CommandParameterPtr> m_prm;

public:
	Command();
	Command(const Command& cmd);
	Command(const String& cmdtxt);
	virtual ~Command();

	Command& operator =(const Command& cmd);

	virtual void Clear();

	inline String CommandTextGet() { return m_cmdtxt; };
	virtual void CommandTextSet(const String& txt);

	virtual CommandParameterPtr CreateParameter(const String& name, int type, int direction, int len);
	virtual CommandParameterPtr CreateParameter(const String& name, int type, int direction);
	virtual CommandParameterPtr CreateParameter(const String& name, const String& value);
	virtual CommandParameterPtr CreateParameter(const String& name, int32 value);
	virtual CommandParameterPtr CreateParameter(const String& name, int8 value);
	virtual CommandParameterPtr CreateParameter(const String& name, float32 value);
	virtual CommandParameterPtr CreateParameter(const String& name, float64 value);

	virtual CommandParameterPtr GetParameter(const String& name) const;
	inline CommandParameterPtr GetParameter(int idx) const
	{ 
		return  m_prm.ElementAt(idx); 
	}

	inline int ParameterCount() const 
	{ 
		return m_prm.Count(); 
	}

	virtual void Prepare();
	virtual int ExecuteNonQuery();
	virtual RecordSetPtr ExecuteQuery();

#if defined(DEBUG)
	virtual void CheckMem() const;
	virtual void ValidateMem() const;
#endif
};

/** @} */
}
#endif
