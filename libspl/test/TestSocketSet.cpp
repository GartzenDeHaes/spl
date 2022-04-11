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
#include <spl/Debug.h>
#include <spl/net/PortListener.h>
#include <spl/net/SocketSet.h>
#include <spl/threading/Thread.h>
#include <spl/io/log/Log.h>

using namespace spl;

#ifdef DEBUG
static volatile int _debug_msgcount;

class ReflectorStListener;
typedef RefCountPtrCast<ReflectorStListener, IStreamReadListener, IStreamReadListenerPtr> ReflectorStListenerPtr;

class ReflectorStListener : public IStreamReadListener
{
private:
	SocketSet *m_ss;

public:
	ReflectorStListener(SocketSet *ss)
	: m_ss(ss)
	{
	}

	virtual void IStreamRead_OnRead( const Array<byte>& buf, int len )
	{
		_debug_msgcount++;
		m_ss->Broadcast( buf, len );
	}

	virtual void IStreamRead_OnClose()
	{
	}

	virtual void IStreamRead_OnError( const String& msg )
	{
		UNIT_ASSERT(msg.GetChars(), false);
	}

#if defined(DEBUG) || defined(_DEBUG)
	void CheckMem() const
	{
	}
	void ValidateMem() const
	{
	}
#endif
};

class ReflectorStServer : public IPortListenerListener
{
protected:
	SocketSet m_ss;
	PortListener m_plistener;

public:
	inline ReflectorStServer() 
	: m_plistener(8011), m_ss()
	{
		m_plistener.Delegates().Add(this);
	}

	inline ~ReflectorStServer()
	{
		m_ss.CloseAndDelete();
		m_plistener.Stop();
	}

	inline void Start()
	{
		m_plistener.Start();
	}

	virtual void IPortListener_OnConnect( TcpSocketPtr sock )
	{
		Array<byte> buf((const byte *)"HI\n", 4);
		sock->SetLingerOn();
		sock->SetBlocking();
		m_ss.AddSocket( ReflectorStListenerPtr(new ReflectorStListener(&m_ss)), sock );
		sock->GetStream()->Write( buf, 0, 4 );
	}

	virtual void IPortListener_OnStop()
	{
		m_ss.CloseAndDelete();
	}

#if defined(DEBUG) || defined(_DEBUG)
	void CheckMem() const
	{
		m_plistener.CheckMem();
		m_ss.CheckMem();
	}
	void ValidateMem() const
	{
		m_plistener.ValidateMem();
		m_ss.ValidateMem();
	}
#endif
};

static void testSS1()
{
	DEBUG_CLEAR_MEM_CHECK_POINTS();
	UNIT_ASSERT_MEM_NOTED("testSS1");

	_debug_msgcount = 0;
	Array<byte> buf(255);
	ReflectorStServer srv;

	DEBUG_CLEAR_MEM_CHECK_POINTS();
	srv.CheckMem();
	buf.CheckMem();
	DEBUG_DUMP_MEM_LEAKS();
	UNIT_ASSERT_MEM_NOTED("testSS1");

	srv.Start();
	Thread::YYield();

	DEBUG_CLEAR_MEM_CHECK_POINTS();
	srv.CheckMem();
	buf.CheckMem();
	DEBUG_DUMP_MEM_LEAKS();
	UNIT_ASSERT_MEM_NOTED("testSS1");

	TcpSocket cnt( "127.0.0.1", 8011 );
	cnt.SetBlocking();
	cnt.Connect();

	Thread::YYield();
	DEBUG_CLEAR_MEM_CHECK_POINTS();
	srv.CheckMem();
	buf.CheckMem();
	DEBUG_DUMP_MEM_LEAKS();
	UNIT_ASSERT_MEM_NOTED("socketTest");
	int count = cnt.GetStream()->Read( buf, 0, 255 );

	Thread::YYield();
	DEBUG_CLEAR_MEM_CHECK_POINTS();
	srv.CheckMem();
	buf.CheckMem();
	DEBUG_DUMP_MEM_LEAKS();
	UNIT_ASSERT_MEM_NOTED("socketTest");

	UNIT_TEST( "socketTest recv H", buf[0] == 'H' );
	UNIT_TEST( "socketTest recv I", buf[1] == 'I' );

	buf[0] = 'Q';
	buf[1] = '\0';
	cnt.GetStream()->Write( buf, 0, 2 );
	Thread::YYield();
	DEBUG_CLEAR_MEM_CHECK_POINTS();
	srv.CheckMem();
	buf.CheckMem();
	DEBUG_DUMP_MEM_LEAKS();
	UNIT_ASSERT_MEM_NOTED("socketTest");

	buf[0] = 'A';
	count = cnt.GetStream()->Read( buf, 0, 255 );
	UNIT_TEST( "socketTest recv Q", buf[0] == 'Q' );
	UNIT_TEST( "socketTest msg count", _debug_msgcount == 1 );

	Thread::YYield();
	DEBUG_CLEAR_MEM_CHECK_POINTS();
	srv.CheckMem();
	buf.CheckMem();
	DEBUG_DUMP_MEM_LEAKS();
	UNIT_ASSERT_MEM_NOTED("socketTest");

	cnt.Close();
	Thread::YYield();
	DEBUG_CLEAR_MEM_CHECK_POINTS();
	Thread::YYield();
	srv.CheckMem();
	buf.CheckMem();
	DEBUG_DUMP_MEM_LEAKS();
	UNIT_ASSERT_MEM_NOTED("socketTest");
	srv.IPortListener_OnStop();

	Thread::YYield();
	DEBUG_CLEAR_MEM_CHECK_POINTS();
	buf.CheckMem();
	srv.CheckMem();
	UNIT_ASSERT_MEM_NOTED("socketTest");
}

static void testSS2()
{
	_debug_msgcount = 0;
	Array<byte> buf(255);
	ReflectorStServer srv;
	srv.Start();
	Thread::YYield();

	DEBUG_CLEAR_MEM_CHECK_POINTS();
	srv.CheckMem();
	buf.CheckMem();
	UNIT_ASSERT_MEM_NOTED("socketTest2");

	TcpSocket cnt( "127.0.0.1", 8011 );
	cnt.SetBlocking();
	cnt.Connect();
	int count = cnt.GetStream()->Read( buf, 0, 255 );

	Thread::YYield();
	DEBUG_CLEAR_MEM_CHECK_POINTS();
	srv.CheckMem();
	buf.CheckMem();
	UNIT_ASSERT_MEM_NOTED("socketTest2");

	UNIT_TEST( "socketTest2 H", buf[0] == 'H' );
	UNIT_TEST( "socketTest2 I", buf[1] == 'I' );

	TcpSocket cnt2( "127.0.0.1", 8011 );
	cnt2.SetBlocking();
	cnt.Connect();
	buf[0] = 'X';
	count = cnt2.GetStream()->Read( buf, 0, 255 );

	UNIT_TEST( "socketTest2a H", buf[0] == 'H' );
	UNIT_TEST( "socketTest2a I", buf[1] == 'I' );

	DEBUG_CLEAR_MEM_CHECK_POINTS();
	srv.CheckMem();
	buf.CheckMem();
	UNIT_ASSERT_MEM_NOTED("socketTest2");

	buf[0] = 'Q';
	buf[1] = '\0';
	cnt.GetStream()->Write( buf, 0, 2 );
	buf[0] = 'A';
	count = cnt.GetStream()->Read( buf, 0, 255 );
	UNIT_TEST( "socketTest2 Q", buf[0] == 'Q' );
	UNIT_TEST( "socketTest2 msg count", _debug_msgcount == 1 );

	DEBUG_CLEAR_MEM_CHECK_POINTS();
	srv.CheckMem();
	buf.CheckMem();
	UNIT_ASSERT_MEM_NOTED("socketTest2");

	buf[0] = 'Q';
	buf[1] = '\0';
	cnt2.GetStream()->Write( buf, 0, 2 );
	buf[0] = 'A';
	count = cnt2.GetStream()->Read( buf, 0, 255 );
	UNIT_TEST( "socketTest2a Q", buf[0] == 'Q' );
	UNIT_TEST( "socketTest2a msg count", _debug_msgcount == 2 );

	cnt2.Close();

	DEBUG_CLEAR_MEM_CHECK_POINTS();
	srv.CheckMem();
	buf.CheckMem();
	UNIT_ASSERT_MEM_NOTED("socketTest2");

	buf[0] = 'Q';
	buf[1] = '\0';
	cnt.GetStream()->Write( buf, 0, 2 );
	buf[0] = 'A';
	count = cnt.GetStream()->Read( buf, 0, 255 );
	UNIT_TEST( "socketTest2b Q", buf[0] == 'Q' );
	UNIT_TEST( "socketTest2b msg count", _debug_msgcount == 3 );

	Thread::YYield();
	cnt.Close();
	Thread::YYield();

	DEBUG_CLEAR_MEM_CHECK_POINTS();
	srv.CheckMem();
	buf.CheckMem();
	UNIT_ASSERT_MEM_NOTED("socketTest2");

	srv.IPortListener_OnStop();

	DEBUG_CLEAR_MEM_CHECK_POINTS();
	srv.CheckMem();
	buf.CheckMem();
	UNIT_ASSERT_MEM_NOTED("socketTest2");
}

void socketSetTestHarness()
{
	testSS1();
	testSS2();
}
#endif
