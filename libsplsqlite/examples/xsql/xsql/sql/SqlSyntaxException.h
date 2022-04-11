#ifndef _sqlsyntaxexception_h
#define _sqlsyntaxexception_h

#include <spl/Exception.h>
#include <spl/String.h>

class SqlSyntaxException : public spl::Exception
{
public:
		inline SqlSyntaxException(spl::String expected, spl::String lexum, int lineNum)
		: Exception(spl::String::Format("Expected '%s' found '%s' on line %d", expected.GetChars(), lexum.GetChars(), lineNum)->GetChars())
		{
		}

		virtual ~SqlSyntaxException();
};

#endif
