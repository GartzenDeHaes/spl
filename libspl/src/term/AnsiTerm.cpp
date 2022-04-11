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
#include <ctype.h>
#include <spl/io/log/Log.h>
#include <spl/term/AnsiTerm.h>
#include <spl/Int32.h>

using namespace spl;

static byte TERM_MODEANSI[] = {ESC, 'c', ESC, '[', '5', 'q'};
#define TERM_MODEANSI_LEN 6

static byte ANSI_QUERY_DEVICE_CODE[] = {ESC, '[', 'c'};
#define ANSI_QUERY_DEVICE_CODE_LEN 3

static byte ANSI_CLEAR_SCREEN[] = {ESC, '[', '3', '2', ';', '4', '0', 'm', ESC, '[', '2', 'J'};
#define ANSI_CLEAR_SCREEN_LEN 12

static byte ANSI_LINE_WRAP_OFF[] = {ESC, '[', '?', '7', 'l'};
#define ANSI_LINE_WRAP_OFF_LEN 5

static byte ANSI_SAVE_CURSOR[] = {ESC, '[', 's'};
#define ANSI_SAVE_CURSOR_LEN 3

static byte ANSI_RESTORE_CURSOR[] = {ESC, '[', 'u'};
#define ANSI_RESTORE_CURSOR_LEN 3

static byte ANSI_SAVE_CURSOR_ATTR[] = {ESC, '7'};
#define ANSI_SAVE_CURSOR_ATTR_LEN 2

static byte ANSI_RESTORE_CURSOR_ATTR[] = {ESC, '8'};
#define ANSI_RESTORE_CURSOR_ATTR_LEN 2

static byte ANSI_QUERY_CURSOR[] = {ESC, '[', '6', 'n'};
#define ANSI_QUERY_CURSOR_LEN 4

static byte ANSI_CHAR_B0[] = {ESC, '(', 'B'};
#define ANSI_CHAR_B0_LEN 3

static byte ANSI_CHAR_B1[] = {ESC, ')', 'B'};
#define ANSI_CHAR_B1_LEN 3

static byte ANSI_CHAR_G1[] = {ESC, ')', '0'};
#define ANSI_CHAR_G1_LEN 3

static byte ANSI_CHAR_G0[] = {ESC, '(', '0'};
#define ANSI_CHAR_G0_LEN 3

static byte ANSI_AS1[] = {(char)0xe, (char)0x4 };
#define ANSI_S1_LEN 2

static byte ANSI_AS0[] = {(char)0xf, (char)0x4 };
#define ANSI_S0_LEN 2

ITerminal::ITerminal() 
{
}
ITerminal::~ITerminal() 
{
}

void AnsiTerm::Init(ITermListener *listener)
{
	m_screenMode = SCREEN_MODE_COLOR80X25;
	m_state = STATE_ASCII;
	m_buf8Csi[0] = ESC;
	m_buf8Csi[1] = CSI;

	if ( NULL != listener )
	{
		m_listeners.Add( listener );
	}

	m_caps.SetTerm("pcansi");
}

void AnsiTerm::InitTelnet()
{
	m_telnet->SetDo(TN_STOP_GA);
	m_telnet->SetWill(TN_STOP_GA);
	m_telnet->SendRaw(TERM_MODEANSI, TERM_MODEANSI_LEN);
	m_telnet->SendRaw(ANSI_QUERY_DEVICE_CODE, ANSI_QUERY_DEVICE_CODE_LEN);
	m_telnet->SetTermTypeDo();
	SendTextModeColor80X25();
	m_telnet->SendRequestTermName();
}

//AnsiTerm::AnsiTerm(TcpSocketPtr sock, SocketSet& ss, ITermListener *listener) 
//:	m_caps(),
//	m_strArgs(), 
//	m_listeners(), 
//	m_termname("pcansi"),
//	m_numrows(25),
//	m_numcols(80),
//	m_deviceCode(),
//	m_termIdentified(false)
//{
//	Init( listener );
//	m_telnet = new Telnet(this, sock, ss);
//	InitTelnet();
//}

AnsiTerm::AnsiTerm(IStreamPtr conn, ITermListener *listener) 
:	m_caps(),
	m_strArgs(), 
	m_listeners(), 
	m_termname("pcansi"),
	m_numrows(25),
	m_numcols(80),
	m_deviceCode(),
	m_termIdentified(false)
{
	Init( listener );
	m_telnet = new Telnet(this, conn);
	InitTelnet();
}

AnsiTerm::~AnsiTerm()
{
	delete m_telnet;
}
/*
void AnsiTerm::tnNegotationComplete()
{
	DEBUG_CLEAR_MEM_CHECK_POINTS();
	DEBUG_NOTE_MEM_ALLOCATION( this->m_server );
	this->m_server->checkMem();
	DEBUG_DUMP_MEM_LEAKS();
	DEBUG_ASSERT_MEM_NOTED();

	ASSERT_MEM( this, sizeof(AnsiTerm) );
	validateMem();

	//setTermModeAnsi();
	queryTermType();
	setTextModeColor80X25();
	//setTextModeGfx();
	disableLineWrap();
	enableKeyPadTransmit();

	ASSERT_PTR( &m_caps );
	ASSERT_MEM( this, sizeof(AnsiTerm) );
	validateMem();

	if ( m_caps.HasS0S1() != 0 )
	{
		setCharSetS1();
	}

	DEBUG_LOCK_HEAP();
	DEBUG_CLEAR_MEM_CHECK_POINTS();
	DEBUG_NOTE_MEM_ALLOCATION( this->m_server );
	this->m_server->checkMem();
	DEBUG_DUMP_MEM_LEAKS();
	DEBUG_ASSERT_MEM_NOTED();
	DEBUG_UNLOCK_HEAP();
}
*/

void AnsiTerm::OnTelnetConnect()
{
	int count = m_listeners.Count();
	for ( int x = 0; x < count; x++ )
	{
		ITermListener *listener = m_listeners.ElementAt(x);
		ASSERT_PTR( listener );
		listener->OnTermOpen();
	}
}

void AnsiTerm::OnTelnetClose()
{
	int count = m_listeners.Count();
	for ( int x = 0; x < count; x++ )
	{
		ITermListener *listener = m_listeners.ElementAt(x);
		ASSERT_PTR( listener );
		listener->OnTermClose();
	}
}

void AnsiTerm::OnTelnetError(const String& se)
{
	int count = m_listeners.Count();
	for ( int x = 0; x < count; x++ )
	{
		ITermListener *listener = m_listeners.ElementAt(x);
		ASSERT_PTR( listener );
		listener->OnTermError(se);
	}
}

void AnsiTerm::OnTelnetUnmappedOption(int command, int option)
{
}

void AnsiTerm::OnTelnetStateChange(int command, int option)
{
}

void AnsiTerm::OnTelnetTermName(const String& termname) 
{ 
	m_termname = termname;
	int count = m_listeners.Count();
	for ( int x = 0; x < count; x++ )
	{
		ITermListener *listener = m_listeners.ElementAt(x);
		ASSERT_PTR( listener );
		listener->OnTermType(m_termname); 	
	}
}

void AnsiTerm::OnTelnetSetWindowSize(int minx, int maxx, int miny, int maxy)
{
	int count = m_listeners.Count();
	for ( int x = 0; x < count; x++ )
	{
		ITermListener *listener = m_listeners.ElementAt(x);
		ASSERT_PTR( listener );
		listener->OnTermWindowSize(minx, maxx, miny, maxy);
	}
}

void AnsiTerm::DispatchOnTermKey(char ch)
{
	int count = m_listeners.Count();
	for ( int x = 0; x < count; x++ )
	{
		ITermListener *listener = m_listeners.ElementAt(x);
		ASSERT_PTR( listener );
		listener->OnTermKey( ch );
	}
}

void AnsiTerm::DispatchOnTermKey(SpecialKey key)
{
	int count = m_listeners.Count();
	for ( int x = 0; x < count; x++ )
	{
		ITermListener *listener = m_listeners.ElementAt(x);
		ASSERT_PTR( listener );
		listener->OnTermKey( key );
	}
}

void AnsiTerm::DispatchOnTermChanged()
{
	int count = m_listeners.Count();
	for ( int x = 0; x < count; x++ )
	{
		ITermListener *listener = m_listeners.ElementAt(x);
		ASSERT_PTR( listener );
		listener->OnTermChanged(  );
	}
}

/**
 *  Dispatch has placed the input in our parent's
 *	circular buffer.
 */
void AnsiTerm::OnTelnetRecv( const byte *data, int len ) 
{
	byte b;
	for ( int x = 0; x < len; x++ )
	{
		b = data[x];
		switch ( m_state )
		{
		case STATE_ASCII:
			if ( iscntrl(b) )
			{
				switch ( b )
				{
				case ESC:
					m_state = STATE_ESCAPE;
					break;
				case DEL:
					DispatchOnTermKey( SKEY_DEL );
					break;
				case BS:
					DispatchOnTermKey( SKEY_BKSP );
					break;
				case '\t':
					DispatchOnTermKey( SKEY_TAB );
					break;
				case LF:
					DispatchOnTermKey( SKEY_LF );
					break;
				case CR:
					DispatchOnTermKey( SKEY_CR );
					break;
				case 0:
					break;
				default:
					if ( b == 0 )
					{
						//m_listener->OnAnsiKeyCtrl( b + 'A' - 1 );
					}
					break;
				}
			}
			else
			{
				DispatchOnTermKey( (char)b );
			}
			break;
		case STATE_ESCAPE:
			switch ( b )
			{
			case CSI:
				m_state = STATE_CSI;
				break;
			case '(':
				m_state = STATE_G0;
				break;
			case ')':
				m_state = STATE_G1;
				break;
			case 'O':
				m_state = FN_KEY;
				break;
			default:
				Log::SWriteError(String::Format("Invalid ESC input of %hd\n", (short)b));
				m_state = STATE_ASCII;
				break;
			}
			break;
		case STATE_CSI:
			if ( '?' == b )
			{
				m_state = STATE_MODE;
			}
			else if ( '[' == b )
			{
				m_state = FN_KEY_2;
			}
			else if ( isdigit(b) )
			{
				m_strArgs.Append( b );
				m_state = STATE_ARG;
			}
			else if ( 'P' == b )
			{
				// pause/break key;
				DispatchOnTermKey(SKEY_BREAK);
				m_state = STATE_ASCII;
			}
			else if ( 'A' == b )
			{
				// UP
				DispatchOnTermKey(SKEY_UP);
				m_state = STATE_ASCII;
			}
			else if ( 'B' == b )
			{
				// DOWN
				DispatchOnTermKey(SKEY_DOWN);
				m_state = STATE_ASCII;
			}
			else if ( 'C' == b )
			{
				DispatchOnTermKey(SKEY_RIGHT);
				m_state = STATE_ASCII;
			}
			else if ( 'D' == b )
			{
				DispatchOnTermKey(SKEY_LEFT);
				m_state = STATE_ASCII;
			}
			else
			{
				Log::SWriteError(String::Format("Invalid CSI input of %hd\n", (short)b));
				m_state = STATE_ASCII;
			}
			break;
		case STATE_MODE:
		case STATE_ARG:
			if ( ! isdigit(b) && ';' != b )
			{
				ExecCmd( b );
				m_state = STATE_ASCII;
				m_strArgs.Clear();
			}
			else
			{
				m_strArgs.Append( (char)b );
			}
			break;
		case FN_KEY:
			DispatchOnTermKey( (SpecialKey)(SKEY_F1 + (b - 80)) );
			m_state = STATE_ASCII;
			break;
		case FN_KEY_2:
			DispatchOnTermKey( (SpecialKey)(SKEY_F1 + (b - 'A')) );
			m_state = STATE_ASCII;
			break;
		default:
			Log::SWriteError(String::Format("Invalid state in AnsiTerm::processInput %d\n", m_state));
			m_state = STATE_ASCII;
			break;
		}
	}
	DispatchOnTermChanged(  );
}

void AnsiTerm::ExecCmd( char cmd )
{
	StringPtr devicecode;
	switch ( cmd )
	{
	case 'c':
		// device type response
		devicecode = m_strArgs.ToString();
		m_deviceCode.Set( *devicecode );
		break;

	case '~':
		switch ( m_strArgs.ToInt() )
		{
		case 1:
			DispatchOnTermKey( SKEY_HOME );
			break;
		case 2:
			DispatchOnTermKey( SKEY_INS );
			break;
		case 4:
			DispatchOnTermKey( SKEY_END );
			break;
		case 5:
			DispatchOnTermKey( SKEY_PGUP );
			break;
		case 6:
			DispatchOnTermKey( SKEY_PGDN );
			break;
		case 15:
			DispatchOnTermKey( SKEY_F5 );
			break;
		case 17:
			DispatchOnTermKey( SKEY_F6 );
			break;
		case 18:
			DispatchOnTermKey( SKEY_F7 );
			break;
		case 19:
			DispatchOnTermKey( SKEY_F8 );
			break;
		case 20:
			DispatchOnTermKey( SKEY_F9 );
			break;
		case 21:
			DispatchOnTermKey( SKEY_F10 );
			break;
		case 23:
			DispatchOnTermKey( SKEY_F11 );
			break;
		case 24:
			DispatchOnTermKey( SKEY_F12 );
			break;
		}
		break;
	}
}

void AnsiTerm::OnError(const char *msg)
{
}

void AnsiTerm::Send( const byte *buf, const int len )
{
	m_telnet->SendRaw(buf, len);
}

void AnsiTerm::SendDisableLineWrap()
{
	//m_buf8Csi[2] = '=';
	//m_buf8Csi[3] = (int)'0' + 7;
	//m_buf8Csi[4] = 'l';
	//SendRaw(m_buf8Csi, 5);

	m_telnet->SendRaw(ANSI_LINE_WRAP_OFF, ANSI_LINE_WRAP_OFF_LEN);

	//m_buf8Csi[2] = '[';
	//m_buf8Csi[3] = '7';
	//m_buf8Csi[4] = 'l';
	//SendRaw(m_buf8Csi, 5);
}

void AnsiTerm::SendEnableLineWrap()
{
	//m_buf8Csi[2] = '=';
	//m_buf8Csi[3] = (int)'0' + 7;
	//m_buf8Csi[4] = 'h';
	//SendRaw(m_buf8Csi, 5);

	m_buf8Csi[2] = '[';
	m_buf8Csi[3] = '7';
	m_buf8Csi[4] = 'h';
	m_telnet->SendRaw(m_buf8Csi, 5);
}

void AnsiTerm::SendTextModeMono80X25()
{
	m_screenMode = SCREEN_MODE_MONO80X25;
	m_buf8Csi[2] = '=';
	m_buf8Csi[3] = SCREEN_MODE_MONO80X25 + (int)'0';
	m_buf8Csi[4] = 'h';
	m_telnet->SendRaw(m_buf8Csi, 5);
}

void AnsiTerm::SendTextModeColor80X25()
{
	m_screenMode = SCREEN_MODE_COLOR80X25;
	m_buf8Csi[2] = '=';
	m_buf8Csi[3] = (int)'0' + SCREEN_MODE_COLOR80X25;
	m_buf8Csi[4] = 'h';
	m_telnet->SendRaw(m_buf8Csi, 5);
}

void AnsiTerm::SendEnableKeyPadTransmit()
{
	char buf[24];
	if ( m_caps.GetCapString(TCAP_ks, &buf[0], 24) )
	{
		m_telnet->SendRaw( (byte *)&buf[0], (int)strlen(buf) );
	}
}

void AnsiTerm::SendSetG0()
{
	//m_telnet->SendRaw(ANSI_CHAR_G0, ANSI_CHAR_G0_LEN);
}

void AnsiTerm::SendSetG1()
{
	//m_telnet->SendRaw(ANSI_CHAR_G1, ANSI_CHAR_G1_LEN);
}

void AnsiTerm::SendSetCharSetS0()
{
	//m_telnet->SendRaw(ANSI_AS0, ANSI_S0_LEN);
}

void AnsiTerm::SendSetCharSetS1()
{
	//m_telnet->SendRaw(ANSI_AS1, ANSI_S1_LEN);
}

void AnsiTerm::SendClearScreen()
{
	m_telnet->SendRaw(ANSI_CLEAR_SCREEN, ANSI_CLEAR_SCREEN_LEN);
}

void AnsiTerm::SendCharAtaTimeMode()
{
	// WONT ECHO
	m_telnet->SetEchoWill();
	// SUPRESS GO AHEAD
	m_telnet->SetStopGaDo();
	m_telnet->SetStopGaWill();
}

void AnsiTerm::SendLineAtaTimeMode()
{
	m_telnet->SetEchoWont();
}

void AnsiTerm::SendLocalEchoOn()
{
	m_telnet->SetEchoWill();
}

void AnsiTerm::SendLocalEchoOff()
{
	m_telnet->SetEchoWont();
}

void AnsiTerm::SendNAWS()
{
	if ( ! m_telnet->ReceivedState(TN_NAWS).IsDONT() )
	{
		m_telnet->SetNawsDo();
		m_telnet->SetNawsWill();
	}
}

int AnsiTerm::GetNumRows()
{
	return m_numrows;
}

int AnsiTerm::GetNumCols()
{
	return m_numcols;
}

void AnsiTerm::SetCursor(int row, int col)
{
	ASSERT( row >= 0 && col >= 0 );
	StringBuffer buf(80);
	char cmdbuf[80];
	GetTermCaps()->GetCapString(TCAP_cm, &cmdbuf[0], 80);
	GetTermCaps()->FormatGoto(cmdbuf, &buf, col, row);
	Send( (byte *)buf.GetChars(), buf.Length() );
}

void AnsiTerm::SendQueryGetCursor()
{
	m_telnet->SendRaw( ANSI_QUERY_CURSOR, ANSI_QUERY_CURSOR_LEN );
}

void AnsiTerm::SaveCursor()
{
	m_telnet->SendRaw(ANSI_SAVE_CURSOR, ANSI_SAVE_CURSOR_LEN);
}

void AnsiTerm::RestoreCursor()
{
	m_telnet->SendRaw(ANSI_RESTORE_CURSOR, ANSI_RESTORE_CURSOR_LEN);
}

void AnsiTerm::SaveAttributes()
{
	m_telnet->SendRaw(ANSI_SAVE_CURSOR_ATTR, ANSI_SAVE_CURSOR_ATTR_LEN);
}

void AnsiTerm::RestoreAttributes()
{
	m_telnet->SendRaw(ANSI_RESTORE_CURSOR_ATTR, ANSI_RESTORE_CURSOR_ATTR_LEN);
}

void AnsiTerm::SetAttributes( int forecolor, int backcolor, int attr )
{
	bool usesemi = false;
	StringBuffer strbuf(80);
	
	strbuf.Append( ESC );
	strbuf.Append( CSI );
	if ( attr & TEXTATRR_BOLD )
	{
		strbuf.Append(Int32::ToString(TEXTATRR_BOLD));
		usesemi = true;
	}
	if ( attr & TEXTATRR_UNDERSCORE )
	{
		if ( usesemi == true )
		{
			strbuf.Append(';');
		}
		strbuf.Append(Int32::ToString(TEXTATRR_UNDERSCORE));
		usesemi = true;
	}
	if ( attr & TEXTATRR_BLINK )
	{
		if ( usesemi == true )
		{
			strbuf.Append(';');
		}
		strbuf.Append(Int32::ToString(TEXTATRR_BLINK));
		usesemi = true;
	}
	if ( attr & TEXTATRR_REV )
	{
		if ( usesemi == true )
		{
			strbuf.Append(';');
		}
		strbuf.Append(Int32::ToString(TEXTATRR_REV));
		usesemi = true;
	}
	if ( attr & TEXTATRR_INVIS )
	{
		if ( usesemi == true )
		{
			strbuf.Append(';');
		}
		strbuf.Append(Int32::ToString(TEXTATRR_INVIS));
		usesemi = true;
	}
	if ( usesemi == true )
	{
		strbuf.Append(';');
	}
	else
	{
		strbuf.Append("0;");
	}
	strbuf.Append( Int32::ToString(forecolor) );
	strbuf.Append( ';' );
	strbuf.Append( Int32::ToString(backcolor) );
	strbuf.Append( 'm' );

	Send((byte *)strbuf.GetChars(), strbuf.Length());
}

void AnsiTerm::IndentifyTermCaps()
{
	StringPtr terminalName = String(m_telnet->TerminalName()).ToLower();
	if ( *terminalName == "ansi" && m_telnet->ReceivedState(TN_NAWS).IsWILL() )
	{
		if ( TermCap::HasTerminalDef( "pcansi" /*"ansi.sys"*/) )
		{
			m_caps.SetTerm( "pcansi" );
		}
		else
		{
			m_caps.SetTerm( "ansi" );
		}
		ValidateMem();
	}
	else
	{
		if ( TermCap::HasTerminalDef( *terminalName ) )
		{
			m_caps.SetTerm( *terminalName );
		}
		else if ( TermCap::HasTerminalDef(m_telnet->TerminalName()) )
		{
			m_caps.SetTerm( m_telnet->TerminalName() );
		}
		else
		{
			m_caps.SetTerm( "ansi" );
		}
		ValidateMem();
	}
	m_caps.ValidateMem();

	char cmdbuf[80];
	if (m_caps.GetCapString(TCAP_ti, &cmdbuf[0], 80))
	{
		m_telnet->SendRaw((byte *)&cmdbuf[0], (int)strlen(cmdbuf));
	}
	if ( m_caps.GetCapString(TCAP_is, &cmdbuf[0], 80) )
	{
		m_telnet->SendRaw((byte *)&cmdbuf[0], (int)strlen(cmdbuf));
	}
	m_caps.ValidateMem();
	m_termIdentified = true;
}

void AnsiTerm::AddListener( ITermListener *listener ) 
{
	ASSERT_PTR( listener );
#ifdef DEBUG
	int count = m_listeners.Count();
	for ( int x = 0; x < count; x++ )
	{
		ASSERT( listener != m_listeners.ElementAt(x) );
	}
#endif
	m_listeners.Add( listener );
}

TermCap *AnsiTerm::GetTermCaps() 
{ 
	if(! m_termIdentified ) 
		IndentifyTermCaps(); 
	return &m_caps; 
}

IStreamPtr AnsiTerm::GetStream()
{
	return m_telnet->GetConnection();
}

void AnsiTerm::TelnetGetWindowSize(int *widthInChars, int *heightInChars) 
{ 
	*widthInChars = 80; 
	*heightInChars = 25; 
}

const String& AnsiTerm::TelnetGetTeminalName() 
{ 
	return m_termname;
}

#ifdef DEBUG
void AnsiTerm::ValidateMem() const
{
	m_telnet->ValidateMem();
	m_strArgs.ValidateMem();
	m_termname.ValidateMem();
	m_deviceCode.ValidateMem();
	m_caps.ValidateMem();
	m_listeners.ValidateMem();
	int count = m_listeners.Count();
	for ( int x = 0; x < count; x++ )
	{
		ITermListener *listener = m_listeners.ElementAt(x);
		ASSERT_PTR( listener );
	}
}

void AnsiTerm::CheckMem() const
{
	DEBUG_NOTE_MEM( m_telnet );
	m_telnet->CheckMem();
	m_strArgs.CheckMem();
	m_termname.CheckMem();
	m_deviceCode.CheckMem();
	m_caps.CheckMem();
	m_listeners.CheckMem();
}
#endif

