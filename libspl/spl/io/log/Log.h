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
#ifndef _log_h_
#define _log_h_

#include <spl/types.h>
#include <spl/Debug.h>
#include <spl/DateTime.h>
#include <spl/Exception.h>
#include <spl/collection/List.h>
#include <spl/String.h>
#include <spl/net/UdpSocket.h>

namespace spl
{
/**
 * @defgroup logging Logging
 * @ingroup core
 * @{
 */

class Log;
typedef RefCountPtr<Log> LogPtr;
typedef WeakReference<Log, LogPtr> LogRef;

/** @brief Syslog format logging class.
 *	Default output is to stdout.
 */
class Log : public IMemoryValidate
{
public:
	class OutputConfig
	{
	public:
		unsigned m_doLogEmergency : 1;
		unsigned m_doLogAlert : 1;
		unsigned m_doLogCritical : 1;
		unsigned m_doLogError : 1;
		unsigned m_doLogWarning : 1;
		unsigned m_doLogNotice : 1;
		unsigned m_doLogInfo : 1;
		unsigned m_doLogDebug : 1;

		inline OutputConfig()
		:	m_doLogEmergency(1),
			m_doLogAlert(1),
			m_doLogCritical(1),
			m_doLogError(1),
			m_doLogWarning(1),
			m_doLogNotice(1),
			m_doLogInfo(1),
			m_doLogDebug(1)
		{
		}
	};

	typedef enum _Severity
	{
		SevEmergency = 0,		//< system is unusable
		SevAlert = 1,			//< action must be taken immediately
		SevCritical = 2,		//< critical conditions
		SevError = 3,			//< error conditions
		SevWarning = 4,			//< warning
		SevNotice = 5,			//< normal but significant condition
		SevInfo = 6,			//< info
		SevDebug = 7			//< debugging
	} Severity;

	typedef enum _Facility
	{
		FacKernal = 0,
		FacUser = 1,
		FacMail = 2,
		FacDaemon = 3,
		FacAuthentication = 4,
		FacSyslog = 5,
		FacPrinter = 6,
		FacNews = 7,
		FacUUCP = 8,
		FacCron1 = 9,
		FacAuthorization = 10,
		FacFTP = 11,
		FacNTP = 12,
		FacLogAudit = 13,
		FacLogAlert = 14,
		FacCron2 = 15,
		FacLocal0 = 16,
		FacLocal1 = 17,
		FacLocal2 = 18,
		FacLocal3 = 19,
		FacLocal4 = 20,
		FacLocal5 = 21,
		FacLocal6 = 22,
		FacLocal7 = 23
	} Facility;

	typedef enum _LogType
	{
		LogToNone = 0,
		LogToConsole = 1<<1,
		LogToFile = 1<<2,
		LogToServer = 1<<3
	} LogType;

	typedef enum _LogRotation
	{
		RotNever = 0,
		RotDay = 1
	} LogRotation;

	class LogEntry : public IMemoryValidate
	{
	private:
		Facility m_facility;
		Severity m_severity;
		DateTime m_dts;
		String m_host;
		String m_process;
		int m_pid;
		String m_message;

	public:
		LogEntry();
		LogEntry(const LogEntry& le);
		LogEntry(Log::Facility facility, Log::Severity severity, const String& host, const String& process, int pid, const String& message);
		LogEntry(Log::Facility facility, Log::Severity severity, const String& message);
		virtual ~LogEntry();

		Log::LogEntry& operator =(const Log::LogEntry& le);

		inline Facility& GetFacility()
		{
			return m_facility;
		}

		inline Severity& GetSeverity()
		{
			return m_severity;
		}

		inline Severity GetSeverity() const
		{
			return m_severity;
		}

		inline int Priority() const
		{
			return m_facility * 8 + m_severity;
		}

		inline DateTime& GetDateTime()
		{
			return m_dts;
		}

		inline String& GetHost()
		{
			return m_host;
		}

		inline String& GetProcess()
		{
			return m_process;
		}

		inline int& GetPID()
		{
			return m_pid;
		}

		inline String& GetMessage()
		{
			return m_message;
		}

		StringPtr ToString() const;
		StringPtr ToShortString() const;

		static LogEntry Parse(const String& msg, unsigned long sourceIp);

#if defined(DEBUG)
		void CheckMem() const;
		void ValidateMem() const;
#endif
	};

	class LogServerInfo : public IMemoryValidate
	{
	private:
		String m_server;
		int m_port;
		OutputConfig m_logMsgTypes;
		UdpSocketPtr m_sock;

	public:
		LogServerInfo();
		LogServerInfo(const String& server, int port, OutputConfig logMsgTypes);
		LogServerInfo(const String& server, int port);
		virtual ~LogServerInfo();

		LogServerInfo& operator =(const LogServerInfo& lsi);

		OutputConfig& GetWhereToLog() { return m_logMsgTypes; }
		void Send(const LogEntry& le);
		void Send(const String& line);

		inline void Close()
		{
			m_sock->Close();
		}

#if defined(DEBUG)
		void CheckMem() const;
		void ValidateMem() const;
#endif
	};

protected:
	static bool _LogDoLog(const Log::LogEntry& le, Log::OutputConfig m_msgToLog);

protected:
	Facility m_facility;
	int m_logType;
	int m_messageCount;
	String m_fileName;
	List<LogServerInfo> m_servers;
	OutputConfig m_msgToLog;
	LogRotation m_logRotation;

	static char m_staticFileName[512];
	static char m_staticServerName[128];
	static int m_staticServerPort;
	static int m_staticMessageCount;
	static int m_staticOldMsgCount;
	static int m_staticLogType;
	static LogRotation m_staticLogRotation;

	static void SWrite(const LogEntry& le);

	bool DoLog(const LogEntry& le);

public:
	Log(String filename = "");
	Log(Facility facility, LogType logType, String filename = "");
	Log(const Log& l);
	virtual ~Log();
	Log& operator =(const Log& l);

	inline void AddServer(const String& hostName, int port = 514) { m_servers.Add(LogServerInfo(hostName, port)); }
	inline void AddServer(const String& hostName, OutputConfig cf, int port = 514) { m_servers.Add(LogServerInfo(hostName, port, cf)); }
	inline void ClearServers() { m_servers.Clear(); }

	inline void DoLogEmergency() { m_msgToLog.m_doLogEmergency = 1; }
	inline void DoLogAlert() { m_msgToLog.m_doLogAlert = 1; }
	inline void DoLogCritical() { m_msgToLog.m_doLogCritical = 1; }
	inline void DoLogError() { m_msgToLog.m_doLogError = 1; }
	inline void DoLogWarning() { m_msgToLog.m_doLogWarning = 1; }
	inline void DoLogNotice() { m_msgToLog.m_doLogNotice = 1; }
	inline void DoLogInfo() { m_msgToLog.m_doLogInfo = 1; }
	inline void DoLogDebug() { m_msgToLog.m_doLogDebug = 1; }

	inline void DontLogEmergency() { m_msgToLog.m_doLogEmergency = 0; }
	inline void DontLogAlert() { m_msgToLog.m_doLogAlert = 0; }
	inline void DontLogCritical() { m_msgToLog.m_doLogCritical = 0; }
	inline void DontLogError() { m_msgToLog.m_doLogError = 0; }
	inline void DontLogWarning() { m_msgToLog.m_doLogWarning = 0; }
	inline void DontLogNotice() { m_msgToLog.m_doLogNotice = 0; }
	inline void DontLogInfo() { m_msgToLog.m_doLogInfo = 0; }
	inline void DontLogDebug() { m_msgToLog.m_doLogDebug = 0; }

	inline void SetLogType(LogType lt) { m_logType = lt; }
	inline void SetFacility(Facility facility) { m_facility = facility; }
	inline void SetLogRotation(LogRotation lrot) { m_logRotation = lrot; }

	virtual void Write(const LogEntry& le);
	void Write(const OutOfMemoryException& mex);
	void Write(Exception *ex);
	void Write(Severity severity, const String& message);
	void Write(Severity severity, Facility facility, const String& process, const String& message);
	void Write(Severity severity, Facility facility, const String& process, int pid, const String& message);
	void Write(Severity severity, Facility facility, const String& host, const String& process, int pid, const String& message);

	void WriteError(const String& message);
	void WriteWarn(const String& message);
	void WriteInfo(const String& msg);

	inline int GetMessageCount() const { return m_messageCount; }

	static void SSetLogType(LogType lt);
	static void SSetLogFileName(const String& filename);
	static void SSetSyslogServer(const String& server);
	static void SSetStaticSyslogPort(int port);
	static void SSetLogRotation(LogRotation lrot);

	static void SWrite(const OutOfMemoryException& mex);
	static void SWrite(Exception *ex);
	static void SWrite(Severity severity, const String& message);
	static void SWrite(Severity severity, Facility facility, const String& process, const String& message);
	static void SWrite(Severity severity, Facility facility, const String& process, int pid, const String& message);
	static void SWrite(Severity severity, Facility facility, const String& host, const String& process, int pid, const String& message);

	static void SWriteError(const String& message);
	static void SWriteWarn(const String& message);
	static void SWriteInfo(const String& msg);

	static int SMessageCount();

	static void SWriteOkFail( const String& msg );
	static void SWriteEndOfRunTotal();

	static bool IsSeverity(const String& severity);
	static bool IsFacility(const String& facility);
	static Severity ParseSeverity(const String& severity);
	static Facility ParseFacility(const String& facility);
	static String FacilityToString(Facility facility);

#if defined(DEBUG)
	void CheckMem() const;
	void ValidateMem() const;
#endif
};

/** @} */
}
#endif


