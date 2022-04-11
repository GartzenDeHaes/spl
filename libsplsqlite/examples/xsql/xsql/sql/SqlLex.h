#ifndef _sqllex_h
#define _sqllex_h

#include <spl/Debug.h>
#include <spl/text/StringBuffer.h>

#include "xsql/sql/ISqlLex.h"

class SqlLex : public ISqlLex
{
private:
	enum SqlToken m_token;
	spl::String m_lexum;
	int m_lineNum;
	int m_pos;
	spl::String m_text;
	spl::StringBuffer m_buf;

	void Read();

public:
	SqlLex(spl::String sql);
	virtual ~SqlLex();

	inline spl::String& Lexum() { return m_lexum; }
	inline enum SqlToken Token() const { return m_token; }
	inline int LineNumber() const { return m_lineNum; }

	bool Next();
	bool Match(const char* cp);
	bool Match(const enum SqlToken tok);

	inline bool IsEOF()
	{
		return m_token == SQL_EOF || m_pos >= m_text.Length();
	}

#ifdef DEBUG
	void ValidateMem() const;
	void CheckMem() const;
#endif
};

#endif
