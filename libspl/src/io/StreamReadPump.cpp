/*   SPL is free software: you can redistribute it and/or modify
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
#if defined(_WIN32) || defined(_WIN64)
#include <spl/configwin32.h>
#include <stdio.h>
#else
#include <spl/autoconf/config.h>
#endif

#include <spl/io/StreamReadPump.h>

using namespace spl;

StreamReadPump::StreamReadPump(IStreamPtr strm, int bufsize)
:	m_strm(strm), 
	m_bufsize(bufsize),
	m_buf(bufsize),
	m_eventDispatch()
{
	ASSERT( m_strm->CanRead() );
	Start();
}

StreamReadPump::~StreamReadPump()
{
	m_running = false;
	m_strm->Close();
	free( m_buf );
}

void StreamReadPump::Run()
{
	m_running = true;
	try
	{
		while ( m_running )
		{
			int count = m_strm->Read(m_buf, 0, m_bufsize);
			if ( ! m_running )
			{
				continue;
			}
			if ( -1 == count )
			{
				m_eventDispatch.DispatchOnClose();
				m_running = false;
				continue;
			}
			if ( 0 == count )
			{
				throw new IOException("Read error");
			}
			m_eventDispatch.DispatchOnRead(m_buf, count);
		}
	}
	catch ( OutOfMemoryException mex )
	{
		m_eventDispatch.DispatchOnError(mex.Message());
	}
	catch ( Exception ex )
	{
		m_eventDispatch.DispatchOnError(ex.Message());
	}
	catch ( Exception *ex )
	{
		m_eventDispatch.DispatchOnError(ex->Message());
		delete ex;
	}
	m_running = false;
}

void StreamReadPump::Stop()
{
	m_running = false;
	m_strm->Close();
}

#ifdef DEBUG
void StreamReadPump::ValidateMem() const
{
	m_strm.ValidateMem();
	m_strm->ValidateMem();
	m_buf.ValidateMem();
	m_eventDispatch.ValidateMem();
}

void StreamReadPump::CheckMem() const
{
	m_strm.CheckMem();
	m_strm->CheckMem();
	m_buf.CheckMem();
	m_eventDispatch.CheckMem();
}
#endif
