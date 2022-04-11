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
#include <stdio.h>
#include <spl/Memory.h>
#include <spl/Exception.h>
#include <spl/collection/CircularArray.h>

using namespace spl;

Exception::Exception( )
{
	m_msg[0] = '\0';
}

Exception::Exception( const char *msg )
{
	char *to = &m_msg[0];
	while ( '\0' != *msg )
	{
		*to++ = *msg++;
	}
	*to = '\0';
}

Exception::~Exception()
{
}

OutOfMemoryException::OutOfMemoryException() : Exception("Out of memory")
{
}

SqlException::SqlException(const char *msg) : Exception(msg)
{
}

IndexOutOfBoundsException::IndexOutOfBoundsException() : Exception("Index out of bounds")
{
}

InvalidArgumentException::InvalidArgumentException(const char *msg) : Exception(msg)
{
}

IOException::IOException(const char *msg) : Exception(msg)
{
}

AssertionFailedException::AssertionFailedException(const char *msg) : Exception(msg) 
{
}

CircularArrayException::CircularArrayException(const char *msg) : Exception(msg)
{
}

NotImplementedException::NotImplementedException(const char *msg) : Exception(msg)
{
}

NotImplementedException::NotImplementedException() : Exception()
{
}

SocketException::SocketException( const char *msg ) : Exception( msg )
{
}

ThreadStartException::ThreadStartException() : Exception()
{
}

FileNotFoundException::FileNotFoundException(const char *filename) : Exception(filename)
{
}

PacketUnderflowException::PacketUnderflowException() : Exception() 
{
}

PacketReadTypeMismatchException::PacketReadTypeMismatchException(const char *msg) : Exception(msg) 
{
}

PacketNotReadyException::PacketNotReadyException(const char *msg) : Exception(msg) 
{
}

InvalidTypeConversionException::InvalidTypeConversionException() : Exception()
{
}

InvalidTypeConversionException::InvalidTypeConversionException(const char *msg) : Exception(msg)
{
}

StateException::StateException(const char *msg) : Exception(msg) 
{
}

SecurityException::SecurityException(const char *msg) : Exception(msg) 
{
}

ConfigurationException::ConfigurationException(const char *msg) : Exception(msg)
{
}

XmlException::XmlException(const char *msg, int row, int col) 
: Exception(msg), m_row(row+1), m_col(col+1)
{
}
