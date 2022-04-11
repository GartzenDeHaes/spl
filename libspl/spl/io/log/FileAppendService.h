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
#ifndef _fileappendservice_h
#define _fileappendservice_h

#include <spl/Memory.h>
#include <spl/collection/Queue.h>
#include <spl/String.h>
#include <spl/io/TextStream.h>
#include <spl/threading/ThreadStartDelegate.h>

namespace spl
{
	class FileAppendService;
	typedef RefCountPtr<FileAppendService> FileAppendServicePtr;
	typedef WeakReference<FileAppendService, FileAppendServicePtr> FileAppendServiceRef;

	class FileAppendService : public IMemoryValidate
	{
	private:
		bool m_running;
		String m_fileName;
		//TextWriterPtr m_out;
		RefCountPtr<ThreadStartDelegate<FileAppendService> > m_thread;
		Queue<String> m_q;

		inline FileAppendService(const FileAppendService&) {}
		inline FileAppendService& operator =(const FileAppendService&) {throw Exception();}

		void Run();

	public:
		FileAppendService(const String& fileName);
		virtual ~FileAppendService();

		void Stop();

		void WriteLine(const String& line);

	#if defined(DEBUG)
		void CheckMem() const;
		void ValidateMem() const;
	#endif
	};
}

#endif
