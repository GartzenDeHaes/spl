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
#ifndef _exception_h
#define _exception_h

#include <spl/Debug.h>

namespace spl
{
/** 
 * @defgroup debugging Debugging
 * @ingroup core
 * @{
 */

/** @brief Generic exception, all exceptions should sub class this.
 *
 */
class Exception
{
protected:
	char m_msg[512];

public:
	Exception( );
	Exception( const char *msg );

	virtual ~Exception();

	inline const char *Message() const
	{
		return m_msg;
	}
};

class OutOfMemoryException : public Exception
{
public:
	OutOfMemoryException();
};

class SqlException : public Exception
{
public:
	SqlException(const char *msg);
};

class IndexOutOfBoundsException : public Exception
{
public:
	IndexOutOfBoundsException();
	inline IndexOutOfBoundsException(const char *msg)
	: Exception(msg)
	{
	}
};

class InvalidArgumentException : public Exception
{
public:
	InvalidArgumentException(const char *msg);
};

class IOException : public Exception
{
public:
	IOException(const char *msg);
};

class AssertionFailedException : public Exception
{
public:
	AssertionFailedException(const char *msg);
};

class NotImplementedException : public Exception
{
public:
	NotImplementedException(const char *msg);
	NotImplementedException();
};

class IErrorListener
{
public:
	virtual void OnError(const char *msg) = 0;
};

class SocketException : public Exception
{
public:
	SocketException( const char *msg );
};

class ThreadStartException : public Exception
{
public:
	ThreadStartException();
};

class FileNotFoundException : public Exception
{
public:
	FileNotFoundException(const char *filename);
};

class PacketUnderflowException : public Exception
{
public:
	PacketUnderflowException();
};

class PacketReadTypeMismatchException : public Exception
{
public:
	PacketReadTypeMismatchException(const char *msg);
};

class PacketNotReadyException : public Exception
{
public:
	PacketNotReadyException(const char *msg);
};

class InvalidTypeConversionException : public Exception
{
public:
	InvalidTypeConversionException();
	InvalidTypeConversionException(const char *msg);
};

class StateException : public Exception
{
public:
	StateException(const char *msg);
};

class SecurityException : public Exception
{
public:
	SecurityException(const char *msg);
};

class ConfigurationException : public Exception
{
public:
	ConfigurationException(const char *msg);
};

class XmlException : public Exception
{
protected:
	int m_row;
	int m_col;
	
public:
	XmlException(const char *msg, int row, int col);

	inline int Row() const { return m_row; }
	inline int Col() const { return m_col; }
};

class ProtocolException : public Exception
{
public:
	inline ProtocolException(const char *msg)
	: Exception(msg)
	{
	}
};

class SyntaxException : public Exception
{
public:
	inline SyntaxException(const char *msg)
	: Exception(msg)
	{
	}
};

class NotSupportedException : public Exception
{
public:
	inline NotSupportedException(const char *msg)
	: Exception(msg)
	{
	}
};

class ParseException : public Exception
{
public:
	inline ParseException(const char *msg)
	: Exception(msg)
	{
	}
};

/** @} */
}
#endif
