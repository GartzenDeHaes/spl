#include <spl/Numeric.h>
#include "xsql/sql/SqlLex.h"
#include "xsql/sql/SqlSyntaxException.h"
#include <ctype.h>

using namespace spl;

static String TokenLexum(enum SqlToken tok);

SqlLex::SqlLex(String sql)
: m_token(SQL_BOF), m_lexum(), m_lineNum(1), m_pos(0), m_text(sql), m_buf()
{
}

SqlLex::~SqlLex()
{
}

void SqlLex::Read()
{
	char ch = m_text.CharAt(m_pos);
	m_buf.Clear();

	while (! IsEOF() && !(isalnum(ch) || ispunct(ch)))
	{
		if (ch == '\n')
		{
			m_lineNum++;
		}
		ch = m_text.CharAt(++m_pos);
	}

	if (IsEOF())
	{
		return;
	}

	if (ch == '\'' || ch == '"')
	{
		char exitch = ch;

		ch = m_text.CharAt(++m_pos);
		while (ch != exitch)
		{
			m_buf.Append(ch);
			ch = m_text.CharAt(++m_pos);
		}
		++m_pos;

		m_token = SQL_STRLIT;
	}
	else if (isalnum(ch) || ch == '_')
	{
		while (! IsEOF() && (isalnum(ch) || ch == '_'))
		{
			m_buf.Append(ch);
			ch = m_text.CharAt(++m_pos);
		}
	}
	else
	{
		while (! IsEOF() && ispunct(ch))
		{
			m_buf.Append(ch);
			ch = m_text.CharAt(++m_pos);
			if (ch == ')')
			{
				break;
			}
		}
	}
}

bool SqlLex::Match(const char* cp)
{
	if (m_lexum.EqualsIgnoreCase(cp))
	{
		return Next();
	}

	throw new SqlSyntaxException(cp, Lexum(), m_lineNum);
}

bool SqlLex::Match(const enum SqlToken tok)
{
	if (m_token == tok)
	{
		return Next();
	}

	throw new SqlSyntaxException(TokenLexum(tok), Lexum(), m_lineNum);
}

bool SqlLex::Next()
{
	if (m_token == SQL_EOF)
	{
		return false;
	}

	if (m_pos >= m_text.Length())
	{
		m_token = SQL_EOF;
		return true;
	}

	m_token = SQL_UNKNOWN;

	Read();

	if (m_buf.Length() == 0)
	{
		return false;
	}

	m_lexum = m_buf.GetChars();

	if (m_token == SQL_STRLIT)
	{
		return true;
	}

	if (m_lexum.EqualsIgnoreCase("("))
	{
		m_token = SQL_LPAR;
	}
	else if (m_lexum.EqualsIgnoreCase(")"))
	{
		m_token = SQL_RPAR;
	}
	else if (m_lexum.EqualsIgnoreCase(">"))
	{
		m_token = SQL_GT;
	}
	else if (m_lexum.EqualsIgnoreCase(">="))
	{
		m_token = SQL_GTEQ;
	}
	else if (m_lexum.EqualsIgnoreCase("<"))
	{
		m_token = SQL_LT;
	}
	else if (m_lexum.EqualsIgnoreCase("<="))
	{
		m_token = SQL_LTEQ;
	}
	else if (m_lexum.EqualsIgnoreCase("<>"))
	{
		m_token = SQL_NEQ;
	}
	else if (m_lexum.EqualsIgnoreCase("*"))
	{
		m_token = SQL_SPLAT;
	}
	else if (m_lexum.EqualsIgnoreCase(":"))
	{
		m_token = SQL_COLON;
	}
	else if (m_lexum.EqualsIgnoreCase("?"))
	{
		/* has the same effect as colon in this program */
		m_token = SQL_COLON;
	}
	else if (m_lexum.EqualsIgnoreCase(";"))
	{
		m_token = SQL_SEMI;
	}
	else if (m_lexum.EqualsIgnoreCase("="))
	{
		m_token = SQL_EQ;
	}
	else if (m_lexum.EqualsIgnoreCase("."))
	{
		m_token = SQL_DOT;
	}
	else if (m_lexum.EqualsIgnoreCase(","))
	{
		m_token = SQL_COMMA;
	}
	else if (m_lexum.EqualsIgnoreCase("CREATE"))
	{
		m_token = SQL_CREATE;
	}
	else if (m_lexum.EqualsIgnoreCase("PROCEDURE"))
	{
		m_token = SQL_PROC;
	}
	else if (m_lexum.EqualsIgnoreCase("SELECT"))
	{
		m_token = SQL_SELECT;
	}
	else if (m_lexum.EqualsIgnoreCase("FROM"))
	{
		m_token = SQL_FROM;
	}
	else if (m_lexum.EqualsIgnoreCase("AS"))
	{
		m_token = SQL_AS;
	}
	else if (m_lexum.EqualsIgnoreCase("INT"))
	{
		m_token = SQL_INT;
	}
	else if (m_lexum.EqualsIgnoreCase("INTEGER"))
	{
		m_token = SQL_INT;
	}
	else if (m_lexum.EqualsIgnoreCase("SMALLINT"))
	{
		m_token = SQL_SMALLINT;
	}
	else if (m_lexum.EqualsIgnoreCase("BIGINT"))
	{
		m_token = SQL_BIGINT;
	}
	else if (m_lexum.EqualsIgnoreCase("CHAR"))
	{
		m_token = SQL_CHAR;
	}
	else if (m_lexum.EqualsIgnoreCase("WHERE"))
	{
		m_token = SQL_WHERE;
	}
	else if (m_lexum.EqualsIgnoreCase("AND"))
	{
		m_token = SQL_AND;
	}
	else if (m_lexum.EqualsIgnoreCase("OR"))
	{
		m_token = SQL_OR;
	}
	else if (m_lexum.EqualsIgnoreCase("INNER"))
	{
		m_token = SQL_INNER;
	}
	else if (m_lexum.EqualsIgnoreCase("JOIN"))
	{
		m_token = SQL_JOIN;
	}
	else if (m_lexum.EqualsIgnoreCase("LEFT"))
	{
		m_token = SQL_LEFT;
	}
	else if (m_lexum.EqualsIgnoreCase("RIGHT"))
	{
		m_token = SQL_RIGHT;
	}
	else if (m_lexum.EqualsIgnoreCase("ON"))
	{
		m_token = SQL_ON;
	}
	else if (m_lexum.EqualsIgnoreCase("INSERT"))
	{
		m_token = SQL_INSERT;
	}
	else if (m_lexum.EqualsIgnoreCase("INTO"))
	{
		m_token = SQL_INTO;
	}
	else if (m_lexum.EqualsIgnoreCase("VALUES"))
	{
		m_token = SQL_VALUES;
	}
	else if (m_lexum.EqualsIgnoreCase("UPDATE"))
	{
		m_token = SQL_UPDATE;
	}
	else if (m_lexum.EqualsIgnoreCase("SET"))
	{
		m_token = SQL_SET;
	}
	else if (m_lexum.EqualsIgnoreCase("ORDER"))
	{
		m_token = SQL_ORDER;
	}
	else if (m_lexum.EqualsIgnoreCase("BY"))
	{
		m_token = SQL_BY;
	}
	else if (m_lexum.EqualsIgnoreCase("DESC"))
	{
		m_token = SQL_DESC;
	}
	else if (m_lexum.EqualsIgnoreCase("ASC"))
	{
		m_token = SQL_ASC;
	}
	else if (m_lexum.EqualsIgnoreCase("IN"))
	{
		m_token = SQL_IN;
	}
	else if (m_lexum.EqualsIgnoreCase("DELETE"))
	{
		m_token = SQL_DELETE;
	}
	else if (m_lexum.EqualsIgnoreCase("GROUP"))
	{
		m_token = SQL_GROUP;
	}
	else if (m_lexum.EqualsIgnoreCase("HAVING"))
	{
		m_token = SQL_HAVING;
	}
	else if (m_lexum.EqualsIgnoreCase("LIKE"))
	{
		m_token = SQL_LIKE;
	}
	else if (m_lexum.EqualsIgnoreCase("IS"))
	{
		m_token = SQL_IS;
	}
	else if (m_lexum.EqualsIgnoreCase("NULL"))
	{
		m_token = SQL_NULL;
	}
	else if (m_lexum.EqualsIgnoreCase("NOT"))
	{
		m_token = SQL_NOT;
	}
	else if (m_lexum.EqualsIgnoreCase("UNION"))
	{
		m_token = SQL_UNION;
	}
	else if (m_lexum.EqualsIgnoreCase("CASE"))
	{
		m_token = SQL_CASE;
	}
	else if (m_lexum.EqualsIgnoreCase("WHEN"))
	{
		m_token = SQL_WHEN;
	}
	else if (m_lexum.EqualsIgnoreCase("THEN"))
	{
		m_token = SQL_THEN;
	}
	else if (m_lexum.EqualsIgnoreCase("ELSE"))
	{
		m_token = SQL_ELSE;
	}
	else if (m_lexum.EqualsIgnoreCase("END"))
	{
		m_token = SQL_END;
	}
	else if (m_lexum.EqualsIgnoreCase("DISTINCT"))
	{
		m_token = SQL_DISTINCT;
	}
	else if (m_lexum.EqualsIgnoreCase("BEGIN"))
	{
		m_token = SQL_BEGIN;
	}
	else if (m_lexum.EqualsIgnoreCase("TRANSACTION"))
	{
		m_token = SQL_TRANS;
	}
	else if (m_lexum.EqualsIgnoreCase("COMMIT"))
	{
		m_token = SQL_COMMIT;
	}
	else if (m_lexum.EqualsIgnoreCase("ABORT"))
	{
		m_token = SQL_ABORT;
	}
	else if (m_lexum.EqualsIgnoreCase("INVOKE"))
	{
		m_token = SQL_INVOKE;
	}
	else if (m_lexum.EqualsIgnoreCase("ALTER"))
	{
		m_token = SQL_ALTER;
	}
	else if (m_lexum.EqualsIgnoreCase("DECLARE"))
	{
		m_token = SQL_DECLARE;
	}
	else if (m_lexum.EqualsIgnoreCase("SECTION"))
	{
		m_token = SQL_SECTION;
	}
	else if (m_lexum.EqualsIgnoreCase("INCLUDE"))
	{
		m_token = SQL_INCLUDE;
	}
	else if (m_lexum.EqualsIgnoreCase("CURSOR"))
	{
		m_token = SQL_CURSOR;
	}
	else if (m_lexum.EqualsIgnoreCase("FOR"))
	{
		m_token = SQL_FOR;
	}
	else if (m_lexum.EqualsIgnoreCase("OPEN"))
	{
		m_token = SQL_OPEN;
	}
	else if (m_lexum.EqualsIgnoreCase("FETCH"))
	{
		m_token = SQL_FETCH;
	}
	else if (m_lexum.EqualsIgnoreCase("BETWEEN"))
	{
		m_token = SQL_BETWEEN;
	}
	else if (m_lexum.EqualsIgnoreCase("CLOSE"))
	{
		m_token = SQL_CLOSE;
	}
	else if (m_lexum.EqualsIgnoreCase("DATETIME"))
	{
		m_token = SQL_DATETIME;
	}
	else if (m_lexum.EqualsIgnoreCase("FREE"))
	{
		m_token = SQL_FREE;
	}
	else if (m_lexum.EqualsIgnoreCase("ROLLBACK"))
	{
		m_token = SQL_ROLLBACK;
	}
	else if (m_lexum.EqualsIgnoreCase("CONTROL"))
	{
		m_token = SQL_CONTROL;
	}
	else if (m_lexum.EqualsIgnoreCase("CAST"))
	{
		m_token = SQL_CAST;
	}
	else if (m_lexum.EqualsIgnoreCase("EXISTS"))
	{
		m_token = SQL_EXISTS;
	}
	else if (m_lexum.EqualsIgnoreCase("BROWSE"))
	{
		m_token = SQL_BROWSE;
	}
	else if (m_lexum.EqualsIgnoreCase("STABLE"))
	{
		m_token = SQL_STABLE;
	}
	else if (m_lexum.EqualsIgnoreCase("INTERVAL"))
	{
		m_token = SQL_INTERVAL;
	}
	else if (m_lexum.EqualsIgnoreCase("SUBSTRING"))
	{
		m_token = SQL_SUBSTRING;
	}
	else if (m_lexum.EqualsIgnoreCase("DATEFORMAT"))
	{
		m_token = SQL_DATEFORMAT;
	}
	else if (m_lexum.EqualsIgnoreCase("POSITION"))
	{
		m_token = SQL_POSITION;
	}
	else if (m_lexum.EqualsIgnoreCase("TRIM"))
	{
		m_token = SQL_TRIM;
	}
	else if (m_lexum.EqualsIgnoreCase("EXEC"))
	{
		m_token = SQL_EXEC;
	}
	else if (m_lexum.EqualsIgnoreCase( "EXECUTE"))
	{
		m_token = SQL_EXEC;
	}
	else if (Numeric::IsNumeric(m_lexum))
	{
		m_token = SQL_NUM;
	}
	else
	{
		m_token = SQL_ID;
	}

	return true;
}

#ifdef DEBUG
void SqlLex::ValidateMem() const
{
	m_lexum.ValidateMem();
	m_text.ValidateMem();
	m_buf.ValidateMem();
}

void SqlLex::CheckMem() const
{
	m_lexum.CheckMem();
	m_text.CheckMem();
	m_buf.CheckMem();
}
#endif

static String TokenLexum(enum SqlToken tok)
{
	switch(tok)
	{
	case SQL_BOF:
		return "BOF";
	case SQL_UNKNOWN:
		return "<<INTERNAL ERROR>>";
	case SQL_CREATE:
		return "CREATE";
	case SQL_PROC:
		return "PROC";
	case SQL_AS:
		return "AS";
	case SQL_VAR:
		return "<<VAR>>";
	case SQL_ID:
		return "<<ID>>";
	case SQL_DOT:
		return ".";
	case SQL_SELECT:
		return "SELECT";
	case SQL_FROM:
		return "FROM";
	case SQL_SPLAT:
		return "*";
	case SQL_LPAR:
		return "(";
	case SQL_RPAR:
		return ")";
	case SQL_STRLIT:
		return "<<STRING>>";
	case SQL_EQ:
		return "=";
	case SQL_SEMI:
		return ";";
	case SQL_COMMA:
		return ",";
	case SQL_INT:
		return "INT";
	case SQL_SMALLINT:
		return "SMALLINT";
	case SQL_BIGINT:
		return "BIGINT";
	case SQL_CHAR:
		return "CHAR";
	case SQL_NUM:
		return "<<NUMBER>>";
	case SQL_WHERE:
		return "WHERE";
	case SQL_AND:
		return "AND";
	case SQL_INNER:
		return "INNER";
	case SQL_JOIN:
		return "JOIN";
	case SQL_RIGHT:
		return "RIGHT";
	case SQL_LEFT:
		return "LEFT";
	case SQL_ON:
		return "ON";
	case SQL_OR:
		return "OR";
	case SQL_INSERT:
		return "INSERT";
	case SQL_INTO:
		return "INTO";
	case SQL_VALUES:
		return "VALUES";
	case SQL_UPDATE:
		return "UPDATE";
	case SQL_SET:
		return "SET";
	case SQL_ORDER:
		return "ORDER";
	case SQL_BY:
		return "BY";
	case SQL_DESC:
		return "DESC";
	case SQL_ASC:
		return "ASC";
	case SQL_IN:
		return "IN";
	case SQL_DELETE:
		return "DELETE";
	case SQL_GROUP:
		return "GROUP";
	case SQL_HAVING:
		return "HAVING";
	case SQL_GT:
		return ">";
	case SQL_LT:
		return "<";
	case SQL_GTEQ:
		return ">=";
	case SQL_LTEQ:
		return "<=";
	case SQL_NEQ:
		return "<>";
	case SQL_LIKE:
		return "LIKE";
	case SQL_IS:
		return "IS";
	case SQL_NULL:
		return "NULL";
	case SQL_NOT:
		return "NOT";
	case SQL_UNION:
		return "UNION";
	case SQL_CASE:
		return "CASE";
	case SQL_WHEN:
		return "WHEN";
	case SQL_THEN:
		return "THEN";
	case SQL_ELSE:
		return "ELSE";
	case SQL_END:
		return "END";
	case SQL_DISTINCT:
		return "DISTINCT";
	case SQL_EOS:
		return "<<EOS>>";
	case SQL_BEGIN:
		return "BEGIN";
	case SQL_TRANS:
		return "TRANS";
	case SQL_COMMIT:
		return "COMMIT";
	case SQL_ABORT:
		return "ABORT";
	case SQL_INVOKE:
		return "INVOKE";
	case SQL_ALTER:
		return "ALTER";
	case SQL_DECLARE:
		return "DECLARE";
	case SQL_SECTION:
		return "SECTION";
	case SQL_INCLUDE:
		return "INCLUDE";
	case SQL_CURSOR:
		return "CURSOR";
	case SQL_FOR:
		return "FOR";
	case SQL_COLON:
		return ":";
	case SQL_OPEN:
		return "OPEN";
	case SQL_FETCH:
		return "FETCH";
	case SQL_BETWEEN:
		return "BETWEEN";
	case SQL_CLOSE:
		return "CLOSE";
	case SQL_DATETIME:
		return "DATETIME";
	case SQL_FREE:
		return "FREE";
	case SQL_ROLLBACK:
		return "ROLLBACK";
	case SQL_CONTROL:
		return "CONTROL";
	case SQL_CAST:
		return "CAST";
	case SQL_EXISTS:
		return "EXISTS";
	case SQL_BROWSE:
		return "BROWSE";
	case SQL_STABLE:
		return "STABLE";
	case SQL_INTERVAL:
		return "INTERVAL";
	case SQL_LOCK:
		return "LOCK";
	case SQL_SUBSTRING:
		return "SUBSTRING";
	case SQL_EOF:
		return "<<EOF>>";
	case SQL_DATEFORMAT:
		return "DATEFORMAT";
	case SQL_TRIM:
		return "TRIM";
	case SQL_POSITION:
		return "POSITION";
	case SQL_PREPARE:
		return "PREPARE";
	case SQL_DESCRIBE:
		return "DESCRIBE";
	case SQL_EXEC:
		return "EXEC";
	default:
		return "unknown";
	}
}
