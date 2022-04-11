#include <spl/Debug.h>
#include <spl/io/log/Log.h>
#include <spl/text/StringTokenizer.h>

#ifdef DEBUG

using namespace spl;

static void _TestStringTokenizer1()
{
	StringTokenizer st(String(" the time\thas\ncome\r,said "));

	DEBUG_CLEAR_MEM_CHECK_POINTS();
	st.CheckMem();
	DEBUG_DUMP_MEM_LEAKS();
	UNIT_ASSERT_MEM_NOTED("StringTokenizer test 1.0");

	String tok;
	
	UNIT_ASSERT("HasMoreTokens", st.HasMoreTokens());
	st.NextToken(tok);
	UNIT_ASSERT("the", tok == "the");

	DEBUG_CLEAR_MEM_CHECK_POINTS();
	st.CheckMem();
	tok.CheckMem();
	DEBUG_DUMP_MEM_LEAKS();
	UNIT_ASSERT_MEM_NOTED("StringTokenizer test 1.1");

	UNIT_ASSERT("HasMoreTokens", st.HasMoreTokens());
	st.NextToken(tok);
	UNIT_ASSERT("time", tok == "time");

	UNIT_ASSERT("HasMoreTokens", st.HasMoreTokens());
	st.NextToken(tok);
	UNIT_ASSERT("has", tok == "has");

	UNIT_ASSERT("HasMoreTokens", st.HasMoreTokens());
	st.NextToken(tok);
	UNIT_ASSERT("come", tok == "come");

	UNIT_ASSERT("HasMoreTokens", st.HasMoreTokens());
	st.NextToken(tok);
	UNIT_ASSERT(",said", tok == ",said");

	UNIT_ASSERT("! HasMoreTokens", !st.HasMoreTokens());

	Log::SWriteOkFail( "StringTokenizer test 1" );
}

void _TestStringTokenizer()
{
	_TestStringTokenizer1();
	DEBUG_CLEAR_MEM_CHECK_POINTS();
	DEBUG_DUMP_MEM_LEAKS();
}

#endif
