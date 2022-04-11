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
#ifndef _ansiterm_h
#define _ansiterm_h

#include <spl/types.h>
#include <spl/Debug.h>
#include <spl/term/ansicodes.h>
#include <spl/Exception.h>
#include <spl/Memory.h>
#include <spl/io/Stream.h>
#include <spl/term/TermCap.h>
#include <spl/text/StringBuffer.h>
#include <spl/collection/Vector.h>
#include <spl/collection/Hashtable.h>
#include <spl/collection/List.h>
#include <spl/term/Telnet.h>

namespace spl
{

typedef enum _SpecialKey
{
	SKEY_NULL,
	SKEY_HOME,
	SKEY_TAB,
	SKEY_END,
	SKEY_DEL,
	SKEY_PGUP,
	SKEY_PGDN,
	SKEY_INS,
	SKEY_BKSP,
	SKEY_CR,
	SKEY_LF,
	SKEY_UP,
	SKEY_LEFT,
	SKEY_RIGHT,
	SKEY_DOWN,
	SKEY_BREAK,

	SKEY_CTRLA,
	SKEY_CTRLB,
	SKEY_CTRLC,
	SKEY_CTRLD,
	SKEY_CTRLE,
	SKEY_CTRLF,
	SKEY_CTRLG,
	SKEY_CTRLH,
	SKEY_CTRLI,
	SKEY_CTRLJ,
	SKEY_CTRLK,
	SKEY_CTRLL,
	SKEY_CTRLM,
	SKEY_CTRLN,
	SKEY_CTRLO,
	SKEY_CTRLP,
	SKEY_CTRLQ,
	SKEY_CTRLR,
	SKEY_CTRLS,
	SKEY_CTRLT,
	SKEY_CTRLU,
	SKEY_CTRLV,
	SKEY_CTRLW,
	SKEY_CTRLX,
	SKEY_CTRLY,
	SKEY_CTRLZ,

	SKEY_F1,
	SKEY_F2,
	SKEY_F3,
	SKEY_F4,
	SKEY_F5,
	SKEY_F6,
	SKEY_F7,
	SKEY_F8,
	SKEY_F9,
	SKEY_F10,
	SKEY_F11,
	SKEY_F12,
	SKEY_SF1,
	SKEY_SF2,
	SKEY_SF3,
	SKEY_SF4,
	SKEY_SF5,
	SKEY_SF6,
	SKEY_SF7,
	SKEY_SF8,
	SKEY_SF9,
	SKEY_SF10,
	SKEY_SF11,
	SKEY_SF12 
} SpecialKey;

class ITermListener : public IMemoryValidate
{
public:
	virtual void OnTermOpen() = 0;
	virtual void OnTermClose() = 0;
	virtual void OnTermError(const String msg) = 0;
	virtual void OnTermKey(char ch) = 0;
	virtual void OnTermKey(SpecialKey ch) = 0;
	virtual void OnTermKeys(const char *buf, const int len) = 0;
	virtual void OnTermSize( int rows, int cols ) = 0;
	virtual void OnTermType(const String termname) = 0;
	virtual void OnTermCursorPos(int row, int col) = 0;
	virtual void OnTermChanged() = 0;
	virtual void OnTermWindowSize(int minx, int maxx, int miny, int maxy) = 0;
};

class ITerminal : public IErrorListener, public IMemoryValidate
{
public:
	// This ensures that the implemntation class destructor is called
	ITerminal();
	virtual ~ITerminal();

	virtual void OnError(const char *msg) = 0;
	virtual void IndentifyTermCaps() = 0;
	virtual void Send( const byte *buf, const int len ) = 0;
	virtual void SendDisableLineWrap() = 0;
	virtual void SendEnableLineWrap() = 0;
	virtual void SendTextModeMono80X25() = 0;
	virtual void SendTextModeColor80X25() = 0;
	virtual void SendEnableKeyPadTransmit() = 0;
	virtual void SendSetG0() = 0;
	virtual void SendSetG1() = 0;
	virtual void SendSetCharSetS0() = 0;
	virtual void SendSetCharSetS1() = 0;
	virtual void SendClearScreen() = 0;
	virtual void SendCharAtaTimeMode() = 0;
	virtual void SendLineAtaTimeMode() = 0;
	virtual void SendLocalEchoOn() = 0;
	virtual void SendLocalEchoOff() = 0;
	virtual void SendNAWS() = 0;
	virtual int GetNumRows() = 0;
	virtual int GetNumCols() = 0;
	virtual void SetCursor(int row, int col) = 0;
	virtual void SendQueryGetCursor() = 0;
	virtual void SaveCursor() = 0;
	virtual void RestoreCursor() = 0;
	virtual void SaveAttributes() = 0;
	virtual void RestoreAttributes() = 0;
	virtual void SetAttributes( int forecolor, int backcolor, int attr ) = 0;
	virtual TermCap *GetTermCaps() = 0;
	virtual void AddListener( ITermListener *listener ) = 0;
	virtual spl::IStreamPtr GetStream() = 0;

#ifdef DEBUG
	virtual void ValidateMem() const = 0;
	virtual void CheckMem() const = 0;
#endif
};

class AnsiTerm : public ITelnetListener, public ITerminal
{
private:
	// forbid copy constructor
	inline AnsiTerm(const AnsiTerm& pmp) {}
	inline void operator =(const AnsiTerm& pmp) {}

private:
	typedef enum _AnsiCodeState
	{
		STATE_ASCII = 0,
		STATE_ESCAPE = 1,
		STATE_CSI = 2,      /* Escape-[ */
		STATE_G0 = 3,       /* Escape-( */
		STATE_G1 = 4,       /* Escape-) */
		STATE_MODE = 5,     /* Escape-[? */
		STATE_ARG = 6,      /* After the above states */
		STATE_CMD = 7,
		FN_KEY = 8,	/* ESC [ O */
		FN_KEY_2 = 9	/* ESC [ [ */
	} AnsiCodeState;

	/* screen modes */
	typedef enum _TermScreenMode
	{
		SCREEN_MODE_MONO40X25 = 0,
		SCREEN_MODE_COLOR40X25 = 1,
		SCREEN_MODE_MONO80X25 = 2,
		SCREEN_MODE_COLOR80X25 = 3,
		SCREEN_MODE_4COLOR320X200 = 4,
		SCREEN_MODE_MONO320X200 = 5,
		SCREEN_MODE_MONO640X200 = 6,
		SCREEN_MODE_ENABLE_LINE_WRAP = 7,
		SCREEN_MODE_COLOR320X200 = 13,
		SCREEN_MODE_COLOR640X200 = 14,
		SCREEN_MODE_MONO640X480 = 17,
		SCREEN_MODE_16COLOR640X480 = 18,
		SCREEN_MODE_256COLOR320X200 = 19
	} TermScreenMode;

	void Init(ITermListener *listener);
	void InitTelnet();

protected:
	Telnet *m_telnet;
	int m_numrows, m_numcols;
	AnsiCodeState m_state;
	byte m_buf8Csi[8];
	StringBuffer m_strArgs;
	TermScreenMode m_screenMode;
	Vector<ITermListener *> m_listeners;
	String m_termname;
	String m_deviceCode;
	TermCap m_caps;
	bool m_termIdentified;

	void ExecCmd( char cmd );
	void IndentifyTermCaps();
	void DispatchOnTermKey(char ch);
	void DispatchOnTermKey(SpecialKey key);
	void DispatchOnTermChanged();

public:
	//AnsiTerm(TcpSocketPtr sock, SocketSet& ss, ITermListener *listener);
	AnsiTerm(spl::IStreamPtr conn, ITermListener *listener);
	virtual ~AnsiTerm();

	inline Telnet& GetTelnet() { return *m_telnet; }

	virtual void OnTelnetRecv(const byte *data, int len);
	virtual void OnTelnetConnect();
	virtual void OnTelnetClose();
	virtual void OnTelnetError(const String& se);
	virtual void OnTelnetUnmappedOption(int command, int option);
	virtual void OnTelnetSetWindowSize(int minx, int maxx, int miny, int maxy);
	virtual void TelnetGetWindowSize(int *widthInChars, int *heightInChars);
	virtual void OnTelnetTermName(const String& termname);
	virtual const String& TelnetGetTeminalName();
	virtual void OnTelnetStateChange(int command, int option);

	/*void SetTermLineWrapOff();
	void SetTermModeAnsi();
	void QueryTermType();
	void SetTextModeMono80X25();
	void SetTextModeColor80X25();
	void EnableLineWrap();
	void DisableLineWrap();
	void EnableKeyPadTransmit();
	void SetG0();
	void SetG1();
	void SetCharSetS0();
	void SetCharSetS1();*/

	virtual void OnError(const char *msg);
	virtual void Send( const byte *buf, const int len );
	virtual void SendDisableLineWrap();
	virtual void SendEnableLineWrap();
	virtual void SendTextModeMono80X25();
	virtual void SendTextModeColor80X25();
	virtual void SendEnableKeyPadTransmit();
	virtual void SendSetG0();
	virtual void SendSetG1();
	virtual void SendSetCharSetS0();
	virtual void SendSetCharSetS1();
	virtual void SendClearScreen();
	virtual void SendCharAtaTimeMode();
	virtual void SendLineAtaTimeMode();
	virtual void SendLocalEchoOn();
	virtual void SendLocalEchoOff();
	virtual void SendNAWS();
	virtual int GetNumRows();
	virtual int GetNumCols();
	virtual void SetCursor(int row, int col);
	virtual void SendQueryGetCursor();
	virtual void SaveCursor();
	virtual void RestoreCursor();
	virtual void SaveAttributes();
	virtual void RestoreAttributes();
	virtual void SetAttributes( int forecolor, int backcolor, int attr );
	virtual void AddListener( ITermListener *listener );
	virtual TermCap *GetTermCaps();
	virtual spl::IStreamPtr GetStream();

#ifdef DEBUG
	virtual void ValidateMem() const;
	virtual void CheckMem() const;
#endif

	/* Text attributes */
	typedef enum _TermTextAttribute
	{
		TEXTATRR_OFF = 0,		/* All attributes off */
		TEXTATRR_BOLD = 1,		/*   Bold on */
		TEXTATRR_DIM = 2,		/*   Dim */
		TEXTATRR_UNDERSCORE =4,/*   Underscore (on monochrome display adapter only) */
		TEXTATRR_BLINK = 5,	/*   Blink on */
		TEXTATRR_REV = 7,		/*   Reverse video on */
		TEXTATRR_INVIS = 8		/*   Concealed on */
	} TermTextAttribute;

	/* Foreground colors */
	typedef enum _TermForeColor
	{
		FORE_BLACK = 30,		/*    Black   */
		FORE_RED = 31,		/*    Red     */
		FORE_GREEN = 32,		/*    Green   */
		FORE_YELLOW = 33,	/*    Yellow  */
		FORE_BLUE = 34,		/*    Blue    */
		FORE_MAG = 35,		/*    Magenta */
		FORE_CYAN = 36,		/*    Cyan    */
		FORE_GREY = 37		/*    GREY    */
	} TermForeColor;

	/* background colors */
	typedef enum _TermBackColor
	{
		BACK_BLACK = 40,		/*    Black    */
		BACK_RED = 41,		/*    Red      */
		BACK_GREEN = 42,		/*    Green    */
		BACK_YELLOW = 43,	/*    Yellow   */
		BACK_BLUE = 44,		/*    Blue     */
		BACK_MAG = 45,		/*    Magenta  */
		BACK_CYAN = 46,		/*    Cyan     */
		BACK_GREY = 47		/*    Grey     */
	} TermBackColor;

	typedef enum _AnsiKeyCodeCmd
	{
		AHOME = 1,
		ATAB = 2,
		AEND = 3,
		ADEL = 4,
		APGUP = 5,
		APGDN = 6,
		AINS = 7,
		ABKSP = 8
	} AnsiKeyCodeCmd;
};

}

#endif
