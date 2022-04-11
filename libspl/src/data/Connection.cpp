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
#include <spl/data/Command.h>
#include <spl/data/Connection.h>

using namespace spl;

Transaction::Transaction()
{
}

Transaction::~Transaction()
{
}

Command::Command()
: m_prmIdx(), m_prm(), m_cmdtxt()
{
}

Command::Command(const String& cmdtxt)
: m_prmIdx(), m_prm(), m_cmdtxt(cmdtxt)
{
}

Command::Command(const Command& cmd)
: m_prmIdx(), m_prm(), m_cmdtxt()
{
	*this = cmd;
}

Command::~Command()
{
	//int count = m_prm.Count();
	//for ( int x = 0; x < count; x++ )
	//{
	//	delete m_prm.ElementAt(x);
	//}
}

Command& Command::operator =(const Command& cmd)
{
	Clear();

	m_cmdtxt = cmd.m_cmdtxt;

	int count = cmd.m_prm.Count();
	for ( int x = 0; x < count; x++ )
	{
		CommandParameterPtr prm = CommandParameterPtr(new CommandParameter(*cmd.m_prm.ElementAt(x)));
		m_prmIdx.Set( String(prm->Name()), prm );
		m_prm.Add( prm );
	}

	return *this;
}

void Command::Clear()
{
	//int count = m_prm.Count();
	//for ( int x = 0; x < count; x++ )
	//{
	//	delete m_prm.ElementAt(x);
	//}
	m_prm.Clear();
	m_prmIdx.Clear();
}

void Command::CommandTextSet(const String& txt)
{
	m_cmdtxt = txt;
}

CommandParameterPtr Command::CreateParameter(const String& name, int type, int direction, int len)
{
	CommandParameterPtr prm = CommandParameterPtr(new CommandParameter(name, type, direction, len));
	m_prmIdx.Set( prm->Name(), prm );
	m_prm.Add( prm );
	ValidateMem();
	return prm;
}

CommandParameterPtr Command::CreateParameter(const String& name, int type, int direction)
{
	CommandParameterPtr prm = CommandParameterPtr(new CommandParameter(name, type, direction, 4));
	m_prmIdx.Set( prm->Name(), prm );
	m_prm.Add( prm );
	return prm;
}

CommandParameterPtr Command::CreateParameter(const String& name, const String& value)
{
	CommandParameterPtr prm = CreateParameter(name, DbSqlType::SQL_TYPE_VARCHAR,ParameterDirection::PARAM_DIR_IN, value.Length());
	prm->Set(value);
	return prm;
}

CommandParameterPtr Command::CreateParameter(const String& name, int32 value)
{
	CommandParameterPtr prm = CreateParameter(name, DbSqlType::SQL_TYPE_INT32,ParameterDirection::PARAM_DIR_IN, 4);
	prm->Set(value);
	return prm;
}

CommandParameterPtr Command::CreateParameter(const String& name, int8 value)
{
	CommandParameterPtr prm = CreateParameter(name, DbSqlType::SQL_TYPE_INT8,ParameterDirection::PARAM_DIR_IN, 1);
	prm->Set(value);
	return prm;
}

CommandParameterPtr Command::CreateParameter(const String& name, float32 value)
{
	CommandParameterPtr prm = CreateParameter(name, DbSqlType::SQL_TYPE_FLOAT32,ParameterDirection::PARAM_DIR_IN, 4);
	prm->Set(value);
	return prm;
}

CommandParameterPtr Command::CreateParameter(const String& name, float64 value)
{
	CommandParameterPtr prm = CreateParameter(name, DbSqlType::SQL_TYPE_FLOAT64,ParameterDirection::PARAM_DIR_IN, 8);
	prm->Set(value);
	return prm;
}

CommandParameterPtr Command::GetParameter(const String& name) const
{
	return m_prmIdx.Get(name);
}

void Command::Prepare()
{
	throw new NotImplementedException ();
}

int Command::ExecuteNonQuery()
{
	throw new NotImplementedException ();
}

RecordSetPtr Command::ExecuteQuery()
{
	throw new NotImplementedException ();
}

#if defined(DEBUG)
void Command::CheckMem() const
{
	m_cmdtxt.CheckMem();
	m_prmIdx.CheckMem();
	m_prm.CheckMem();
	
	int count = m_prm.Count();
	for ( int x = 0; x < count; x++ )
	{
		CommandParameterPtr prm = m_prm.ElementAt(x);
		prm.CheckMem();
	}
}

void Command::ValidateMem() const
{
	m_cmdtxt.ValidateMem();
	m_prmIdx.ValidateMem();
	m_prm.ValidateMem();
	
	int count = m_prm.Count();
	for ( int x = 0; x < count; x++ )
	{
		CommandParameterPtr prm = m_prm.ElementAt(x);
		prm.ValidateMem();
	}
}
#endif

Connection::Connection
(
	const String& serverOrIP, 
	const String& database, 
	const String& uid, 
	const String& pw
)
: m_host(serverOrIP), m_database(database), m_uid(uid), m_pw(pw)
{
}

Connection::~Connection()
{
}

ConnectionPtr Connection::GetConnection(const String& connectString)
{
	String provider;
	String server;
	String database;
	String uid;
	String pw;

	String cs(connectString);
	RefCountPtr<Vector<StringPtr> > parts = cs.Split(";");
	int count = parts->Count();
	int idx;
	for ( int x = 0; x < count; x++ )
	{
		StringPtr part = parts->ElementAt(x);
		if ( 0 > (idx = part->IndexOf('=')) )
		{
			throw new SqlException("Invalid connection string");
		}
		StringPtr key = part->Substring(0, idx)->Trim()->ToLower();
		if ( key->Equals("provider") )
		{
			provider = *part->Substring(idx+1)->Trim();
		}
		else if ( key->Equals("server") )
		{
			server = *part->Substring(idx+1)->Trim();
		}
		else if ( key->Equals("database") )
		{
			database = *part->Substring(idx+1)->Trim();
		}
		else if ( key->Equals("uid") )
		{
			uid = *part->Substring(idx+1)->Trim();
		}
		else if ( key->Equals("user id") )
		{
			uid = *part->Substring(idx+1)->Trim();
		}
		else if ( key->Equals("pw") )
		{
			pw = *part->Substring(idx+1)->Trim();
		}
		else if ( key->Equals("pwd") )
		{
			pw = *part->Substring(idx+1)->Trim();
		}
		else if ( key->Equals("password") )
		{
			pw = *part->Substring(idx+1)->Trim();
		}
		else
		{
			throw new SqlException("Unknown connect string key");
		}
	}

	if ( provider.Length() == 0 )
	{
		throw new SqlException("provider key is required");
	}
	if ( server.Length() == 0 )
	{
		throw new SqlException("server key is required");
	}
	if ( database.Length() == 0 )
	{
		throw new SqlException("database key is required");
	}
	if ( uid.Length() == 0 )
	{
		throw new SqlException("user id key is required");
	}
	if ( pw.Length() == 0 )
	{
		throw new SqlException("pw key is required");
	}
	ConnectionPtr con;
	if ( provider.Equals("mysql") )
	{
#if defined(HAVE_MYSQL_H) || defined(HAVE_MYSQL_MYSQL_H)
		con = ConnectionPtr(new MySqlConnection(server, database, uid, pw));
#else
		throw new InvalidArgumentException("mysql isn't available on your system (on windows, enable HAVE_MYSQL_H in spl/configwin32.h).");
#endif
	}
	else if ( provider.Equals("mssql") )
	{
//#if defined(HAVE_SQLFRONT_H) && defined(HAVE_SQLDB_H)
//		throw new NotImplementedException("mssql support isn't complete yet");
//#else
//		throw new InvalidArgumentException("mssql isn't available on your system (on windows, enable HAVE_SQLFRONT_H and HAVE_SQLDB_H in spl/configwin32.h).");
//#endif
		throw new NotImplementedException("mssql is not longer supported.");
	}
	else
	{
		throw new InvalidArgumentException("Unsupported provider; must be mysql or mssql");
	}

	return con;
}

#if defined(DEBUG)
void Connection::CheckMem() const
{
	m_host.CheckMem();
	m_database.CheckMem();
	m_uid.CheckMem();
	m_pw.CheckMem();
}

void Connection::ValidateMem() const
{
	m_host.ValidateMem();
	m_database.ValidateMem();
	m_uid.ValidateMem();
	m_pw.ValidateMem();
}
#endif

