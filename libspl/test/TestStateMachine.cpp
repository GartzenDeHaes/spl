/*
 *   This file is part of the Standard Portable Library (SPL).
 *
 *   SPL is free software: you can redistribute it and/or modify
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
#include <spl/io/File.h>
#include <spl/io/log/Log.h>
#include <spl/math/StateMachine.h>

using namespace spl;

#ifdef DEBUG

class _SmTest2 : public IStateEventListener
{
public:
	String m_enter;
	String m_leave;
	String m_trans;
			
	_SmTest2();
	~_SmTest2();
		
	virtual void OnStateEnter( const String &input, State *to, State *from );
	virtual void OnStateLeave( const String &input, State *to, State *from );
	virtual void OnStateTransition( const String &input, Transition *trans );
	
	virtual void ValidateMem() const;
	virtual void CheckMem() const;
};

_SmTest2::_SmTest2()
: m_enter(), m_leave(), m_trans()
{
}

_SmTest2::~_SmTest2()
{
}

void _SmTest2::OnStateEnter( const String &input, State *to, State *from )
{
	m_enter.Set( input );
}

void _SmTest2::OnStateLeave( const String &input, State *to, State *from )
{
	m_leave.Set( input );
}

void _SmTest2::OnStateTransition( const String &input, Transition *trans )
{
	m_trans.Set( input );
}

void _SmTest2::ValidateMem() const
{
}

void _SmTest2::CheckMem() const
{
	m_enter.CheckMem();
	m_leave.CheckMem();
	m_trans.CheckMem();
}

static void _TestStateMachine1()
{
	StateMachine *sm = new StateMachine();
	
	DEBUG_CLEAR_MEM_CHECK_POINTS();
	DEBUG_NOTE_MEM(sm);
	sm->CheckMem();
	DEBUG_DUMP_MEM_LEAKS();
	UNIT_ASSERT_MEM_NOTED("StateMachine test 1.0");

	delete sm;
	
	DEBUG_CLEAR_MEM_CHECK_POINTS();
	DEBUG_DUMP_MEM_LEAKS();
	UNIT_ASSERT_MEM_NOTED("StateMachine test 1.1");

	Log::SWriteOkFail( "StateMachine test 1" );
}

static void _TestStateMachine2()
{
	StateMachine sm;
	_SmTest2 *listener = new _SmTest2();
	
	sm.GetResolver().Add("SmTest", listener);
	sm.ValidateMem();
	sm.DefineState("start", "SmTest", "SmTest");
	sm.DefineState("end", "SmTest", "SmTest");
	sm.DefineTransition("one", "start", "end", "SmTest");
	sm.DefineTransition("*", "end", "start", "SmTest");		// lamda
	sm.SetState("start");
	
	sm.ChangeState( "one" );
	
	UNIT_ASSERT("enter", listener->m_enter.Equals("one"));
	UNIT_ASSERT("leave", listener->m_leave.Equals("one"));
	UNIT_ASSERT("trans", listener->m_trans.Equals("one"));

	sm.ChangeState( "lamda" );
	
	UNIT_ASSERT("enter", listener->m_enter.Equals("lamda"));
	UNIT_ASSERT("leave", listener->m_leave.Equals("lamda"));
	UNIT_ASSERT("trans", listener->m_trans.Equals("lamda"));

	DEBUG_CLEAR_MEM_CHECK_POINTS();
	sm.CheckMem();
	DEBUG_DUMP_MEM_LEAKS();
	UNIT_ASSERT_MEM_NOTED("StateMachine test 2.0");

	delete listener;
	
	Log::SWriteOkFail( "StateMachine test 2" );
}

static void _TestStateMachine3()
{
	StateMachine sm;
	_SmTest2 *listener = new _SmTest2();
	sm.GetResolver().Add("SmTest", listener);
/*	if ( File::Exists("test/StateMachine1.xml") )
	{
		sm.Load( "test/StateMachine1.xml" );
	}
	else if ( File::Exists("../test/StateMachine1.xml") )
	{
		sm.Load( "../test/StateMachine1.xml" );
	}
	else if ( File::Exists("../../test/StateMachine1.xml") )
	{
		sm.Load( "../../test/StateMachine1.xml" );
	}
	else if ( File::Exists("../../spl/test/StateMachine1.xml") )
	{
		sm.Load( "../../spl/test/StateMachine1.xml" );
	}
	else
	{
		sm.Load( "StateMachine1.xml" );
	}

	sm.SetState("start");
	sm.ChangeState( "one" );
	
	UNIT_ASSERT("enter", listener->m_enter.Equals("one"));
	UNIT_ASSERT("leave", listener->m_leave.Equals("one"));
	UNIT_ASSERT("trans", listener->m_trans.Equals("one"));

	sm.ChangeState( "lamda" );
	
	UNIT_ASSERT("enter", listener->m_enter.Equals("lamda"));
	UNIT_ASSERT("leave", listener->m_leave.Equals("lamda"));
	UNIT_ASSERT("trans", listener->m_trans.Equals("lamda"));
*/
	DEBUG_CLEAR_MEM_CHECK_POINTS();
	sm.CheckMem();
	DEBUG_DUMP_MEM_LEAKS();
	UNIT_ASSERT_MEM_NOTED("StateMachine test 3.0");

	delete listener;
	
	Log::SWriteOkFail( "StateMachine test 3" );
}

void _TestStateMachine()
{
	_TestStateMachine1();
	DEBUG_CLEAR_MEM_CHECK_POINTS();
	DEBUG_DUMP_MEM_LEAKS();

	_TestStateMachine2();
	DEBUG_CLEAR_MEM_CHECK_POINTS();
	DEBUG_DUMP_MEM_LEAKS();

	_TestStateMachine3();
	DEBUG_CLEAR_MEM_CHECK_POINTS();
	DEBUG_DUMP_MEM_LEAKS();
}
 
#endif
 
