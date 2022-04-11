/*
 *   This file is part of the Syslog X.
 *
 *   Syslog X is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   Syslog X is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with Syslog X.  If not, see <http://www.gnu.org/licenses/>.
 */
#include <spl/Environment.h>
#include <spl/io/log/ThreadedLog.h>

using namespace spl;

ThreadedLog::ThreadedLog()
:	m_file(),
	Log(Log::FacLocal0, Log::LogToServer),
	m_relay(),
	m_running(false),
	m_thread(this, &ThreadedLog::Run)
{
	m_file = FileAppendServicePtr(new FileAppendService(*Environment::ProcessName().Replace(".exe", "") + ".log"));	
	m_thread.Start();
}

ThreadedLog::~ThreadedLog()
{
	Close();
}

void ThreadedLog::Close()
{
	m_running = false;
	m_relay.Put(Log::LogEntry(Log::FacSyslog, Log::SevNotice, "Logging service shutdown"));
	m_file->Stop();
}

void ThreadedLog::Write(const Log::LogEntry& le)
{
	m_file->WriteLine(le.ToString());
	m_relay.Put(le);
}

void ThreadedLog::Run()
{
	if (m_running)
	{
		throw new Exception("ThreadedLog is already running");
	}
	m_running = true;
	
	while (m_running)
	{
		LogEntry le(m_relay.Get());
		
		for (List<Log::LogServerInfo>::Iterator iter = m_servers.Begin(); iter.Next(); )
		{
			if( _LogDoLog(le, iter.CurrentRef().GetWhereToLog()))
			{
				iter.CurrentRef().Send(le);
			}
		}
	}
}
