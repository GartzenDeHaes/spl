#include <spl/Debug.h>
#include <spl/io/log/Log.h>
//#include <spl/term/TermCap.h>

#ifdef DEBUG

static void _TestTermCap1()
{
	//Log::WriteCheck( "TermCap test 1" );

	//TermCap cp;

	//Log::WriteOkFail(  );
}


void _TestTermCap()
{
	_TestTermCap1();
	DEBUG_CLEAR_MEM_CHECK_POINTS();
	DEBUG_DUMP_MEM_LEAKS();
}

#endif
