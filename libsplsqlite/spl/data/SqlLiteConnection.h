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
#ifndef _sqlliteconnection_h
#define _sqlliteconnection_h

#include <spl/types.h>
#include <spl/Debug.h>
#include <spl/data/Connection.h>
#include <spl/RefCountPtr.h>
#include <spl/String.h>
#include <spl/WeakReference.h>

namespace spl
{
/** 
 * @defgroup sqlite SQLITE
 * @ingroup database
 * @{
 */

class SqlLiteConnection;
typedef RefCountPtrCast<SqlLiteConnection, Connection, ConnectionPtr> SqlLiteConnectionPtr;
typedef WeakReference<SqlLiteConnection, SqlLiteConnectionPtr> SqlLiteConnectionRef;

/** @brief Connection for Sqlite file.
 *
 */
class SqlLiteConnection : public Connection
{
private:
	// Copy constructor doesn't make sense for this class
	inline SqlLiteConnection(const SqlLiteConnection& con) : Connection("", "", "", "") {}
	inline void operator =(const SqlLiteConnection& con) {}

protected:
	void *m_db;		//< Actually a pointer to sqlite3

public:
	SqlLiteConnection(const String& databaseFilename);
	virtual ~SqlLiteConnection();

	/// @brief Open the connection, ready for use after Open.
	virtual void Open();

	/// @brief Close the connection, free resources.
	virtual void Close();

	/// \brief Change to a different database
	virtual void ChangeDatabase(const String& dbFilename);

	virtual TransactionPtr BeginTransaction();
	virtual CommandPtr CreateCommand();
	virtual CommandPtr CreateCommand(const String& cmdText);

	virtual int ExecuteNonQuery(const String& sql);
	virtual RecordSetPtr ExecuteQuery(const String& sql);

	/// @brief If multiple threads are accessing the connection, each thread needs to call this.
	virtual void RegisterThread();

	/// @brief If multiple threads are accessing the connection, each thread needs to call this on exit.
	virtual void RegisterThreadExit();

	/// @brief Setting to true requires Command's to query the parameter names from the stored procedure -- default: false.
	virtual void AllowOutOfOrderCommandParameters(bool allow);

#if defined(DEBUG)
	virtual void CheckMem() const;
	virtual void ValidateMem() const;
#endif
};

REGISTER_TYPEOF( 430, SqlLiteConnection );
REGISTER_TYPEOF( 432, SqlLiteConnectionPtr );
REGISTER_TYPEOF( 434, SqlLiteConnectionRef );

/** @} */
}
#endif
