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
#include <spl/io/File.h>
#include <spl/io/log/Log.h>
#include <spl/io/log/FileAppendService.h>

using namespace spl;

FileAppendService::FileAppendService(const String& fileName)
: m_fileName(fileName), m_running(false), m_thread(), m_q()
{	
	m_thread = RefCountPtr<ThreadStartDelegate<FileAppendService> >(new ThreadStartDelegate<FileAppendService>(this, &FileAppendService::Run));
	m_thread->Start();
}

FileAppendService::~FileAppendService()
{
	Stop();
}

void FileAppendService::Stop()
{
	m_running = false;
}

void FileAppendService::WriteLine(const String& line)
{
	m_q.Put(line);
}

void FileAppendService::Run()
{
	if (m_running)
	{
		throw new Exception("File append service is already running");
	}
	
	m_running = true;
	
	while (m_running)
	{
		String line = m_q.Get();
		TextWriterPtr writer;
		try
		{
			writer = File::AppendText(m_fileName);
		}
		catch (Exception *ex)
		{
			Log::SWrite(ex);
			continue;
		}
		try
		{
			writer->WriteLine(line);
		}
		catch (Exception *ex)
		{
			Log::SWrite(ex);
		}
		writer->Close();
	}
}

#if defined(_DEBUG)
void FileAppendService::CheckMem() const
{
	m_fileName.CheckMem();
	m_q.CheckMem();
}

void FileAppendService::ValidateMem() const
{
	m_fileName.ValidateMem();
	m_q.ValidateMem();
}
#endif
