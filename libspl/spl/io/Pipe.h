#ifndef _pipe_h
#define _pipe_h

#include <spl/types.h>
#include <spl/Debug.h>
#include <spl/Memory.h>
#include <spl/RefCountPtr.h>
#include <spl/io/FileStream.h>

namespace spl
{
class Pipe;
typedef RefCountPtr<Pipe> PipePtr;

class Pipe : public IMemoryValidate
{
protected:
	FdStreamPtr m_read;
	FdStreamPtr m_write;
	TextWriterPtr m_txtWrite;
	TextReaderPtr m_txtRead;
	
public:
	Pipe();
	virtual ~Pipe();
	
	spl::IStream& Reader();
	spl::IStream& Writer();
	
	TextReader& ReaderText();
	TextWriter& WriterText();
	
	void Close();
	
#ifdef DEBUG
	virtual void ValidateMem() const;
	virtual void CheckMem() const;
#endif
};
}
#endif
