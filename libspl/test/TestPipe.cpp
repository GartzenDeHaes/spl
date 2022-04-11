#include <spl/io/Pipe.h>

#ifdef DEBUG

#include <spl/io/log/Log.h>

using namespace spl;

static void _TestPipe1()
{
	PipePtr p(new Pipe());
	
	DEBUG_CLEAR_MEM_CHECK_POINTS();
	p.CheckMem();	
	DEBUG_DUMP_MEM_LEAKS();
	UNIT_ASSERT_MEM_NOTED("Pipe 1.0");

	p->WriterText().WriteLine("now is the time");
	
	DEBUG_CLEAR_MEM_CHECK_POINTS();
	p.CheckMem();	
	DEBUG_DUMP_MEM_LEAKS();
	UNIT_ASSERT_MEM_NOTED("Pipe 1.1");

	StringPtr line(p->ReaderText().ReadLine());
	DEBUG_CLEAR_MEM_CHECK_POINTS();
	p.CheckMem();
	line.CheckMem();
	DEBUG_DUMP_MEM_LEAKS();
	UNIT_ASSERT_MEM_NOTED("Pipe 1.2");

	UNIT_ASSERT("lines don't match", line->Equals("now is the time"));

	Log::SWriteOkFail( "Pipe 1" );
}


void _TestPipe()
{
	_TestPipe1();
	DEBUG_CLEAR_MEM_CHECK_POINTS();
	DEBUG_DUMP_MEM_LEAKS();
}

#endif
