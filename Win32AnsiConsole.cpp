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
#include <spl/term/ansicodes.h>
#include <spl/Int32.h>
#include <spl/io/log/Log.h>
#include <spl/term/Win32AnsiConsole.h>

#if defined(WIN32) || defined(_WIN32)

using namespace spl;

static int ansiFlen = 4;
static char ansiF1[] = {ESC, CSI, '[', 'A' + 0};
static char ansiF2[] = {ESC, CSI, '[', 'A' + 1};
static char ansiF3[] = {ESC, CSI, '[', 'A' + 2};
static char ansiF4[] = {ESC, CSI, '[', 'A' + 3};
static char ansiF5[] = {ESC, CSI, '[', 'A' + 4};
static char ansiF6[] = {ESC, CSI, '[', 'A' + 5};
static char ansiF7[] = {ESC, CSI, '[', 'A' + 6};
static char ansiF8[] = {ESC, CSI, '[', 'A' + 7};
static char ansiF9[] = {ESC, CSI, '[', 'A' + 8};
static char ansiF10[] = {ESC, CSI, '[', 'A' + 9};
static char ansiF11[] = {ESC, CSI, '[', 'A' + 10};
static char ansiF12[] = {ESC, CSI, '[', 'A' + 11};

struct arg5
{
	char txt[5];
	byte eos;
};

inline void ValidateType(struct arg5& sa)
{
}

Win32AnsiConsole::Win32AnsiConsole()
: m_caps(), m_curvis(true)
{
	m_hstdin = GetStdHandle(STD_INPUT_HANDLE); 
	m_hstdout = GetStdHandle(STD_OUTPUT_HANDLE); 
	if (m_hstdin == INVALID_HANDLE_VALUE || m_hstdout == INVALID_HANDLE_VALUE) 
	{
		throw new Exception("Can't open Console handles");
	}

	// Save the current text colors. 
	if (! GetConsoleScreenBufferInfo(m_hstdout, &m_csbiInfo)) 
	{
		throw new Exception("Can't get Console screen buffer info");
	}
	m_oldColorAttrs = m_csbiInfo.wAttributes; 

	if (! GetConsoleMode(m_hstdin, &m_fdwOldMode)) 
	{
		throw new Exception("Can't get Console mode");
	}

    // Turn off the line input and echo input modes 
	DWORD fdwMode = m_fdwOldMode & ~(ENABLE_LINE_INPUT | ENABLE_ECHO_INPUT);
	if (! SetConsoleMode(m_hstdin, fdwMode)) 
	{
		throw new Exception("Can't set char-at-a-time input and no echo");
	}
	m_caps.SetTerm("pcansi");
	m_caps.ValidateMem();
	SetConsoleTitle("ANSI Terminal");
}

Win32AnsiConsole::~Win32AnsiConsole()
{
	// Restore the original console mode. 
    SetConsoleMode(m_hstdin, m_fdwOldMode);

    // Restore the original text colors. 
    SetConsoleTextAttribute(m_hstdout, m_oldColorAttrs);
}

void Win32AnsiConsole::SetCursor(int x, int y)
{
	ASSERT (x > 0 && y > 0);
	if (! GetConsoleScreenBufferInfo(m_hstdout, &m_csbiInfo)) 
	{
		throw new Exception("Can't get console screen buffer info");
	}
	m_csbiInfo.dwCursorPosition.X = x - 1; 
	m_csbiInfo.dwCursorPosition.Y = y - 1;

	if (! SetConsoleCursorPosition(m_hstdout, m_csbiInfo.dwCursorPosition)) 
	{
		throw new Exception("Cannot set cursor");
	}
}

void Win32AnsiConsole::GetCursor(int *x, int *y)
{
	if (! GetConsoleScreenBufferInfo(m_hstdout, &m_csbiInfo)) 
	{
		throw new Exception("Can't get console screen buffer info");
	}
	*x = m_csbiInfo.dwCursorPosition.X + 1; 
	*y = m_csbiInfo.dwCursorPosition.Y + 1;
}

void Win32AnsiConsole::Repeat(char ch, int x, int y, int count, int inc)
{
	ASSERT( x > 0 && y > 0 );
	if ( y > m_csbiInfo.srWindow.Bottom + 1 )
	{
		return;
	}

	DWORD writeCount;
	int start = x - 1;
	int end = start + count;
	if ( end > m_csbiInfo.srWindow.Right+1 )
	{
		end = m_csbiInfo.srWindow.Right+1;
	}
	SetCursor(x, y);
	for ( int pos = start; pos < end; pos += inc )
	{
		WriteConsole(m_hstdout, &ch, (DWORD)1, &writeCount, NULL);	
	}
}

void Win32AnsiConsole::Run()
{
	m_running = true;
	DWORD cNumRead;
	INPUT_RECORD irInBuf[1];
	
	ValidateMem();

	while ( m_running )
	{
		try
		{
			if (! ReadConsoleInput( 
					m_hstdin,      // input buffer handle 
					irInBuf,     // buffer to read into 
					1,         // size of read buffer 
					&cNumRead) ) // number of records read 
			{
				Exception *ex = new Exception("ReadFile failed");
				OnError( ex->Message() );
				throw ex;
			}
			if ( cNumRead > 0 )
			{
				switch(irInBuf[0].EventType) 
				{ 
					case KEY_EVENT: // keyboard input 
						KeyEventProc(irInBuf[0].Event.KeyEvent); 
						break; 

					case MOUSE_EVENT: // mouse input 
						//MouseEventProc(irInBuf[0].Event.MouseEvent); 
						break; 

					case WINDOW_BUFFER_SIZE_EVENT: // scrn buf. resizing 
						//ResizeEventProc(irInBuf[0].Event.WindowBufferSizeEvent); 
						break; 

					case FOCUS_EVENT:  // disregard focus events 

					case MENU_EVENT:   // disregard menu events 
						break; 

					default:
						throw new Exception("Unknown event type in Win32 Key read Run()");
						break; 
				} 
			}
			ValidateMem();
		}
		catch ( Exception *ex )
		{
			Log::WriteError(ex);
		}
	}
}

static void ScrollScreenBuffer(HANDLE h, INT x, CONSOLE_SCREEN_BUFFER_INFO *csbiInfo)
{
    SMALL_RECT srctScrollRect, srctClipRect;
    CHAR_INFO chiFill;
    COORD coordDest;

    srctScrollRect.Left = 0;
    srctScrollRect.Top = 1;
    srctScrollRect.Right = csbiInfo->srWindow.Right; 
    srctScrollRect.Bottom = csbiInfo->srWindow.Bottom - x; 
 
    // The destination for the scroll rectangle is one row up. 
 
    coordDest.X = 0; 
    coordDest.Y = 0; 
 
    // The clipping rectangle is the same as the scrolling rectangle. 
    // The destination row is left unchanged. 
 
    srctClipRect = srctScrollRect; 
 
    // Set the fill character and attributes. 
 
    chiFill.Attributes = FOREGROUND_RED|FOREGROUND_INTENSITY; 
    chiFill.Char.AsciiChar = (char)' '; 
 
    // Scroll up one line. 
 
    ScrollConsoleScreenBuffer( 
        h,               // screen buffer handle 
        &srctScrollRect, // scrolling rectangle 
        &srctClipRect,   // clipping rectangle 
        coordDest,       // top left destination cell 
        &chiFill);       // fill character and color 
}

// The NewLine function handles carriage returns when the processed 
// input mode is disabled. It gets the current cursor position 
// and resets it to the first cell of the next row. 
 
void Win32AnsiConsole::NewLine() 
{ 
    if (! GetConsoleScreenBufferInfo(m_hstdout, &m_csbiInfo)) 
    {
		throw new Exception("GetConsoleScreenBufferInfo failed");
    }
    m_csbiInfo.dwCursorPosition.X = 0; 

    // If it is the last line in the screen buffer, scroll 
    // the buffer up. 
    if ((m_csbiInfo.dwSize.Y-1) == m_csbiInfo.dwCursorPosition.Y) 
    { 
        ScrollScreenBuffer(m_hstdout, 1, &m_csbiInfo); 
    } 
    else 
	{
	    // Otherwise, advance the cursor to the next line. 
		m_csbiInfo.dwCursorPosition.Y += 1; 
	}
    if (! SetConsoleCursorPosition(m_hstdout, m_csbiInfo.dwCursorPosition)) 
    {
		throw new Exception("SetConsoleCursorPosition failed");
    }
} 

void Win32AnsiConsole::Send(const byte *buf, const int len)
{
	DWORD writeCount;
	int bufpos = 0;

	while (bufpos < len)
	{
		if ( ESC != buf[bufpos++] )
		{
			if ( '\n' == buf[bufpos-1] )
			{
				continue;
			}
			int x, y;
			GetCursor(&x, &y);
			x--; y--;
			int count = 1;
			while (bufpos < len && ESC != buf[bufpos] && '\n' != buf[bufpos])
			{
				int pos = y * 80 + x + count;
				if ( pos >= 80 * 25 )
				{
					break;
				}
				m_vis[pos] = m_curvis;
				count++;
				bufpos++;
			}
			if ( 1 == count )
			{
				// possible echo of typing, check vis buf
				if ( ! m_vis[y * 80 + x] )
				{
					continue;
				}
			}
			else
			{
				m_vis[y * 80 + x] = m_curvis;
				if ( !m_curvis )
				{
					x++;
				}
			}
			if ( ! WriteConsole(m_hstdout, &buf[bufpos-count], (DWORD)count, &writeCount, NULL) )
			{
				throw new IOException("Error writing to the console");
			}
			continue;
		}
		switch( buf[bufpos++] )
		{
		default:
			throw new Exception("Unknown CSI");

		case 7:		// save cursor and attributes
			break;
		case 8:		// restore cursor and  attributes
			break;
		case 'D':	// scroll down
			break;
		case 'M':	// scroll up
			break;
		case 'H':	// set tab
			break;
		case CSI:
			DoCSI(buf, len, &bufpos);
			break;
		}
	}
}

void Win32AnsiConsole::DoCSI(const byte *buf, const int len, int *bufpos)
{
	Vector<struct arg5> arglist;
	char cmd;

	while ( *bufpos < len && isdigit(buf[*bufpos]) )
	{
		int argpos = 0;
		struct arg5 arg;
		arg.eos = 0;
		while ( *bufpos < len && isdigit( buf[*bufpos] ) )
		{
			arg.txt[argpos++] = buf[(*bufpos)++];
		}
		arg.txt[argpos] = '\0';
		arglist.Add( arg );
		if ( ';' == buf[*bufpos] )
		{
			(*bufpos)++;
		}
		arglist.ValidateMem();
		ASSERT( arg.eos == 0 );
	}
	cmd = buf[(*bufpos)++];
	switch ( cmd )
	{
	// cursor

	case 'H':	// HOME
		arglist.ValidateMem();
		if ( arglist.Count() == 0 )
		{
			SetCursor(1, 1);
		}
		else if (arglist.Count() == 2 )
		{
			ASSERT( 0 == arglist.ElementAt(1).eos && 0 == arglist.ElementAt(0).eos );
			SetCursor( Int32::Parse(arglist.ElementAt(1).txt), Int32::Parse(arglist.ElementAt(0).txt) );
		}
		else
		{
			throw new Exception("Expected zero or two args for HOME");
		}
		break;
	case 'A':	// UP
		break;
	case 'B':	// DOWN
		break;
	case 'C':	// right
		break;
	case 'D':	// left
		break;
	case 'f':	// force (set) cursor
		break;
	case 's':	// save cursor
		break;
	case 'u':	// unsave cursor
		break;

	// scrolling

	case 'r':	// scroll screen
		break;

	// tabs

	case 'g':	// clear tab
		break;

	// erasing text

	case 'K':	// clear to end (or start with arg1==1, or complete line arg1==2) of line
		break;
	case 'J':	// erase from current line to end (top if arg1==1, all if arg1==2) of screen
		arglist.ValidateMem();
		if ( arglist.Count() == 0 )
		{
			// end of screen
			int x, y;
			GetCursor(&x, &y);
			for ( ; y <= m_csbiInfo.srWindow.Bottom+1; y++ )
			{
				Repeat(' ', 1, y, m_csbiInfo.srWindow.Right+1, 1);
			}
		}
		else if ( arglist.ElementAt(0).txt[0] == '1' )
		{
			// top of screen
			ASSERT( 0 == arglist.ElementAt(0).eos );
			int x, y;
			GetCursor(&x, &y);
			for ( ; y > 0; y-- )
			{
				Repeat(' ', 1, y, m_csbiInfo.srWindow.Right+1, 1);
			}
		}
		else if ( arglist.ElementAt(0).txt[0] == '2' )
		{
			// all
			ASSERT( 0 == arglist.ElementAt(0).eos );
			for ( int i = 0; i < m_csbiInfo.srWindow.Bottom+1; i++ )
			{
				Repeat(' ', 1, i+1, m_csbiInfo.srWindow.Right+1, 1);
			}
		}
		else
		{
			throw new Exception("Unknown clear arg");
		}
		break;

	case 'm':	// set attibutes
		WORD attribs = 0;
		for ( int attribcnt = 0; attribcnt < arglist.Count(); attribcnt++ )
		{
			arglist.ValidateMem();
			ASSERT( 0 == arglist.ElementAt(attribcnt).eos );
			int attrib = Int32::Parse( arglist.ElementAt(attribcnt).txt );
			switch( attrib )
			{
			case AnsiTerm::TEXTATRR_OFF: 		/* All attributes off */
				m_curvis = true;
				attribs = 0;
				break;
			case AnsiTerm::TEXTATRR_BOLD: 		/*   Bold on */
				attribs |= FOREGROUND_INTENSITY;
				break;
			case AnsiTerm::TEXTATRR_DIM: 		/*   Dim */
				break;
			case AnsiTerm::TEXTATRR_UNDERSCORE:  /*   Underscore (on monochrome display adapter only) */
				attribs |= COMMON_LVB_UNDERSCORE;
				attribs |= BACKGROUND_BLUE | BACKGROUND_RED | BACKGROUND_GREEN;
				break;
			case AnsiTerm::TEXTATRR_BLINK: 		/*   Blink on */
				break;
			case AnsiTerm::TEXTATRR_REV: 		/*   Reverse video on */
				//attribs |= COMMON_LVB_REVERSE_VIDEO;
				attribs |= BACKGROUND_BLUE | BACKGROUND_RED | BACKGROUND_GREEN;
				break;
			case AnsiTerm::TEXTATRR_INVIS: 		/*   Concealed on */
				//attribs |= BACKGROUND_BLUE | FOREGROUND_BLUE;
				m_curvis = false;
				break;
 
				/* Foreground colors */
			case AnsiTerm::FORE_BLACK:		/*    Black   */
				break;
			case AnsiTerm::FORE_RED: 		/*    Red     */
				attribs |= FOREGROUND_RED;
				break;
			case AnsiTerm::FORE_GREEN: 	/*    Green   */
				attribs |= FOREGROUND_GREEN;
				break;
			case AnsiTerm::FORE_YELLOW:	/*    Yellow  */
				attribs |= FOREGROUND_RED | FOREGROUND_GREEN;
				break;
			case AnsiTerm::FORE_BLUE: 		/*    Blue    */
				attribs |= FOREGROUND_BLUE;
				break;
			case AnsiTerm::FORE_MAG: 		/*    Magenta */
				attribs |= FOREGROUND_GREEN | FOREGROUND_RED;
				break;
			case AnsiTerm::FORE_CYAN: 		/*    Cyan    */
				attribs |= FOREGROUND_BLUE | FOREGROUND_GREEN;
				break;
			case AnsiTerm::FORE_GREY: 		/*    GREY    */
				attribs |= FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED;
				break;

				/* background colors */
			case AnsiTerm::BACK_BLACK:		/*    Black    */
				break;
			case AnsiTerm::BACK_RED:		/*    Red      */
				attribs |= BACKGROUND_RED;
				break;
			case AnsiTerm::BACK_GREEN:		/*    Green    */
				attribs |= BACKGROUND_GREEN;
				break;
			case AnsiTerm::BACK_YELLOW:	/*    Yellow   */
				attribs |= BACKGROUND_RED | BACKGROUND_GREEN;
				break;
			case AnsiTerm::BACK_BLUE: 		/*    Blue     */
				attribs |= BACKGROUND_BLUE;
				break;
			case AnsiTerm::BACK_MAG: 		/*    Magenta  */
				attribs |= BACKGROUND_GREEN | BACKGROUND_RED;
				break;
			case AnsiTerm::BACK_CYAN: 		/*    Cyan     */
				attribs |= BACKGROUND_GREEN | BACKGROUND_BLUE;
				break;
			case AnsiTerm::BACK_GREY: 		/*    Grey     */
				attribs |= BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED;
				break;

			case 10:
				// stupid tandem
				attribs |= FOREGROUND_GREEN;
				break;
			default:
				Log::WriteInfo("unknown attribute %d", attrib);
				//throw new Exception("Unknown attribute");
				break;
			}
		}
		if ( ! SetConsoleTextAttribute(m_hstdout, attribs) )
		{
			throw new Exception( "SetConsoleTextAttribute failed" );
		}
		break;
	}
	ValidateMem();
}

#if defined(DEBUG) || defined(_DEBUG)
void Win32AnsiConsole::CheckMem() const
{ 
	m_caps.CheckMem();
}

void Win32AnsiConsole::ValidateMem()  const
{ 
	m_caps.ValidateMem();
}
#endif

Win32AnsiConsoleOutTerm::Win32AnsiConsoleOutTerm(ITermListener *listener)
: m_listeners(), m_strm()
{
	if ( NULL != listener )
	{
		m_listeners.Add(listener);
	}
	Start();
}

Win32AnsiConsoleOutTerm::~Win32AnsiConsoleOutTerm()
{
}

void Win32AnsiConsoleOutTerm::Send( const byte *str, const int len )
{
	Win32AnsiConsole::Send( str, len );
}

void Win32AnsiConsoleOutTerm::SendLineWrapOff()
{
}

void Win32AnsiConsoleOutTerm::SendDisableLineWrap()
{
}

void Win32AnsiConsoleOutTerm::SendEnableLineWrap()
{
}

void Win32AnsiConsoleOutTerm::SendQueryTermType()
{
}

void Win32AnsiConsoleOutTerm::SendQueryTermSizeRowsCols()
{
}

void Win32AnsiConsoleOutTerm::SendTextModeMono80X25()
{
}

void Win32AnsiConsoleOutTerm::SendTextModeColor80X25()
{
}

void Win32AnsiConsoleOutTerm::SendEnableKeyPadTransmit()
{
}

void Win32AnsiConsoleOutTerm::SendSetG0()
{
}

void Win32AnsiConsoleOutTerm::SendSetG1()
{
}

void Win32AnsiConsoleOutTerm::SendSetCharSetS0()
{
}

void Win32AnsiConsoleOutTerm::SendSetCharSetS1()
{
}

void Win32AnsiConsoleOutTerm::SendClearScreen()
{
    if (! GetConsoleScreenBufferInfo(m_hstdout, &m_csbiInfo)) 
    {
		throw new Exception("GetConsoleScreenBufferInfo failed");
    }
	for ( int y = 1; y <= m_csbiInfo.srWindow.Bottom; y++ )
	{
		Repeat(' ', 1, y, m_csbiInfo.srWindow.Right+1, 1 );
	}
	// Prevent window scroll
	Repeat(' ', 1, m_csbiInfo.srWindow.Bottom+1, m_csbiInfo.srWindow.Right, 1 );
}

void Win32AnsiConsoleOutTerm::SendCharAtaTimeMode()
{
	DWORD mode;
	if (! GetConsoleMode(m_hstdin, &mode)) 
	{
		throw new Exception("Can't get Console mode");
	}
	DWORD fdwMode = mode & ~(ENABLE_LINE_INPUT | ENABLE_ECHO_INPUT);
	if (! SetConsoleMode(m_hstdin, fdwMode)) 
	{
		throw new Exception("Can't set char-at-a-time input and no echo");
	}
}

void Win32AnsiConsoleOutTerm::SendLineAtaTimeMode()
{
	DWORD mode;
	if (! GetConsoleMode(m_hstdin, &mode)) 
	{
		throw new Exception("Can't get Console mode");
	}
	DWORD fdwMode = mode | ENABLE_LINE_INPUT | ENABLE_ECHO_INPUT;
	if (! SetConsoleMode(m_hstdin, fdwMode)) 
	{
		throw new Exception("Can't set line-a-time input and no echo");
	}
#ifdef DEBUG
	mode = 0;
	GetConsoleMode(m_hstdin, &mode);
	ASSERT( mode & (ENABLE_LINE_INPUT | ENABLE_ECHO_INPUT) );
#endif
}

void Win32AnsiConsoleOutTerm::SendLocalEchoOn()
{
	DWORD mode;
	if (! GetConsoleMode(m_hstdin, &mode)) 
	{
		throw new Exception("Can't get Console mode");
	}
	DWORD fdwMode = mode & ENABLE_ECHO_INPUT;
	if (! SetConsoleMode(m_hstdin, fdwMode)) 
	{
		throw new Exception("Can't set echo on");
	}
}

void Win32AnsiConsoleOutTerm::SendLocalEchoOff()
{
	DWORD mode;
	if (! GetConsoleMode(m_hstdin, &mode)) 
	{
		throw new Exception("Can't get Console mode");
	}
	DWORD fdwMode = mode & ~(ENABLE_ECHO_INPUT);
	if (! SetConsoleMode(m_hstdin, fdwMode)) 
	{
		throw new Exception("Can't set echo off");
	}
}

int Win32AnsiConsoleOutTerm::GetNumRows()
{
    if (! GetConsoleScreenBufferInfo(m_hstdout, &m_csbiInfo)) 
    {
		throw new Exception("GetConsoleScreenBufferInfo failed");
    }
    return m_csbiInfo.srWindow.Bottom; 
}

int Win32AnsiConsoleOutTerm::GetNumCols()
{
    if (! GetConsoleScreenBufferInfo(m_hstdout, &m_csbiInfo)) 
    {
		throw new Exception("GetConsoleScreenBufferInfo failed");
    }
    return m_csbiInfo.srWindow.Right; 
}

void Win32AnsiConsoleOutTerm::SetCursor(int row, int col)
{
	ASSERT(row > 0 && col > 0);
	Win32AnsiConsole::SetCursor(col, row);
}

void Win32AnsiConsoleOutTerm::SendQueryGetCursor()
{
	int row, col;
	Win32AnsiConsole::GetCursor(&col, &row);
	for ( int x = 0; x < m_listeners.Count(); x++ )
	{
		ASSERT_PTR( m_listeners.ElementAt(x) );
		m_listeners.ElementAt(x)->OnTermCursorPos(row, col);
	}
}

void Win32AnsiConsoleOutTerm::SaveCursor()
{
}

void Win32AnsiConsoleOutTerm::RestoreCursor()
{
}

void Win32AnsiConsoleOutTerm::SaveAttributes()
{
}

void Win32AnsiConsoleOutTerm::RestoreAttributes()
{
}

void Win32AnsiConsoleOutTerm::SetAttributes( int forground, int background, int attr )
{
	WORD attribs = 0;
	if ( attr & AnsiTerm::TEXTATRR_OFF) 		/* All attributes off */
	{
		m_curvis = true;
		attribs = 0;
	}
	if ( attr & AnsiTerm::TEXTATRR_BOLD) 		/*   Bold on */
	{
		attribs |= FOREGROUND_INTENSITY;
	}
	if ( attr & AnsiTerm::TEXTATRR_DIM) 		/*   Dim */
	{
	}
	if ( attr & AnsiTerm::TEXTATRR_UNDERSCORE)  /*   Underscore (on monochrome display adapter only) */
	{
		attribs |= COMMON_LVB_UNDERSCORE;
		attribs |= BACKGROUND_BLUE | BACKGROUND_RED | BACKGROUND_GREEN;
	}
	if ( attr & AnsiTerm::TEXTATRR_BLINK) 		/*   Blink on */
	{
	}
	if ( attr & AnsiTerm::TEXTATRR_REV) 		/*   Reverse video on */
	{
		//attribs |= COMMON_LVB_REVERSE_VIDEO;
		attribs |= BACKGROUND_BLUE | BACKGROUND_RED | BACKGROUND_GREEN;
	}
	if ( attr & AnsiTerm::TEXTATRR_INVIS) 		/*   Concealed on */
	{
		//attribs |= BACKGROUND_BLUE | FOREGROUND_BLUE;
		m_curvis = false;
	}

		/* Foreground colors */
	if ( forground == AnsiTerm::FORE_BLACK)		/*    Black   */
	{
	}
	if ( forground == AnsiTerm::FORE_RED) 		/*    Red     */
	{
		attribs |= FOREGROUND_RED;
	}
	if ( forground == AnsiTerm::FORE_GREEN) 	/*    Green   */
	{
		attribs |= FOREGROUND_GREEN;
	}
	if ( forground == AnsiTerm::FORE_YELLOW)	/*    Yellow  */
	{
		attribs |= FOREGROUND_RED | FOREGROUND_GREEN;
	}
	if ( forground == AnsiTerm::FORE_BLUE) 		/*    Blue    */
	{
		attribs |= FOREGROUND_BLUE;
	}
	if ( forground == AnsiTerm::FORE_MAG) 		/*    Magenta */
	{
		attribs |= FOREGROUND_GREEN | FOREGROUND_RED;
	}
	if ( forground == AnsiTerm::FORE_CYAN) 		/*    Cyan    */
	{
		attribs |= FOREGROUND_BLUE | FOREGROUND_GREEN;
	}
	if ( forground == AnsiTerm::FORE_GREY) 		/*    GREY    */
	{
		attribs |= FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED;
	}

		/* background colors */
	if ( background == AnsiTerm::BACK_BLACK)		/*    Black    */
	{
	}
	if ( background == AnsiTerm::BACK_RED)		/*    Red      */
	{
		attribs |= BACKGROUND_RED;
	}
	if ( background == AnsiTerm::BACK_GREEN)		/*    Green    */
	{
		attribs |= BACKGROUND_GREEN;
	}
	if ( background == AnsiTerm::BACK_YELLOW)	/*    Yellow   */
	{
		attribs |= BACKGROUND_RED | BACKGROUND_GREEN;
	}
	if ( background == AnsiTerm::BACK_BLUE) 		/*    Blue     */
	{
		attribs |= BACKGROUND_BLUE;
	}
	if ( background == AnsiTerm::BACK_MAG) 		/*    Magenta  */
	{
		attribs |= BACKGROUND_GREEN | BACKGROUND_RED;
	}
	if ( background == AnsiTerm::BACK_CYAN) 		/*    Cyan     */
	{
		attribs |= BACKGROUND_GREEN | BACKGROUND_BLUE;
	}
	if ( background == AnsiTerm::BACK_GREY) 		/*    Grey     */
	{
		attribs |= BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED;
	}
		
	if ( ! SetConsoleTextAttribute(m_hstdout, attribs) )
	{
		throw new Exception( "SetConsoleTextAttribute failed" );
	}
}

void Win32AnsiConsoleOutTerm::Receive(byte *buf, int len)
{
	throw new Exception("Not expecting Receive to be called");
}

bool Win32AnsiConsoleOutTerm::RecvBlocking()
{
	Thread::Sleep(10 * 60 * 1000);
	return true;
}

void Win32AnsiConsoleOutTerm::OnOpen()
{
}

void Win32AnsiConsoleOutTerm::OnClose()
{
}

void Win32AnsiConsoleOutTerm::OnError(const char *msg)
{
}

void Win32AnsiConsoleOutTerm::IndentifyTermCaps()
{
}

void Win32AnsiConsoleOutTerm::SendNAWS()
{
}

bool Win32AnsiConsoleOutTerm::IsClosed()
{
	return false;
}

void Win32AnsiConsoleOutTerm::DispatchTermKey(SpecialKey c)
{
	for ( int x = 0; x < m_listeners.Count(); x++ )
	{
		ASSERT_PTR( m_listeners.ElementAt(x) );
		m_listeners.ElementAt(x)->OnTermKey(c);
	}
}

void Win32AnsiConsoleOutTerm::DispatchTermKey(char c)
{
	for ( int x = 0; x < m_listeners.Count(); x++ )
	{
		ASSERT_PTR( m_listeners.ElementAt(x) );
		m_listeners.ElementAt(x)->OnTermKey(c);
	}
}

void Win32AnsiConsoleOutTerm::KeyEventProc(KEY_EVENT_RECORD event) 
{
	if ( event.bKeyDown || 0 == m_listeners.Count() )
	{
		return;
	}
	switch (event.wVirtualKeyCode)
	{
		//case VK_LSHIFT:
		//case VK_RSHIFT:
		//case VK_LCONTROL:
		//case VK_RCONTROL:
		case VK_F1:
			if ( event.dwControlKeyState & SHIFT_PRESSED )
			{
				DispatchTermKey(SKEY_SF1);
			}
			else
			{
				DispatchTermKey(SKEY_F1);
			}
			break;
		case VK_F2:
			if ( event.dwControlKeyState & SHIFT_PRESSED )
			{
				DispatchTermKey(SKEY_SF2);
			}
			else
			{
				DispatchTermKey(SKEY_F2);
			}
			break;
		case VK_F3:
			if ( event.dwControlKeyState & SHIFT_PRESSED )
			{
				DispatchTermKey(SKEY_SF3);
			}
			else
			{
				DispatchTermKey(SKEY_F3);
			}
			break;
		case VK_F4:
			if ( event.dwControlKeyState & SHIFT_PRESSED )
			{
				DispatchTermKey(SKEY_SF4);
			}
			else
			{
				DispatchTermKey(SKEY_F4);
			}
			break;
		case VK_F5:
			if ( event.dwControlKeyState & SHIFT_PRESSED )
			{
				DispatchTermKey(SKEY_SF5);
			}
			else
			{
				DispatchTermKey(SKEY_F5);
			}
			break;
		case VK_F6:
			if ( event.dwControlKeyState & SHIFT_PRESSED )
			{
				DispatchTermKey(SKEY_SF6);
			}
			else
			{
				DispatchTermKey(SKEY_F6);
			}
			break;
		case VK_F7:
			if ( event.dwControlKeyState & SHIFT_PRESSED )
			{
				DispatchTermKey(SKEY_SF7);
			}
			else
			{
				DispatchTermKey(SKEY_F7);
			}
			break;
		case VK_F8:
			if ( event.dwControlKeyState & SHIFT_PRESSED )
			{
				DispatchTermKey(SKEY_SF8);
			}
			else
			{
				DispatchTermKey(SKEY_F8);
			}
			break;
		case VK_F9:
			if ( event.dwControlKeyState & SHIFT_PRESSED )
			{
				DispatchTermKey(SKEY_SF9);
			}
			else
			{
				DispatchTermKey(SKEY_F9);
			}
			break;
		case VK_F10:
			if ( event.dwControlKeyState & SHIFT_PRESSED )
			{
				DispatchTermKey(SKEY_SF10);
			}
			else
			{
				DispatchTermKey(SKEY_F10);
			}
			break;
		case VK_F11:
			if ( event.dwControlKeyState & SHIFT_PRESSED )
			{
				DispatchTermKey(SKEY_SF11);
			}
			else
			{
				DispatchTermKey(SKEY_F11);
			}
			break;
		case VK_F12:
			if ( event.dwControlKeyState & SHIFT_PRESSED )
			{
				DispatchTermKey(SKEY_SF12);
			}
			else
			{
				DispatchTermKey(SKEY_F12);
			}
			break;
		case VK_HOME:
			DispatchTermKey(SKEY_HOME);
			break;
		case VK_INSERT:
			DispatchTermKey(SKEY_INS);
			break;
		case VK_DELETE:
			DispatchTermKey(SKEY_DEL);
			break;
		case VK_DOWN:
			DispatchTermKey(SKEY_DOWN);
			break;
		case VK_END:
			DispatchTermKey(SKEY_END);
			break;
		case VK_LEFT:
			DispatchTermKey(SKEY_LEFT);
			break;
		case VK_NEXT:
			break;
		case VK_PRIOR:
			break;
		case VK_SNAPSHOT:
			break;
		case VK_RIGHT:
			DispatchTermKey(SKEY_RIGHT);
			break;
		case VK_UP:
			DispatchTermKey(SKEY_UP);
			break;
		case VK_SCROLL:
			break;
		case VK_TAB:
			DispatchTermKey(SKEY_TAB);
			break;
		default:
			if (event.uChar.AsciiChar == '\n')
			{
				DispatchTermKey(SKEY_LF);
			}
			else if (event.uChar.AsciiChar == '\r')
			{
				DispatchTermKey(SKEY_CR);
			}
			else
			{
				DispatchTermKey(event.uChar.AsciiChar);
			}
			break;
	}	
}

TermCap *Win32AnsiConsoleOutTerm::GetTermCaps() 
{ 
	return &m_caps; 
}

void Win32AnsiConsoleOutTerm::AddListener( ITermListener *listener ) 
{
	ASSERT_PTR( listener );
	m_listeners.Add(listener); 
}

bool Win32AnsiConsoleOutTerm::IsListenerReady() 
{ 
	return m_listeners.Count() > 0; 
}

IStreamPtr Win32AnsiConsoleOutTerm::GetStream()
{
	if ( m_strm._IsNull() )
	{
		m_strm = Win32ConsoleStreamPtr(new Win32ConsoleStream(this));
	}
	return m_strm;
}

#if defined(DEBUG) || defined(_DEBUG)
void Win32AnsiConsoleOutTerm::CheckMem() const
{ 
	Win32AnsiConsole::CheckMem(); 
	m_listeners.CheckMem();
	m_strm.CheckMem();
}
void Win32AnsiConsoleOutTerm::ValidateMem() const
{ 
	Win32AnsiConsole::ValidateMem(); 
	m_listeners.ValidateMem();
	m_strm.ValidateMem();
}
#endif

Win32ConsoleStream::Win32ConsoleStream(Win32AnsiConsoleOutTerm *term)
: m_term(term)
{
	ASSERT_MEM(m_term, sizeof(Win32AnsiConsoleOutTerm));
}

Win32ConsoleStream::~Win32ConsoleStream()
{
}

int Win32ConsoleStream::Read(Array<byte>& buf, const int offset, int len)
{
	ReadByte();
	return 0;
}

int Win32ConsoleStream::ReadByte()
{
	Thread::Sleep(1000 * 60 * 60 * 12); // 12 hour
	return 0;
}

void Win32ConsoleStream::Write( const Array<byte>& buf, const int offset, const int len )
{
	m_term->Send(buf, len);
}

void Win32ConsoleStream::WriteByte( byte value )
{
	m_term->Send(&value, 1);
}

void Win32ConsoleStream::Close()
{
}

void Win32ConsoleStream::Flush()
{
}

long Win32ConsoleStream::Seek(const long offset, const SeekOrigin origin)
{
	throw new NotImplementedException();
}

bool Win32ConsoleStream::CanRead() const
{
	return false;
}

bool Win32ConsoleStream::CanSeek() const
{
	return false;
}

bool Win32ConsoleStream::CanWrite() const
{
	return true;
}

long Win32ConsoleStream::Length() const
{
	return 0;
}

long Win32ConsoleStream::Position() const
{
	return 0;
}

#if defined(DEBUG) || defined(_DEBUG)
void Win32ConsoleStream::CheckMem() const
{ 
}

void Win32ConsoleStream::ValidateMem() const 
{ 
	ASSERT_MEM(m_term, sizeof(Win32AnsiConsoleOutTerm));
	m_term->ValidateMem();
}
#endif


#endif /* WIN32 */
