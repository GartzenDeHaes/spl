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
#ifndef _telnetfilter_h
#define _telnetfilter_h

#include <spl/types.h>
#include <spl/Debug.h>

#ifdef _WINDOWS
#include <spl/configwin32.h>
#else
#include <spl/autoconf/config.h>
#endif

#include <spl/Memory.h>
#include <spl/io/Stream.h>
#include <spl/io/StreamReadPump.h>
#include <spl/net/Socket.h>
#include <spl/net/SocketSet.h>
#include <spl/text/StringBuffer.h>
#include <spl/term/acs.h>

namespace spl
{

typedef enum _TelnetCodeState
{
	TERM_STATE_0 = 0,
	TERM_STATE_IAC = 1,		// 255 TELNET command seq
	TERM_STATE_DO = 2,
	TERM_STATE_DONT = 3,
	TERM_STATE_WILL = 4,
	TERM_STATE_WONT = 5,
	TERM_STATE_SB = 6,
	TERM_STATE_SB_TERM_TYPE = 7,
	TERM_STATE_SB_TERM_TYPE_IS = 8,
	TERM_STATE_SB_TERM_TYPE_SEND = 9,
	TERM_STATE_SB_NAWS = 10,
	TERM_STATE_SB_NAWS_MAXX = 11,
	TERM_STATE_SB_NAWS_MINY = 12,
	TERM_STATE_SB_NAWS_MAXY = 13,
	TERM_STATE_SB_SE = 14
} TelnetCodeState;

#define TN_IAC (byte)255

// control
#define TN_DONT (byte)254
#define TN_DO (byte)253
#define TN_WONT (byte)252
#define TN_WILL (byte)251
#define TN_SB (byte)250		// SuBnegotiation option
#define TN_SE (byte)240		// Subnegotiation End 
#define TN_NOP (byte)241
#define TN_DATAMARK (byte)242
#define TN_BREAK (byte)243
#define TN_IP (byte)244		// interrupt
#define TN_AO (byte)245		// abort output
#define TN_PING (byte)246	// are you there
#define TN_EC (byte)247		// erase char
#define TN_EL (byte)248		// erase line
#define TN_GA (byte)249		// go ahead

#define TN_SB_SEND (byte)1
#define TN_SB_IS (byte)0

// options
#define TN_BINARY (byte)0
#define TN_ECHO (byte)1
#define TN_RECONNECT (byte)2
#define TN_STOP_GA (byte)3		// stop go ahead
#define TN_MSG_SIZE_NEG (byte)4
#define TN_STATUS (byte)5
#define TN_TIMING (byte)6
#define TN_REMOTE_CTL_TRANS_ECHO (byte)7
#define TN_OUT_LINE_WIDTH (byte)8
#define TN_OUT_PAGE_SIZE (byte)9
#define TN_NAOCRD (byte)10
#define TN_NAOHTS (byte)11
#define TN_NAOHTD (byte)12
#define TN_NAOFFD (byte)13
#define TN_NAOVTS (byte)14
#define TN_NAOVTD (byte)15
#define TN_NAOLFD (byte)16
#define TN_EXTEND_ASCII (byte)17
#define TN_LOGOUT (byte)18
#define TN_BM (byte)19			// byte macro
#define TN_DTE (byte)20			// data entry terminal
#define TN_SUPDUP (byte)21
#define TN_SUPDUP_OUT (byte)22
#define TN_SEND_LOC (byte)23
#define TN_TERM_TYPE (byte)24
#define TN_EOR (byte)25
#define TN_USERID (byte)26
#define TN_OUT_MARK (byte)27
#define TN_TTYLOC (byte)28
#define TN_VT3270 (byte)29
#define TN_X3PAD (byte)30
#define TN_NAWS (byte)31		// negotiate about window size
#define TN_TERM_SPEED (byte)32
#define TN_REMOTE_FLOW_CTL (byte)33
#define TN_LINE_MODE (byte)34
#define TN_X_LOC (byte)35
#define TN_ENV_OPT (byte)36
#define TN_AUTH (byte)37
#define TN_ENCRYPT (byte)38
#define TN_NEW_ENV_OPT (byte)39
#define TN_XAUTH (byte)40
#define TN_CHARSET (byte)41
#define TN_SUPPRESS_LOCAL_ECHO (byte)45
#define TN_EXENDS_OPTION_LIST (byte)255

class OptionState
{
public:
	OptionState();
	~OptionState();
	inline byte GetState() { return m_state; }
	inline void SetState(byte val) { m_state = val; }
	inline bool IsDO() { return m_state == DO; }
	inline bool IsDONT() { return m_state == DONT; }
	inline bool IsWILL() { return m_state == WILL; }
	inline bool IsWONT() { return m_state == WONT; }
	inline void SetDO() { m_state = DO; }
	inline void SetDONT() { m_state = DONT; }
	inline void SetWILL() { m_state = WILL; }
	inline void SetWONT() { m_state = WONT; }
	inline bool IsSet() { return m_state != UNSET; }
	inline void Clear() { m_state = UNSET; }

	enum {
		DO = 1,
		DONT = 2,
		WILL = 3,
		WONT = 4,
		UNSET = 0
	};

	char *ToString(char *buf);

protected:
	byte m_state;
};

class Telnet;
typedef void (*TNACTION)(Telnet *tn, byte option);

class ITelnetListener
{
public:
	// This ensures that the implemntation class destructor is called
	ITelnetListener();
	virtual ~ITelnetListener();

	virtual void OnTelnetRecv( const byte *data, int len ) = 0;
	virtual void OnTelnetConnect() = 0;
	virtual void OnTelnetClose() = 0;
	virtual void OnTelnetError(const String& msg) = 0;
	virtual void OnTelnetUnmappedOption(int command, int option) = 0;
	virtual void OnTelnetSetWindowSize(int minx, int maxx, int miny, int maxy) = 0;
	virtual void TelnetGetWindowSize(int *widthInChars, int *heightInChars) = 0;
	virtual void OnTelnetTermName(const String& termname) = 0;
	virtual const String& TelnetGetTeminalName() = 0;
	virtual void OnTelnetStateChange(int command, int option) = 0;
};

class Telnet : public IStreamReadListener
{
private:
	// forbid copy constructor
	inline Telnet(const Telnet& t) {}
	inline void operator =(const Telnet& t) {}

protected:
	spl::IStreamPtr m_conn;
	StreamReadPump *m_readpump;
	Array<byte> m_oneByteBuf;

	int m_state;
	Array<byte> m_iacbuf;
	StringBuffer m_termtype;
	bool m_notifiedConnect;

	int m_naws_minx;
	int m_naws_maxx;
	int m_naws_miny;
	int m_naws_maxy;

	/**
	 * What IAC request's have we received already ?
	 */
	OptionState m_received[256];
	
	/**
	 * What IAC request's do we have sent already ?
	 */
	OptionState m_sent[256];

	TNACTION m_willAction[256];
	TNACTION m_wontAction[256];
	TNACTION m_doAction[256];
	TNACTION m_dontAction[256];

	TNACTION m_sbIsAction[256];
	TNACTION m_sbSendAction[256];

	void DumpTermFPrint();

	friend void doActionTermType(Telnet *tn, byte option);
	friend void doActionNAWS(Telnet *tn, byte option);
	friend void sbSendActionTermType(Telnet *tn, byte option);

	static const char *m_telnetOptionNames[];

	virtual void IStreamRead_OnRead(const Array<byte>& buf, int len);
	virtual void IStreamRead_OnError(const String& msg);
	virtual void IStreamRead_OnClose();

	void Init();

public:
	ITelnetListener *m_listener;

	Telnet( ITelnetListener *listener, spl::IStreamPtr conn );
	//Telnet( ITelnetListener *listener, TcpSocketPtr sock, SocketSet& ss );
	virtual ~Telnet();

	void SetConnection( spl::IStreamPtr conn );
	//void SetConnection( TcpSocketPtr sp, SocketSet& ss );

	inline void SetDoAction( TNACTION fn, byte option ) { m_doAction[option] = fn; }
	inline void SetDontAction( TNACTION fn, byte option ) { m_dontAction[option] = fn; }
	inline void SetWillAction( TNACTION fn, byte option ) { m_willAction[option] = fn; }
	inline void SetWontAction( TNACTION fn, byte option ) { m_wontAction[option] = fn; }

	void SetWill( int command );
	void SetWont( int command );
	void SetDo( int command );
	void SetDont( int command );

	inline OptionState ReceivedState(int option) { return m_received[option]; }
	
	inline void SetBinaryModeWill() { SetWill(TN_BINARY); }
	inline void SetBinaryModeWont() { SetWont(TN_BINARY); }
	inline void SetBinaryModeDo() { SetDo(TN_BINARY); }
	
	inline void SetEchoWill() { SetWill(TN_ECHO); }
	inline void SetEchoWont() { SetWont(TN_ECHO); }
	inline void SetEchoDo() { SetDo(TN_ECHO); }
	inline void SetEchoDont() { SetDont(TN_ECHO); }

	inline void SetStopGaWill() { SetWill(TN_STOP_GA); }
	inline void SetStopGaWont() { SetWont(TN_STOP_GA); }
	inline void SetStopGaDo() { SetDo(TN_STOP_GA); }
	inline void SetStopGaDont() { SetDont(TN_STOP_GA); }

	inline void SetTermTypeDo() { SetDo(TN_TERM_TYPE); }
	inline void SetTermSpeedDo() { SetDo(TN_TERM_SPEED); }
	inline void SetDteDo() { SetDo(TN_DTE); }
	inline void SetNawsDo() { SetDo(TN_NAWS); }
	inline void SetNawsWill() { SetWill(TN_NAWS); }
	inline void SetStatusDo() { SetDo(TN_STATUS); }
	inline void SetCrNegDo() { SetWill(TN_NAOCRD); }
	inline void SetLineModeWill() { SetWill(TN_LINE_MODE); }
	inline void SetLineModeDo() { SetDo(TN_LINE_MODE); }

	inline void SendRequestTermName() { SendSB(TN_TERM_TYPE, TN_SB_SEND, "", 0); }
	inline StringPtr TerminalName() { return m_termtype.ToString(); }

	inline spl::IStreamPtr GetConnection() { return m_conn; }
	void Join();

	void SendIAC(const byte command, const byte option, const int len, ...);
	void SendSB(const byte option, const byte isOrSend, StringBuffer *txt);
	void SendSB(byte option, const byte isOrSend, const char *txt, const int len);
	inline void SendRaw( const Array<byte>& buf, const int len ) { m_conn->Write( buf, 0, len ); }
	inline void SendRaw( const byte buf ) { m_oneByteBuf[0] = buf; m_conn->Write( m_oneByteBuf, 0, 1 ); }
	void SendRaw( const byte *buf, const int len );
	const char *GetTelnetOptionName(byte opt);

#ifdef DEBUG
	virtual void ValidateMem() const;
	virtual void CheckMem() const;
#endif
};

/** @} */
}

#endif
