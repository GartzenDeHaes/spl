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
#ifndef _sqllitecommand_h
#define _sqllitecommand_h

#include <spl/data/Command.h>
#include <spl/WeakReference.h>

namespace spl
{
/** 
 * @defgroup sqlite SQLITE
 * @ingroup database
 * @{
 */

class SqlLiteCommand;
typedef RefCountPtrCast<SqlLiteCommand, Command, CommandPtr> SqlLiteCommandPtr;
typedef WeakReference<SqlLiteCommand, SqlLiteCommandPtr> SqlLiteCommandRef;

/** @brief A parameterized sqlite query.
  * <p>In your SQL string, prepend '@' to the parameter names and use the same
  * fully qualified name in the command parameter.</p>&nbsp;<br/>
  * <pre>
  *	CommandPtr cmd = con.CreateParameter("SELECT * FROM data WHERE FIELD1 = @param1;");
  * cmd->CreateParameter("@param1", DbSqlType::SQL_TYPE_INT32, ParameterDirection::PARAM_DIR_IN, 4)->Set(1);
  * </pre>
  * <p>The parameter direction is ignored in SQLite, since only IN parameters are supported.</p>
  */
class SqlLiteCommand : public Command
{
protected:
	void *m_db;
	void *m_stmt;

	void BindParameters();

public:
	SqlLiteCommand();
	SqlLiteCommand(const SqlLiteCommand& cmd);
	SqlLiteCommand(void *db, const String& cmdtxt);
	virtual ~SqlLiteCommand();

	SqlLiteCommand& operator =(const SqlLiteCommand& cmd);
	
	virtual void Clear();

	virtual void Prepare();
	virtual int ExecuteNonQuery();
	virtual RecordSetPtr ExecuteQuery();
};

REGISTER_TYPEOF( 422, SqlLiteCommand );
REGISTER_TYPEOF( 424, SqlLiteCommandPtr );
REGISTER_TYPEOF( 426, SqlLiteCommandRef );

/** @} */
}
#endif
