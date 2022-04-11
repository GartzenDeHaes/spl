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
#ifndef _commandparameter_h
#define _commandparameter_h

#include <spl/Exception.h>
#include <spl/collection/Hashtable.h>
#include <spl/Memory.h>
#include <spl/RefCountPtr.h>
#include <spl/String.h>
#include <spl/Variant.h>

namespace spl
{
/** 
 * @defgroup datamodel Data Access Classes
 * @ingroup database
 * @{
 */

class ParameterDirection
{
public:
	enum
	{
		PARAM_DIR_IN,
		PARAM_DIR_OUT,
		PARAM_DIR_INOUT,
		PARAM_DIR_RET
	};
};

class CommandParameter;
typedef RefCountPtr<CommandParameter> CommandParameterPtr;

class CommandParameter : public IMemoryValidate
{
protected:
	String m_name;
	int m_type;
	int m_dir;
	Variant m_data;
	bool m_isnumeric;
	int m_len;

public:
	CommandParameter(const String& name, int type, int dir, int len);
	CommandParameter(const String& name, int8 val);
	CommandParameter(const String& name, int16 val);
	CommandParameter(const String& name, int32 val);
	CommandParameter(const String& name, int64 val);
	CommandParameter(const String& name, float32 val);
	CommandParameter(const String& name, float64 val);
	CommandParameter(const String& name, bool val);
	CommandParameter(const String& name, DateTime val);
	CommandParameter(const String& name, char *val);
	CommandParameter(const CommandParameter& prm);
	virtual ~CommandParameter();

	CommandParameter& operator =(const CommandParameter& prm);

	byte GetByte();
	int16 GetInt16();
	int32 GetInt32();
	int64 GetInt64();
	Decimal GetDecimal();
	float32 GetFloat32();
	float64 GetFloat64();
	bool GetBit();
	DateTime GetTimeStamp();
	Date GetDate();
	DateTime GetDateTime();
	StringPtr GetChar();
	StringPtr GetVarchar();
	Variant &GetVariant();

	void Set(int8 val);
	void Set(int16 val);
	void Set(int32 val);
	void Set(int64 val);
	void Set(Decimal val);
	void Set(float32 val);
	void Set(float64 val);
	void Set(bool val);
	void Set(DateTime& val);
	void Set(Date& val);
	void Set(const String& val);

	inline int Type()
	{
		return m_type;
	}

	inline int Direction()
	{
		return m_dir;
	}

	inline int Length()
	{
		return m_len;
	}

	inline bool IsNumeric()
	{
		return m_isnumeric;
	}

	inline String& Name()
	{
		return m_name;
	}

	inline StringPtr ToString()
	{
		return m_data.ToString();
	}

#if defined(DEBUG)
	virtual void CheckMem() const;
	virtual void ValidateMem() const;
#endif
};

inline void TypeValidate( CommandParameter *prm )
{
	if ( NULL != prm )
	{
		ASSERT_MEM( prm, sizeof(CommandParameter) );
		prm->ValidateMem();
	}
}

inline void TypeCheckMem( CommandParameter *prm )
{
	if ( NULL != prm )
	{
		DEBUG_NOTE_MEM( prm );
		prm->CheckMem();
	}
}

/** @} */
}
#endif
