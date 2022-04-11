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
#ifdef _WIN32
#include <spl/configwin32.h>
#else
#include <spl/autoconf/config.h>
#endif
#include <stdarg.h>
//#include <varargs.h>
#include <spl/term/Telnet.h>
#include <spl/io/log/Log.h>

using namespace spl;

static int indexofbytefromWithLen( const byte *str, const byte ch, int start, const int len )
{
	int pos;

	for ( pos = start; pos < len; pos++ )
	{
		if ( str[pos] == ch )
		{
			return pos;
		}
	}
	return -1;
}

OptionState::OptionState() { m_state = UNSET; }
OptionState::~OptionState() {}
char *OptionState::ToString(char *buf)
{
	switch( m_state )
	{
	case DO: strcpy(buf, "DO"); break;
	case DONT: strcpy(buf, "DONT"); break;
	case WILL: strcpy(buf, "WILL"); break;
	case WONT: strcpy(buf, "WONT"); break;
	default: strcpy(buf, "???"); break;
	}
	return buf;
}

ITelnetListener::ITelnetListener() {}
ITelnetListener::~ITelnetListener() {}

void doActionTermType(Telnet *tn, byte option)
{
	tn->SetWill(TN_TERM_TYPE);
}

void sbSendActionTermType(Telnet *tn, byte option)
{
	const char *termname = tn->m_listener->TelnetGetTeminalName().GetChars();
	tn->SendSB(TN_TERM_TYPE, TN_SB_IS, termname, (int)strlen(termname));
}

void doActionNAWS(Telnet *tn, byte option)
{
	int width, height;
	tn->m_listener->TelnetGetWindowSize(&width, &height);
	tn->SendIAC(TN_SB, TN_NAWS, 7, TN_SB_IS, (byte)(width>>8), (byte)(width&0xFF), (byte)(height>>8), (byte)(height&0xFF), TN_IAC, TN_SE );
}

void Telnet::Init()
{
	memset(m_received, 0, sizeof(OptionState) * 256);
	memset(m_sent, 0, sizeof(OptionState) * 256);

	memset(m_willAction, 0, sizeof(TNACTION) * 256);
	memset(m_wontAction, 0, sizeof(TNACTION) * 256);
	memset(m_doAction, 0, sizeof(TNACTION) * 256);
	memset(m_dontAction, 0, sizeof(TNACTION) * 256);

	memset(m_sbIsAction, 0, sizeof(TNACTION) * 256);
	memset(m_sbSendAction, 0, sizeof(TNACTION) * 256);

	m_doAction[TN_TERM_TYPE] = doActionTermType;
	m_sbSendAction[TN_TERM_TYPE] = sbSendActionTermType;
	m_doAction[TN_NAWS] = doActionNAWS;

	m_iacbuf[0] = TN_IAC;
}

//Telnet::Telnet( ITelnetListener *listener, TcpSocketPtr sp, SocketSet& ss )
//:	m_listener(listener), 
//	m_termtype(), 
//	m_conn(), 
//	m_notifiedConnect(false), 
//	m_state(TERM_STATE_0), 
//	m_readpump(NULL),
//	m_oneByteBuf(1),
//	m_iacbuf(6)
//{
//	Init();
//
//	if ( sp.IsNotNull() )
//	{
//		SetConnection( sp, ss );
//	}
//}

Telnet::Telnet( ITelnetListener *listener, IStreamPtr conn )
:	m_listener(listener), 
	m_termtype(), 
	m_conn(), 
	m_notifiedConnect(false), 
	m_state(TERM_STATE_0), 
	m_readpump(NULL),
	m_oneByteBuf(1),
	m_iacbuf(6)
{
	Init();

	if ( conn.IsNotNull() )
	{
		SetConnection( conn );
	}
}

Telnet::~Telnet()
{
	if ( NULL != m_readpump )
	{
		m_readpump->Stop();
		m_readpump->Join(30000);
		delete m_readpump;
	}
}

void Telnet::Join()
{
	if ( NULL == m_readpump )
	{
		throw new Exception("Join with socketset instead");
	}
	else
	{
		m_readpump->Join();
	}
}

void Telnet::SetConnection( IStreamPtr conn )
{
	ASSERT( m_conn.IsNotNull() );
	m_conn = conn;
	m_readpump = new StreamReadPump(conn);
	m_readpump->Delegates().Add(this);
}

//void Telnet::SetConnection( TcpSocketPtr sp, SocketSet& ss )
//{
//	ASSERT( m_conn.IsNull() );
//	m_conn = sp->GetStream();
//	ss.AddSocket( this, sp );
//}

void Telnet::IStreamRead_OnClose()
{
	m_listener->OnTelnetClose();
}

void Telnet::IStreamRead_OnError(const String& msg)
{
	m_listener->OnTelnetError(msg);
}

void Telnet::IStreamRead_OnRead( const Array<byte>& data, int len )
{
	ValidateMem();
	if ( len == 0 )
	{
		return;
	}
	int nextiac = indexofbytefromWithLen(data, TN_IAC, 0, len);
	if ( TERM_STATE_0 == m_state && nextiac < 0 )
	{
		ASSERT_PTR(m_listener);
		m_listener->OnTelnetRecv( data, len );
		ValidateMem();
		return;
	}
	for ( int x = 0; x < len; x++ )
	{
		byte b = data[x];
		switch ( m_state )
		{
		case TERM_STATE_0:
			if ( TN_IAC != b )
			{
				if ( 0 > (nextiac = indexofbytefromWithLen(data, TN_IAC, x+1, len)) )
				{
					nextiac = len;
				}
				ASSERT( nextiac - x > 0 );
				m_listener->OnTelnetRecv(&data[x], nextiac - x);
				x = nextiac-1;
				ASSERT(x == (len-1) || TN_IAC == data[x+1]);
				break;
			}
			m_state = TERM_STATE_IAC;
			break;
		case TERM_STATE_IAC:
			switch ( b )
			{
			case TN_IAC:
				m_listener->OnTelnetRecv(&data[x], 1);
				break;
			case TN_SE:
				break;
			case TN_DONT:
				m_state = TERM_STATE_DONT;
				break;
			case TN_DO:
				m_state = TERM_STATE_DO;
				break;
			case TN_WONT:
				m_state = TERM_STATE_WONT;
				break;
			case TN_WILL:
				m_state = TERM_STATE_WILL;
				break;
			case TN_SB:		// SuBnegotiation option
				m_state = TERM_STATE_SB;
				break;
			case TN_DATAMARK:
			case TN_BREAK:
			case TN_IP:		// interrupt
			case TN_AO:		// abort output
			case TN_PING:	// are you there
			case TN_EC:		// erase char
			case TN_EL:		// erase line
			case TN_GA:		// go ahead
				m_state = TERM_STATE_0;
				break;
			default:
				Log::SWriteInfo(String::Format("Unknown IAC command of %hd\n", (short)data[x]));
				break;
			}
			break;
		case TERM_STATE_DO:
			// client requests server to start doing something.
			// should respond with WILL or WONT
			//DEBUG_LOG2("DO %s\n", GetTelnetOptionName(b));
			m_received[b].SetDO();
			if ( NULL != m_doAction[b] )
			{
				m_doAction[b](this, b);
			}
			else
			{
				m_listener->OnTelnetUnmappedOption(TN_DO, b);
			}
			m_listener->OnTelnetStateChange(TN_DO, b);
			m_state = TERM_STATE_0;
			break;
		case TERM_STATE_DONT:
			// client requests server to stop doing something
			// should not repsond
			//DEBUG_LOG2("DONT %s\n", GetTelnetOptionName(b));
			m_received[b].SetDONT();
			if ( NULL != m_dontAction[b] )
			{
				m_dontAction[b](this, b);
			}
			else
			{
				m_listener->OnTelnetUnmappedOption(TN_DONT, b);
			}
			m_listener->OnTelnetStateChange(TN_DONT, b);
			m_state = TERM_STATE_0;
			break;
		case TERM_STATE_WILL:
			// client notifies server it will do something.
			// Resond with DO to do it or DONT to not do it.
			//DEBUG_LOG2("WILL %s\n", GetTelnetOptionName(b));
			m_received[b].SetWILL();
			if ( NULL != m_willAction[b] )
			{
				m_willAction[b](this, b);
			}
			else
			{
				m_listener->OnTelnetUnmappedOption(TN_WILL, b);
			}
			m_listener->OnTelnetStateChange(TN_WILL, b);
			m_state = TERM_STATE_0;
			break;
		case TERM_STATE_WONT:
			// client notifies server it wont do something.
			// no response.
			//DEBUG_LOG2("WONT %s\n", GetTelnetOptionName(b));
			m_received[b].SetWONT();
			if ( NULL != m_wontAction[b] )
			{
				m_wontAction[b](this, b);
			}
			else
			{
				m_listener->OnTelnetUnmappedOption(TN_WONT, b);
			}
			m_listener->OnTelnetStateChange(TN_WONT, b);
			m_state = TERM_STATE_0;
			break;
		case TERM_STATE_SB:
			switch ( data[x] )
			{
			case TN_IAC:
				m_state = TERM_STATE_IAC;
				break;
			case TN_TERM_TYPE:
				m_state = TERM_STATE_SB_TERM_TYPE;
				break;
			case TN_NAWS:
				m_state = TERM_STATE_SB_NAWS;
				break;
			default:
				Log::SWriteWarn(String::Format("Telnet: SB not handled (%s)\n", GetTelnetOptionName(data[x])));
				m_listener->OnTelnetUnmappedOption(TN_SB, data[x]);
				break;
			}
			break;
		case TERM_STATE_SB_TERM_TYPE:
			if( TN_SB_IS == b)
			{
				m_state = TERM_STATE_SB_TERM_TYPE_IS;
			}
			else if( TN_SB_SEND == b )
			{
				m_state = TERM_STATE_SB_TERM_TYPE_SEND;
			}
			else
			{
				Log::SWriteWarn(String::Format("Telnet: IAC SB TERM expected IS or SEND (%d)\n", (int)b));
			}
			break;
		case TERM_STATE_SB_TERM_TYPE_IS:
			if ( TN_IAC == b )
			{
			}
			else if ( TN_SE == b )
			{
				m_state = TERM_STATE_0;
				if ( NULL != m_sbIsAction[TN_TERM_TYPE] )
				{
					m_sbIsAction[TN_TERM_TYPE](this, 0);
				}
				m_termtype.ToLower();
				m_listener->OnTelnetTermName(m_termtype.GetChars());
			}
			else
			{
				m_termtype.Append((char)b);
			}
			break;
		case TERM_STATE_SB_TERM_TYPE_SEND:
			if ( TN_IAC == b )
			{
				m_state = TERM_STATE_SB_SE;
				if ( NULL != m_sbSendAction[TN_TERM_TYPE] )
				{
					m_sbSendAction[TN_TERM_TYPE](this, 0);
				}
			}
			else
			{
				Log::SWriteError(String::Format("Telnet: IAC SB TERM-TYPE expected IAC (%d)\n", (int)b));
				m_state = TERM_STATE_0;
			}
			break;
		case TERM_STATE_SB_NAWS:
			m_naws_minx = (int)b;
			m_state = TERM_STATE_SB_NAWS_MAXX;
			break;
		case TERM_STATE_SB_NAWS_MAXX:
			m_naws_maxx = (int)b;
			m_state = TERM_STATE_SB_NAWS_MINY;
			break;
		case TERM_STATE_SB_NAWS_MINY:
			m_naws_miny = (int)b;
			m_state = TERM_STATE_SB_NAWS_MAXY;
			break;
		case TERM_STATE_SB_NAWS_MAXY:
			if ( TN_IAC == b )
			{
			}
			else if ( TN_SE == b )
			{
				m_listener->OnTelnetSetWindowSize(m_naws_minx, m_naws_maxx, m_naws_miny, m_naws_maxy);
				m_state = TERM_STATE_0;
			}
			else
			{
				m_naws_maxy = (int)b;
			}
			break;
		case TERM_STATE_SB_SE:
			if ( TN_SE != b )
			{
				Log::SWriteError(String::Format("Telnet: IAC SB ... IAC expected SE (%d)\n", (int)b));
			}
			m_state = TERM_STATE_0;
			break;
		default:
			Log::SWriteError(String::Format("Telnet: Unknown state (%d)\n", (int)m_state));
			break;
		}
	}
	if ( ! m_notifiedConnect )
	{
		m_notifiedConnect = true;
		m_listener->OnTelnetConnect();
	}
/*	if ( m_negotiating && m_negotResponsesRemaining <= 0 )
	{
		if ( m_termtype.Length() > 0 )
		{
			m_negotiating = false;
			DumpTermFPrint();
			IndentifyTermCaps();
			m_listener->OnTelnetNegotationComplete();
		}
		else if ( m_receivedWX[TN_NAWS].IsHigh() && m_receivedDX[TN_NAWS].IsLow() )
		{
			// identify dos terminal
			m_negotResponsesRemaining = 1;
		}
	}*/
}

void Telnet::DumpTermFPrint()
{
	int x;
	char buf[256];
	printf ("TERM_TYPE=%s\n", m_termtype.GetChars());

	for ( x = 0; x < 255; x++ )
	{
		if ( m_received[x].IsSet() )
		{
			printf("%s %s\n", m_received[x].ToString(buf), GetTelnetOptionName(x));
		}
	}
}

void Telnet::SetWill( int command )
{
	m_sent[command].SetWILL();
	m_iacbuf[1] = TN_WILL;
	m_iacbuf[2] = command;
	m_conn->Write(m_iacbuf, 0, 3);
}

void Telnet::SetWont( int command )
{
	m_sent[command].SetWONT();
	m_iacbuf[1] = TN_WONT;
	m_iacbuf[2] = command;
	m_conn->Write(m_iacbuf, 0, 3);
}

void Telnet::SetDo( int command )
{
	m_sent[command].SetDO();
	m_iacbuf[1] = TN_DO;
	m_iacbuf[2] = command;
	m_conn->Write(m_iacbuf, 0, 3);
}

void Telnet::SetDont( int command )
{
	m_sent[command].SetDONT();
	m_iacbuf[1] = TN_DONT;
	m_iacbuf[2] = command;
	m_conn->Write(m_iacbuf, 0, 3);
}

void Telnet::SendIAC(byte command, byte option, int len, ...)
{
	va_list marker;
	Array<byte> temp(128);
	temp[0] = TN_IAC;
	temp[1] = command;
	temp[2] = option;

	if ( len > 125 )
	{
		throw new Exception("Telnet::SendIAC: command string too long");
	}
	va_start(marker, len);
	for ( int x = 0; x < len; x++ )
	{
#ifdef _WINDOWS
		temp[x+3] = va_arg(marker, byte);
#else
		temp[x+3] = (byte)va_arg(marker, int);
#endif
	}
	m_conn->Write(temp, 0, len+3);
}

void Telnet::SendSB(byte option, byte isOrSend, StringBuffer *txt)
{
	SendSB(option, isOrSend, txt->GetChars(), txt->Length());
}

void Telnet::SendSB(const byte option, const byte isOrSend, const char *txt, const int len)
{
	Array<byte> buf(256);
	int pos = 0;
	
	buf[pos++] = TN_IAC;
	buf[pos++] = TN_SB;
	buf[pos++] = option;
	buf[pos++] = isOrSend;
	for ( int x = 0; x < len; x++ )
	{
		buf[pos++]  = txt[x];
	}
	buf[pos++] = TN_IAC;
	buf[pos++] = TN_SE;
	m_conn->Write(buf, 0, pos);
}

void Telnet::SendRaw( const byte *buf, const int len )
{
	Array<byte> buf2(buf, len);
	SendRaw(buf2, len);
}

#ifdef DEBUG
void Telnet::ValidateMem() const
{
	m_conn.ValidateMem();
	m_termtype.ValidateMem();
	if ( NULL != m_readpump )
	{
		ASSERT_MEM( m_readpump, sizeof(StreamReadPump) );
		m_readpump->ValidateMem();
	}
	m_oneByteBuf.ValidateMem();
}

void Telnet::CheckMem() const
{
	m_conn.CheckMem();
	m_termtype.CheckMem();
	if ( NULL != m_readpump )
	{
		DEBUG_NOTE_MEM( m_readpump );
		m_readpump->CheckMem();
	}
	m_oneByteBuf.CheckMem();
}
#endif

const char *Telnet::GetTelnetOptionName(byte opt)
{
	if ( opt > 49 )
	{
		if ( opt == 255 )
		{
			return "Extended-Options-List                               [RFC861]";
		}
		return "Unassigned					                        [IANA]";
	}
	return m_telnetOptionNames[opt];
}

char const *Telnet::m_telnetOptionNames[] = {
   /*0     */"Binary Transmission                                 [RFC856]",
   /*1     */"Echo                                                [RFC857]",
   /*2     */"Reconnection                                      [NIC50005]",
   /*3     */"Suppress Go Ahead                                   [RFC858]",
   /*4     */"Approx Message Size Negotiation                   [ETHERNET]",
   /*5     */"Status                                              [RFC859]",
   /*6     */"Timing Mark                                         [RFC860]",
   /*7     */"Remote Controlled Trans and Echo                    [RFC726]",
   /*8     */"Output Line Width                                 [NIC50005]",
   /*9     */"Output Page Size                                  [NIC50005]",
  /*10     */"Output Carriage-Return Disposition                  [RFC652]",
  /*11     */"Output Horizontal Tab Stops                         [RFC653]",
  /*12     */"Output Horizontal Tab Disposition                   [RFC654]",
  /*13     */"Output Formfeed Disposition                         [RFC655]",
  /*14     */"Output Vertical Tabstops                            [RFC656]",
  /*15     */"Output Vertical Tab Disposition                     [RFC657]",
  /*16     */"Output Linefeed Disposition                         [RFC658]",
  /*17     */"Extended ASCII                                      [RFC698]",
  /*18     */"Logout                                              [RFC727]",
  /*19     */"Byte Macro                                          [RFC735]",
  /*20     */"Data Entry Terminal                         [RFC1043,RFC732]",
  /*21     */"SUPDUP                                       [RFC736,RFC734]",
  /*22     */"SUPDUP Output                                       [RFC749]",
  /*23     */"Send Location                                       [RFC779]",
  /*24     */"Terminal Type                                      [RFC1091]",
  /*25     */"End of Record                                       [RFC885]",
  /*26     */"TACACS User Identification                          [RFC927]",
  /*27     */"Output Marking                                      [RFC933]",
  /*28     */"Terminal Location Number                            [RFC946]",
  /*29     */"Telnet 3270 Regime                                 [RFC1041]",
  /*30     */"X.3 PAD                                            [RFC1053]",
  /*31     */"Negotiate About Window Size                        [RFC1073]",
  /*32     */"Terminal Speed                                     [RFC1079]",
  /*33     */"Remote Flow Control                                [RFC1372]",
  /*34     */"Linemode                                           [RFC1184]",
  /*35     */"X Display Location                                 [RFC1096]",
  /*36     */"Environment Option                                 [RFC1408]",
  /*37     */"Authentication Option                              [RFC2941]",
  /*38     */"Encryption Option                                  [RFC2946]",
  /*39     */"New Environment Option                             [RFC1572]",
  /*40     */"TN3270E                                            [RFC1647]",
  /*41     */"XAUTH                                              [Earhart]",
  /*42     */"CHARSET                                            [RFC2066]",
  /*43	  */"Telnet Remote Serial Port (RSP)                      [Barnes]",
  /*44     */"Com Port Control Option                            [RFC2217]",
  /*45     */"Telnet Suppress Local Echo                           [Atmar]",
  /*46     */"Telnet Start TLS                                       [Boe]",
  /*47     */"KERMIT                                             [RFC2840]",
  /*48     */"SEND-URL                                             [Croft]",
  /*49	   */"FORWARD_X					                          [Altman]",
};
