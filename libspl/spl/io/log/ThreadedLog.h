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
#ifndef _threadedlog_h
#define _threadedlog_h

#include <spl/io/log/Log.h>
#include <spl/threading/ThreadStartDelegate.h>
#include <spl/net/UdpServer.h>
#include <spl/io/log/FileAppendService.h>

namespace spl
{
	class ThreadedLog : public Log
	{
	private:
		bool m_running;
		FileAppendServicePtr m_file;
		Queue<Log::LogEntry> m_relay;
		ThreadStartDelegate<ThreadedLog> m_thread;

		inline ThreadedLog(const ThreadedLog& tl) : Log() {}
		inline ThreadedLog& operator =(const ThreadedLog& tl) {throw Exception();}

		void Run();

		inline void Join() { m_thread.Join(); }

	public:
		ThreadedLog();
		virtual ~ThreadedLog();

		void Close();

		virtual void Write(const Log::LogEntry& le);

		inline void Write(Severity severity, Facility facility, const String& process, const String& message)
		{
			Log::Write(severity, facility, process, message);
		}
	};
}

#endif
