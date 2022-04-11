#include <spl/Environment.h>
#include <spl/io/Pipe.h>

#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif
#ifdef HAVE_SYS_FCNTL_H
#include <sys/fcntl.h>
#endif
#ifdef HAVE_FCNTL_H
#include <fcntl.h>
#endif
#ifdef HAVE_SYS_TYPES_H
#include <sys/types.h>
#endif
#ifdef HAVE_IO_H
#include <io.h>
#endif

using namespace spl;

Pipe::Pipe()
: m_read(), m_write(), m_txtRead(), m_txtWrite()
{
	int fd[2];
	
#ifdef _WINDOWS
	int ret = _pipe(fd, 256, O_BINARY);
#else
	int ret = pipe(fd);
#endif
	if (0 > ret)
	{
		throw new IOException(Environment::LastErrorMessage());
	}
	
	m_read = new FdStream(fd[0], File::FILEACC_Read, true);
	m_write = new FdStream(fd[1], File::FILEACC_Write, true);
}

Pipe::~Pipe()
{
}

spl::IStream& Pipe::Reader()
{
	return *m_read;
}

spl::IStream& Pipe::Writer()
{
	return *m_write;
}

TextReader& Pipe::ReaderText()
{
	if (m_txtRead.IsNull())
	{
		m_txtRead = new TextReader(m_read);
	}
	return *m_txtRead;
}

TextWriter& Pipe::WriterText()
{
	if (m_txtWrite.IsNull())
	{
		m_txtWrite = new TextWriter(m_write);
	}
	return *m_txtWrite;
}

void Pipe::Close()
{
	m_read->Close();
	m_write->Close();
}

#ifdef DEBUG
void Pipe::ValidateMem() const
{
	m_read.ValidateMem();
	m_write.ValidateMem();
	m_txtRead.ValidateMem();
	m_txtWrite.ValidateMem();
}

void Pipe::CheckMem() const
{
	m_read.CheckMem();
	m_write.CheckMem();
	m_txtRead.CheckMem();
	m_txtWrite.CheckMem();
}
#endif
