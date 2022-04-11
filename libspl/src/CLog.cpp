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

#include <spl/types.h>

#include <spl/Debug.h>
#include <spl/io/log/Log.h>
#ifdef HAVE_STRING_H
#include <string.h>
#endif
#ifdef HAVE_TIME_H
#include <time.h>
#endif
#ifdef HAVE_SYS_TIME_H
#include <sys/time.h>
#endif
#include <stdio.h>
#include <spl/Environment.h>
#include <spl/Exception.h>
#include <spl/io/File.h>
#include <spl/Int32.h>
#include <spl/threading/Mutex.h>
#include <spl/text/StringBuffer.h>
#include <spl/net/UdpSocket.h>

using namespace spl;

bool Log::_LogDoLog(const Log::LogEntry& le, Log::OutputConfig m_msgToLog)
{
	return (le.GetSeverity() == Log::SevAlert && m_msgToLog.m_doLogAlert) ||
		(le.GetSeverity() == Log::SevCritical && m_msgToLog.m_doLogCritical) ||
		(le.GetSeverity() == Log::SevDebug && m_msgToLog.m_doLogDebug) ||
		(le.GetSeverity() == Log::SevEmergency && m_msgToLog.m_doLogEmergency) ||
		(le.GetSeverity() == Log::SevError && m_msgToLog.m_doLogError) ||
		(le.GetSeverity() == Log::SevInfo && m_msgToLog.m_doLogInfo) ||
		(le.GetSeverity() == Log::SevNotice && m_msgToLog.m_doLogNotice) ||
		(le.GetSeverity() == Log::SevWarning && m_msgToLog.m_doLogWarning);
}

Log::LogEntry::LogEntry()
:	m_facility(Log::FacLocal1),
	m_severity(Log::SevInfo),
	m_dts(),
	m_host(Environment::HostName()),
	m_process(Environment::ProcessName()),
	m_pid(),
	m_message()
{
#ifndef _WINDOWS
	m_pid = (int)getpid();
#endif
}

Log::LogEntry::LogEntry(Log::Facility facility, Log::Severity severity, const String& message)
:	m_facility(facility),
	m_severity(severity),
	m_dts(),
	m_host(Environment::HostName()),
	m_process(Environment::ProcessName()),
	m_pid(),
	m_message(message)
{
#ifndef _WINDOWS
	m_pid = (int)getpid();
#endif
}

Log::LogEntry::LogEntry(const LogEntry& le)
:	m_facility(le.m_facility),
	m_severity(le.m_severity),
	m_dts(le.m_dts),
	m_host(le.m_host),
	m_process(le.m_process),
	m_pid(le.m_pid),
	m_message(le.m_message)
{
}

Log::LogEntry::LogEntry
(
	Log::Facility facility,
	Log::Severity severity,
	const String& host,
	const String& process,
	int pid,
	const String& message
)
:	m_facility(facility),
	m_severity(severity),
	m_dts(),
	m_host(host),
	m_process(process),
	m_pid(pid),
	m_message(message)
{
}

Log::LogEntry::~LogEntry()
{
}

Log::LogEntry& Log::LogEntry::operator =(const LogEntry& le)
{
	m_facility = le.m_facility;
	m_severity = le.m_severity;
	m_dts = le.m_dts;
	m_host = le.m_host;
	m_process = le.m_process;
	m_pid = le.m_pid;
	m_message = le.m_message;

	return *this;
}

StringPtr Log::LogEntry::ToString() const
{
	StringBuffer buf(30 + m_host.Length() + m_process.Length() + m_message.Length());
	buf.Append('<');
	buf.Append(Int32::ToString(Priority()));
	buf.Append('>');
	buf.Append(m_dts.ToStringISO8601());
	buf.Append(' ');
	buf.Append(m_host);
	buf.Append(' ');
	buf.Append(m_process);
	if (m_pid > 0)
	{
		buf.Append('[');
		buf.Append(Int32::ToString(m_pid));
		buf.Append(']');
	}
	buf.Append(": ");
	buf.Append(m_message);

	if (buf.Length() < 1025)
	{
		return buf.ToString();
	}

	StringPtr line(buf.ToString());
	return line->Substring(0, 1024);
}

StringPtr Log::LogEntry::ToShortString() const
{
	StringBuffer buf(20 + m_message.Length());
	buf.Append(m_dts.ToTimeString());
	buf.Append(' ');

	switch(m_severity)
	{
		case Log::SevEmergency:
			buf.Append("PANIC ");
			break;
		case Log::SevAlert:
			buf.Append("ALERT ");
			break;
		case Log::SevCritical:
			buf.Append("CRITL ");
			break;
		case Log::SevError:
			buf.Append("ERROR ");
			break;
		case Log::SevWarning:
			buf.Append("WARNG ");
			break;
		case Log::SevNotice:
			buf.Append("NOTIC ");
			break;
		case Log::SevInfo:
			buf.Append("INFO: ");
			break;
		case Log::SevDebug:
			buf.Append("DEBUG ");
			break;
		default:
			buf.Append("????? ");
			break;
	}

	buf.Append(m_message);

	if (buf.Length() < 1025)
	{
		return buf.ToString();
	}

	StringPtr line(buf.ToString());
	return line->Substring(0, 1024);
}

Log::LogEntry Log::LogEntry::Parse(const String& msg, unsigned long sourceIp)
{
	LogEntry le;
	le.GetMessage() = msg;

	struct in_addr ia;
	ia.s_addr = sourceIp;
	le.GetHost() = String(inet_ntoa(ia));

	if (msg.CharAt(0) != '<')
	{
		return le;
	}

	int idx = msg.IndexOf('>');
	if (0 > idx)
	{
		return le;
	}

	StringPtr str(msg.Mid(1, idx));
	if (! Int32::IsInt(str))
	{
		return le;
	}
	int priority = Int32::Parse(str);
	le.GetFacility() = (Log::Facility)(priority / 8);
	le.GetSeverity() = (Log::Severity)(priority >> 3);

	int colonPos = msg.LastIndexOf(":");
	if (0 > colonPos)
	{
		return le;
	}

	StringPtr header = msg.Mid(idx+1, colonPos);
	StringPtr text = msg.Substring(colonPos + 1)->Trim();

	RefCountPtr<Vector<StringPtr> > parts(header->Split(" "));
	if (parts->Count() < 2 || parts->Count() > 4 || ! Date::IsDate(parts->ElementAt(0)))
	{
		return le;
	}
	String sdtm = *parts->ElementAt(0) + " " + *parts->ElementAt(1);
	if (! DateTime::IsDateTime(sdtm))
	{
		return le;
	}
	le.GetDateTime() = DateTime::Parse(sdtm);

	le.GetMessage() = msg;
	if (parts->Count() > 2)
	{
		le.GetHost() = parts->ElementAt(2);
	}
	if (parts->Count() > 3)
	{
		StringPtr tag = parts->ElementAt(3);
		if ((idx = tag->IndexOf('[')) > -1 && tag->IndexOf(']') > -1 )
		{
			le.GetProcess() = tag->Substring(0, idx);
			StringPtr spid = tag->Mid(idx+1, tag->IndexOf(']'));
			if (Int32::IsInt(spid))
			{
				le.GetPID() = Int32::Parse(spid);
			}
			else
			{
				le.GetProcess() = tag;
			}
		}
		else
		{
			le.GetProcess() = tag;
		}
	}

	return le;
}

#if defined(DEBUG)
void Log::LogEntry::CheckMem() const
{
	m_host.CheckMem();
	m_process.CheckMem();
	m_message.CheckMem();
}

void Log::LogEntry::ValidateMem() const
{
	m_host.CheckMem();
	m_process.CheckMem();
	m_message.CheckMem();
}
#endif

Log::LogServerInfo::LogServerInfo()
: m_server(), m_port(0), m_sock()
{
}

Log::LogServerInfo::LogServerInfo(const String& server, int port)
: m_server(server), m_port(port), m_sock(new UdpSocket())
{
	m_logMsgTypes.m_doLogAlert = 1;
	m_logMsgTypes.m_doLogCritical = 1;
	m_logMsgTypes.m_doLogDebug = 1;
	m_logMsgTypes.m_doLogEmergency = 1;
	m_logMsgTypes.m_doLogError = 1;
	m_logMsgTypes.m_doLogInfo = 1;
	m_logMsgTypes.m_doLogNotice = 1;
	m_logMsgTypes.m_doLogWarning = 1;
}

Log::LogServerInfo::LogServerInfo(const String& server, int port, OutputConfig logMsgTypes)
: m_server(server), m_port(port), m_logMsgTypes(logMsgTypes), m_sock(new UdpSocket())
{
}

Log::LogServerInfo::~LogServerInfo()
{
}

Log::LogServerInfo& Log::LogServerInfo::operator =(const LogServerInfo& lsi)
{
	m_server = lsi.m_server;
	m_port = lsi.m_port;
	m_logMsgTypes = lsi.m_logMsgTypes;
	m_sock = lsi.m_sock;

	return *this;
}

void Log::LogServerInfo::Send(const LogEntry& le)
{
	m_sock->Send(m_server, m_port, le.ToString()->ToByteArray());
}

void Log::LogServerInfo::Send(const String& line)
{
	m_sock->Send(m_server, m_port, line.ToByteArray());
}

#if defined(DEBUG)
void Log::LogServerInfo::CheckMem() const
{
	m_server.CheckMem();
	m_sock.CheckMem();
}

void Log::LogServerInfo::ValidateMem() const
{
	m_server.ValidateMem();
	m_sock.ValidateMem();
}
#endif

Log::Log(String filename)
:	m_facility(Log::FacLocal0),
	m_logType(Log::LogToConsole),
	m_messageCount(0),
	m_servers(),
	m_msgToLog(),
	m_logRotation(RotNever),
	m_fileName(filename)
{
}

Log::Log(Facility facility, LogType logType, String filename)
:	m_facility(facility),
	m_logType(logType),
	m_servers(),
	m_messageCount(0),
	m_msgToLog(),
	m_logRotation(RotNever),
	m_fileName(filename)
{
}

Log::Log(const Log& l)
:	m_facility(l.m_facility),
	m_logType(l.m_logType),
	m_messageCount(l.m_messageCount),
	m_fileName(l.m_fileName),
	m_servers(l.m_servers),
	m_logRotation(l.m_logRotation)
{
	m_msgToLog = l.m_msgToLog;
}

Log::~Log()
{
	for (List<LogServerInfo>::Iterator iter = m_servers.Begin(); iter.Next(); )
	{
		iter.CurrentRef().Close();
	}
}

Log& Log::operator =(const Log& l)
{
	m_facility = l.m_facility;
	m_logType = l.m_logType;
	m_messageCount = l.m_messageCount;
	m_fileName = l.m_fileName;
	m_servers = l.m_servers;
	m_msgToLog = l.m_msgToLog;
	m_logRotation = l.m_logRotation;

	return *this;
}

int Log::m_staticLogType = Log::LogToConsole;
char Log::m_staticFileName[512] = {0};
char Log::m_staticServerName[128] = {0};
int Log::m_staticServerPort = 514;
int Log::m_staticMessageCount = 0;
int Log::m_staticOldMsgCount = 0;
Log::LogRotation Log::m_staticLogRotation = Log::RotNever;

static Mutex g_loglock;

static void _LogWriteFile(const Log::LogEntry& le, const String& filename)
{
	ASSERT(filename.Length() > 0);

	g_loglock.Lock();

	try
	{
		TextWriterPtr writer = File::AppendText(filename);
		writer->WriteLine(le.ToString());
		writer->Close();
	}
	catch(Exception *ex)
	{
		printf("Error writing log file (%s) %s\n", filename.GetChars(), ex->Message());
		delete ex;
	}

	g_loglock.Unlock();
}

static void _LogWriteServer(const Log::LogEntry& le, const String& server, int port)
{
	ASSERT(server.Length() > 0 && port > 0);

	StringPtr line(le.ToString());
	UdpSocket sock;
	sock.Send(server, port, line->ToByteArray());
	sock.Close();
}

static void _LogWriteConsole(const Log::LogEntry& le)
{
	g_loglock.Lock();
	StringPtr line(le.ToShortString());
	printf("%s\n", line->GetChars());
	g_loglock.Unlock();
}

static void _LogWrite
(
	const Log::LogEntry& le,
	int logType,
	const String& filename,
	const String& server,
	int port,
	Log::LogRotation lrot
)
{
	if (0 != (logType & Log::LogToFile) && filename.Length() > 0)
	{
		if (Log::RotNever == lrot)
		{
			_LogWriteFile(le, filename);
		}
		else
		{
			int pos = filename.LastIndexOf(".");
			if (pos < 0)
			{
				_LogWriteFile(le, filename + Int32::ToString(Date::Now().ToRevInt()));
			}
			else
			{
				_LogWriteFile(le, *filename.Substring(0, pos) + Int32::ToString(Date::Now().ToRevInt()) + *filename.Substring(pos));
			}
		}
	}
	if (0 != (logType & Log::LogToServer) && server.Length() > 0)
	{
		_LogWriteServer(le, server, port);
	}
	if (0 != (logType & Log::LogToConsole))
	{
		_LogWriteConsole(le);
	}
}

bool Log::DoLog(const LogEntry& le)
{
	return _LogDoLog(le, m_msgToLog);
}

void Log::SWrite(const LogEntry& le)
{
	_LogWrite(le, m_staticLogType, m_staticFileName, m_staticServerName, m_staticServerPort, m_staticLogRotation);
}

void Log::Write(const LogEntry& le)
{
	if (! DoLog(le))
	{
		return;
	}
	m_messageCount++;

	_LogWrite(le, m_logType, m_fileName, "", 0, m_logRotation);

	if (0 != (m_logType & Log::LogToServer) && m_servers.Count() > 0)
	{
		for (List<LogServerInfo>::Iterator iter = m_servers.Begin(); iter.Next(); )
		{
			if( _LogDoLog(le, iter.CurrentRef().GetWhereToLog()))
			{
				iter.CurrentRef().Send(le);
			}
		}
	}
}

void Log::SWriteOkFail( const String& msg )
{
	LogEntry le;

	if ( m_staticMessageCount == m_staticOldMsgCount )
	{
		le.GetMessage() = msg + " OK";
	}
	else
	{
		m_staticMessageCount++;
		le.GetMessage() = msg + " FAIL";
	}
	SWrite(le);

	m_staticOldMsgCount = m_staticMessageCount;
}

void Log::SWriteEndOfRunTotal()
{
	LogEntry le;
	if ( 0 == m_staticMessageCount )
	{
		le.GetMessage() = "No errors.";
	}
	else
	{
		le.GetMessage() = "Errors.";
	}
	SWrite(le);
}

void Log::Write(const OutOfMemoryException& mex)
{
	WriteError(mex.Message());
}

void Log::Write(Exception *ex)
{
	WriteError(ex->Message());
}

void Log::Write(Severity severity, const String& message)
{
	LogEntry le(m_facility, severity, message);
	Write(le);
}

void Log::Write(Severity severity, Facility facility, const String& process, const String& message)
{
	LogEntry le(facility, severity, message);
	le.GetProcess() = process;
	le.GetPID() = 0;
	Write(le);
}

void Log::Write(Severity severity, Facility facility, const String& process, int pid, const String& message)
{
	LogEntry le(facility, severity, message);
	le.GetProcess() = process;
	le.GetPID() = pid;
	Write(le);
}

void Log::Write(Severity severity, Facility facility, const String& host, const String& process, int pid, const String& message)
{
	LogEntry le(facility, severity, host, process, pid, message);
	Write(le);
}

void Log::WriteError(const String& message)
{
	LogEntry le(m_facility, Log::SevError, message);
	Write(le);
}

void Log::WriteWarn(const String& message)
{
	LogEntry le(m_facility, Log::SevWarning, message);
	Write(le);
}

void Log::WriteInfo(const String& message)
{
	LogEntry le(m_facility, Log::SevInfo, message);
	Write(le);
}

void Log::SSetLogType(LogType lt)
{
	m_staticLogType = lt;
}

void Log::SSetLogRotation(LogRotation lrot)
{
	m_staticLogRotation = lrot;
}

void Log::SSetLogFileName(const String& filename)
{
	spl::StrCpyLen(m_staticFileName, filename.GetChars(), sizeof(m_staticFileName));
}

void Log::SSetSyslogServer(const String& server)
{
	spl::StrCpyLen(m_staticServerName, server.GetChars(), sizeof(m_staticServerName));
}

void Log::SSetStaticSyslogPort(int port)
{
	m_staticServerPort = port;
}

void Log::SWrite(const OutOfMemoryException& mex)
{
	SWriteError(mex.Message());
}

void Log::SWrite(Exception *ex)
{
	SWriteError(ex->Message());
}

void Log::SWrite(Severity severity, const String& message)
{
	LogEntry le(Log::FacLocal0, severity, message);
	SWrite(le);
}

void Log::SWrite(Severity severity, Facility facility, const String& process, const String& message)
{
	LogEntry le(facility, severity, message);
	le.GetProcess() = process;
	le.GetPID() = 0;
	SWrite(le);
}

void Log::SWrite(Severity severity, Facility facility, const String& process, int pid, const String& message)
{
	LogEntry le(facility, severity, message);
	le.GetProcess() = process;
	le.GetPID() = pid;
	SWrite(le);
}

void Log::SWrite(Severity severity, Facility facility, const String& host, const String& process, int pid, const String& message)
{
	LogEntry le(facility, severity, host, process, pid, message);
	SWrite(le);
}

void Log::SWriteError(const String& message)
{
	m_staticMessageCount++;
	LogEntry le(Log::FacLocal0, Log::SevError, message);
	SWrite(le);
}

void Log::SWriteWarn(const String& message)
{
	LogEntry le(Log::FacLocal0, Log::SevWarning, message);
	SWrite(le);
}

void Log::SWriteInfo(const String& message)
{
	LogEntry le(Log::FacLocal0, Log::SevInfo, message);
	SWrite(le);
}

int Log::SMessageCount()
{
	return m_staticMessageCount;
}

Log::Severity Log::ParseSeverity(const String& severity)
{
	if (severity.EqualsIgnoreCase("Emergency"))
	{
		return SevEmergency;
	}
	if (severity.EqualsIgnoreCase("Alert"))
	{
		return SevAlert;
	}
	if (severity.EqualsIgnoreCase("Critical"))
	{
		return SevCritical;
	}
	if (severity.EqualsIgnoreCase("Error"))
	{
		return SevError;
	}
	if (severity.EqualsIgnoreCase("Warning"))
	{
		return SevWarning;
	}
	if (severity.EqualsIgnoreCase("Warn"))
	{
		return SevWarning;
	}
	if (severity.EqualsIgnoreCase("Notice"))
	{
		return SevNotice;
	}
	if (severity.EqualsIgnoreCase("Info"))
	{
		return SevInfo;
	}
	if (severity.EqualsIgnoreCase("Information"))
	{
		return SevInfo;
	}
	if (severity.EqualsIgnoreCase("Informational"))
	{
		return SevInfo;
	}
	if (severity.EqualsIgnoreCase("Debug"))
	{
		return SevDebug;
	}
	return SevInfo;
}

String Log::FacilityToString(Facility facility)
{
	switch(facility)
	{
		case FacKernal: return "KERNAL";
		case FacUser: return "USER";
		case FacMail: return "MAIL";
		case FacDaemon: return "DAEMON";
		case FacAuthentication: return "AUTHENTICATION";
		case FacSyslog: return "SYSLOG";
		case FacPrinter: return "PRINTER";
		case FacNews: return "NEWS";
		case FacUUCP: return "UUCP";
		case FacCron1: return "CRON";
		case FacAuthorization: return "AUTHORIZATION";
		case FacFTP: return "FTP";
		case FacNTP: return "NTP";
		case FacLogAudit: return "AUDIT";
		case FacLogAlert: return "ALERT";
		case FacCron2: return "CRON2";
		case FacLocal0: return "LOCAL0";
		case FacLocal1: return "LOCAL1";
		case FacLocal2: return "LOCAL2";
		case FacLocal3: return "LOCAL3";
		case FacLocal4: return "LOCAL4";
		case FacLocal5: return "LOCAL5";
		case FacLocal6: return "LOCAL6";
		case FacLocal7: return "LOCAL7";
		default: return "LOCAL0";
	}
}

Log::Facility Log::ParseFacility(const String& facility)
{
	if (facility.EqualsIgnoreCase(FacilityToString(FacKernal)))
	{
		return FacKernal;
	}
	if (facility.EqualsIgnoreCase(FacilityToString(FacUser)))
	{
		return FacUser;
	}
	if (facility.EqualsIgnoreCase(FacilityToString(FacMail)))
	{
		return FacMail;
	}
	if (facility.EqualsIgnoreCase(FacilityToString(FacDaemon)))
	{
		return FacDaemon;
	}
	if (facility.EqualsIgnoreCase(FacilityToString(FacAuthentication)))
	{
		return FacAuthentication;
	}
	if (facility.EqualsIgnoreCase("Auth"))
	{
		return FacAuthentication;
	}
	if (facility.EqualsIgnoreCase(FacilityToString(FacSyslog)))
	{
		return FacSyslog;
	}
	if (facility.EqualsIgnoreCase(FacilityToString(FacPrinter)))
	{
		return FacPrinter;
	}
	if (facility.EqualsIgnoreCase(FacilityToString(FacNews)))
	{
		return FacNews;
	}
	if (facility.EqualsIgnoreCase("NNP"))
	{
		return FacNews;
	}
	if (facility.EqualsIgnoreCase(FacilityToString(FacUUCP)))
	{
		return FacUUCP;
	}
	if (facility.EqualsIgnoreCase(FacilityToString(FacCron1)))
	{
		return FacCron1;
	}
	if (facility.EqualsIgnoreCase("Cron"))
	{
		return FacCron1;
	}
	if (facility.EqualsIgnoreCase(FacilityToString(FacAuthorization)))
	{
		return FacAuthorization;
	}
	if (facility.EqualsIgnoreCase(FacilityToString(FacFTP)))
	{
		return FacFTP;
	}
	if (facility.EqualsIgnoreCase(FacilityToString(FacNTP)))
	{
		return FacNTP;
	}
	if (facility.EqualsIgnoreCase(FacilityToString(FacLogAudit)))
	{
		return FacLogAudit;
	}
	if (facility.EqualsIgnoreCase(FacilityToString(FacLogAlert)))
	{
		return FacLogAlert;
	}
	if (facility.EqualsIgnoreCase(FacilityToString(FacCron2)))
	{
		return FacCron2;
	}
	if (facility.EqualsIgnoreCase(FacilityToString(FacLocal0)))
	{
		return FacLocal0;
	}
	if (facility.EqualsIgnoreCase("Local"))
	{
		return FacLocal0;
	}
	if (facility.EqualsIgnoreCase(FacilityToString(FacLocal1)))
	{
		return FacLocal1;
	}
	if (facility.EqualsIgnoreCase(FacilityToString(FacLocal2)))
	{
		return FacLocal2;
	}
	if (facility.EqualsIgnoreCase(FacilityToString(FacLocal3)))
	{
		return FacLocal3;
	}
	if (facility.EqualsIgnoreCase(FacilityToString(FacLocal4)))
	{
		return FacLocal4;
	}
	if (facility.EqualsIgnoreCase(FacilityToString(FacLocal5)))
	{
		return FacLocal5;
	}
	if (facility.EqualsIgnoreCase(FacilityToString(FacLocal6)))
	{
		return FacLocal6;
	}
	if (facility.EqualsIgnoreCase(FacilityToString(FacLocal7)))
	{
		return FacLocal7;
	}
	return FacLocal0;
}

bool Log::IsSeverity(const String& severity)
{
	Severity s = ParseSeverity(severity);
	if (SevInfo == s && !severity.EqualsIgnoreCase("info"))
	{
		return false;
	}
	return true;
}

bool Log::IsFacility(const String& facility)
{
	Facility f = ParseFacility(facility);
	if (FacLocal0 == f)
	{
		if (! facility.EqualsIgnoreCase("local") && ! facility.EqualsIgnoreCase("local0"))
		{
			return false;
		}
	}
	return true;
}

#if defined(DEBUG)
void Log::CheckMem() const
{
	m_fileName.CheckMem();
	m_servers.CheckMem();
}

void Log::ValidateMem() const
{
	m_fileName.ValidateMem();
	m_servers.ValidateMem();
}
#endif
