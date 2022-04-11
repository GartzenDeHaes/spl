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

#include <ctype.h>
#include <spl/types.h>
#include <spl/Debug.h>
#include <spl/Int32.h>
#include <spl/term/TermCap.h>
#include <spl/term/Color.h>
#include <spl/term/TextTerm.h>

using namespace spl;

static void padchar( char *str, const char ch, const int len )
{
	int x;
	for ( x = 0; x < len; x++ )
	{
		str[x] = ch;
	}
	str[x] = '\0';
}

static void insertin( char *str, const char *str1, const int at, const int len )
{
	int x;
	ASSERT( len + at <= (int)strlen( str ) );
	for ( x = 0; x < len; x++ )
	{
		str[at + x] = *(str1++);
	}
}

TerminalCell::TerminalCell()
{
	Clear();
	m_updated = true;
}

TerminalCell::~TerminalCell()
{
}

void TerminalCell::Clear()
{
	m_bold = 0;
	m_under = 0;
	m_blink = 0;
	m_reverse  = 0;
	m_invis = 0;
	m_acs = 0;
	if ( m_ch != ' ' )// || m_attribs != 0 || this->m_foreColor != AnsiTerm::FORE_GREEN || this->m_backColor != AnsiTerm::BACK_BLACK )
	{
		m_updated = true;
	}
	m_ch = ' ';
	m_attribs = 0;
	SetForeColor(AnsiTerm::FORE_GREEN);
	SetBackColor(AnsiTerm::BACK_BLACK);
}

void TerminalCell::SetAttributes( int attr )
{
	if ( attr != m_attribs )
	{
		m_attribs = attr;
		m_bold = (attr & TA_BOLD) != 0;
		m_under = (attr & TA_UNDER) != 0;
		m_blink = (attr & TA_BLINK) != 0;
		m_reverse = (attr & TA_REVRS) != 0;
		m_invis = (attr & TA_INVIS) != 0;
		m_acs = (attr & TA_ACS) != 0;
		m_updated = true;

		ASSERT ( m_attribs == ((m_bold?TA_BOLD:0) | (m_under?TA_UNDER:0) | 	(m_blink?TA_BLINK:0) | (m_reverse?TA_REVRS:0) | (m_invis?TA_INVIS:0) | (m_acs?TA_ACS:0)) );
	}
}

void TerminalCell::WriteText( StringBuffer *strbuf, TermCap *termcaps )
{
	ASSERT_PTR( strbuf );
	strbuf->ValidateMem();
	ASSERT_PTR( termcaps );
	if ( m_acs )
	{
		// write attribs turns on acs
		strbuf->Append( termcaps->ACS(m_ch) );
		//if ( termcaps->HasACS() /*&& !termcaps->HasS0S1()*/ )
		//{
		//	termcaps->GetCapString(TCAP_ae, strbuf);
		//}
	}
	else
	{
		//if ( termcaps->HasS0S1() )
		//{
		//	strbuf->Append( (char)toupper(m_ch) );
		//}
		//else
		//{
			strbuf->Append( m_ch );
		//}
	}
	m_updated = false;
}

void TerminalCell::WriteAttributes( StringBuffer *strbuf, TermCap *termcaps, char *tmpbuf, int tmpbuflen )
{
	bool usesemi = false;

	strbuf->Append( ESC );
	strbuf->Append( CSI );
	if ( m_bold )
	{
		strbuf->Append(Int32::ToString(AnsiTerm::TEXTATRR_BOLD));
		usesemi = true;
	}
	if ( m_under )
	{
		if ( usesemi == true )
		{
			strbuf->Append(';');
		}
		strbuf->Append(Int32::ToString(AnsiTerm::TEXTATRR_UNDERSCORE));
		usesemi = true;
	}
	if ( m_blink )
	{
		if ( usesemi == true )
		{
			strbuf->Append(';');
		}
		strbuf->Append(Int32::ToString(AnsiTerm::TEXTATRR_BLINK));
		usesemi = true;
	}
	if ( m_reverse )
	{
		if ( usesemi == true )
		{
			strbuf->Append(';');
		}
		strbuf->Append(Int32::ToString(AnsiTerm::TEXTATRR_REV));
		usesemi = true;
	}
	if ( m_invis )
	{
		if ( usesemi == true )
		{
			strbuf->Append(';');
		}
		strbuf->Append(Int32::ToString(AnsiTerm::TEXTATRR_INVIS));
		usesemi = true;
	}
	if ( usesemi == true )
	{
		strbuf->Append(';');
	}
	else
	{
		strbuf->Append("0;");
	}
	strbuf->Append( Int32::ToString(GetForeColor()) );
	strbuf->Append( ';' );
	strbuf->Append( Int32::ToString(GetBackColor()) );
	strbuf->Append( 'm' );

	ASSERT_PTR(termcaps);
	termcaps->ValidateMem();

	ASSERT( -1 == termcaps->GetCapNum(TCAP_sg) || 0 == termcaps->GetCapNum(TCAP_sg) );

	if ( m_acs && (termcaps->HasACS() || termcaps->HasS0S1()) )
	{
		termcaps->GetCapString(TCAP_as, strbuf);
	}
	//else if ( m_acs && termcaps->HasS0S1() )
	//{
	//	strbuf->Append( (char)0xe );
	//	strbuf->Append( (char)0x4 );
	//}
	else if ( !m_acs && (termcaps->HasACS() || termcaps->HasS0S1()) )
	{
		termcaps->GetCapString(TCAP_ae, strbuf);
	}
	//else if ( !m_acs && termcaps->HasS0S1() )
	//{
	//	strbuf->Append( (char)0xf );
	//	strbuf->Append( (char)0x4 );
	//}

}

GC::GC( TermCap *termcaps, int width, int height )
{
	m_termcaps = termcaps;
	m_width = width;
	m_height = height;

	m_attribs = 0;
	SetBackColor( AnsiTerm::BACK_BLACK );
	SetForeColor( AnsiTerm::FORE_GREEN );
	if ( NULL == (m_cells = new TerminalCell[width * height]) )
	{
		throw OutOfMemoryException();
	}
	ASSERT_MEM( m_cells, sizeof(TerminalCell) * m_width * m_height );
	for ( int x = 0; x < m_width * m_height; x++ )
	{
		m_cells[x].Clear();
		SetCell( &m_cells[x] );
	}
}

GC::~GC()
{
	ValidateMem();
	delete[] m_cells;
}

void GC::Resize( int width, int height )
{
	delete[] m_cells;
	m_width = width;
	m_height = height;
	if ( NULL == (m_cells = new TerminalCell[width * height]) )
	{
		throw OutOfMemoryException();
	}
	ASSERT_MEM( m_cells, sizeof(TerminalCell) * m_width * m_height );
	for ( int x = 0; x < m_width * m_height; x++ )
	{
		m_cells[x].Clear();
		SetCell( &m_cells[x] );
	}
}

void GC::SetCell( TerminalCell *cell )
{
	ASSERT_PTR( cell );
	if ( cell->GetAttributes() != m_attribs )
	{
		cell->SetAttributes( m_attribs );
	}
	if ( cell->GetForeColor() != m_forecolor )
	{
		cell->SetForeColor( m_forecolor );
	}
	if ( cell->GetBackColor() != m_backcolor )
	{
		cell->SetBackColor( m_backcolor );
	}
}

void GC::WriteText( const char *ch, int x, int y )
{
	ASSERT(y >= 0 && x >= 0);
	if ( x >= m_width || y >= m_height )
	{
		return;
	}
	int writeLen = (int)strlen(ch);
	if ( writeLen + x >= m_width )
	{
		writeLen = m_width - x;
	}
	if ( writeLen < 1 )
	{
		return;
	}
	ASSERT(x*y < m_width*m_height);
	ASSERT_MEM( m_cells, sizeof(TerminalCell) * m_width * m_height );
	TerminalCell *cell = &m_cells[y * m_width + x];
	
	for ( int pos = 0; pos < writeLen; pos++ )
	{
		ASSERT_PTR( cell );
		SetCell( cell );
		cell->SetChar( ch[pos] );
		cell++;
	}
}

void GC::WriteText( byte ch, int x, int y )
{
	if ( x >= m_width || y >= m_height )
	{
		return;
	}
	TerminalCell *cell = &m_cells[y * m_width + x];
	SetCell( cell );
	cell->SetChar( ch );
}

void GC::ClearScreen()
{
	for ( int x = 0; x < m_width * m_height; x++ )
	{
		SetCell( &m_cells[x] );
		m_cells[x].SetChar(' ');
	}
}

void GC::Invalidate()
{
	for ( int x = 0; x < m_width * m_height; x++ )
	{
		m_cells[x].SetDirty(true);
	}
}

void GC::RepeatChar( int count, char ch, int x, int y )
{
	char *buf = new char[count+1];
	memset(buf, ch, count);
	buf[count] = '\0';
	ASSERT( (int)strlen(buf) == count );
	WriteText( buf, x, y );
	delete [] buf;
}

void GC::UpdateToString( StringBuffer *sbuf )
{
	int attributes = 0;
	int forecolor = 0;
	int backcolor = 0;
	TerminalCell *cell;
	bool resetCursor = true;
	int len = m_width * m_height;

	ValidateMem();

	//if ( termcaps->HasACS() /*&& !termcaps->HasS0S1()*/ )
	//{
	//	termcaps->GetCapString(TCAP_ae, strbuf);
	//}

	if ( m_termcaps->GetTermName().Equals("cygwin") ||
		m_termcaps->GetTermName().Equals("pcansi") )
	{
		// prevent scrollup after writing the lower right char,
		// also means that the poistion 79X24 is unwritable.
		len--;
	}
	// Moving cursor to HOME after screen write seems to solve this problem
	//if ( !m_termcaps->IsDOS() && (!m_termcaps->GetCapFlag(TCAP_ns) || m_termcaps->GetCapFlag(TCAP_am)) )
	//{
	//	// prevent scrollup after writing the lower right char,
	//	// also means that the poistion 79X24 is unwritable.
	//	len--;
	//}
	for ( int x = 0; x < len; x++ )
	{
		if ( 0 == (x % 80) )
		{
			resetCursor = true;
		}
		cell = &m_cells[x];
		ASSERT_PTR( cell );
		if ( cell->IsDirty() )
		{
			if ( resetCursor )
			{
				int xp = x % m_width;
				int xy = x / m_width;
				SetCursor( sbuf, xp, xy );
				resetCursor = false;
			}
			if ( attributes != cell->GetAttributes() ||
				forecolor != cell->GetForeColor() ||
				backcolor != cell->GetBackColor() )
			{
				cell->WriteAttributes( sbuf, m_termcaps, m_cmdbuf, GC_CMD_BUF_SIZE );
				attributes = cell->GetAttributes();
				forecolor = cell->GetForeColor();
				backcolor = cell->GetBackColor();
			}
			ASSERT_PTR( cell );
			cell->WriteText( sbuf, m_termcaps );
		}
		else
		{
			resetCursor = true;
		}
	}
	//printf("%s\n", sbuf->getChars() );
}

void GC::SetCursor( StringBuffer *sbuf, int x, int y )
{
	ValidateMem();
	m_termcaps->GetCapString(TCAP_cm, &m_cmdbuf[0], GC_CMD_BUF_SIZE);
	m_termcaps->FormatGoto(m_cmdbuf, sbuf, x, y);
}

void GC::DrawBox( int x, int y, int width, int height )
{
	int attrback = m_attribs;
	m_attribs |= TerminalCell::TA_ACS;
	int len = y + height;
	for ( int yy = y+1; yy < len; yy++ )
	{
		WriteText(VLINE, x, yy);
		WriteText(VLINE, x+width, yy);
	}
	len = x + width;
	for ( int xx = x+1; xx < len; xx++ )
	{
		WriteText(HLINE, xx, y);
		WriteText(HLINE, xx, y+height);
	}
	WriteText(ULCORNER, x, y);
	WriteText(URCORNER, x+width, y);
	WriteText(LLCORNER, x, y+height);
	WriteText(LRCORNER, x+width, y+height);

	m_attribs = attrback;
}

void GC::DrawHLine( int x, int y, int length )
{
	int count = x + length;
	int attrback = m_attribs;
	m_attribs |= TerminalCell::TA_ACS;
	for ( int xx = x; xx < count; xx++ )
	{
		WriteText(HLINE, xx, y);
	}
	m_attribs = attrback;
}

void GC::DrawVLine( int x, int y, int length )
{
	int count = y + length;
	int attrback = m_attribs;
	m_attribs |= TerminalCell::TA_ACS;
	for ( int yy = y; yy < count; yy++ )
	{
		WriteText(VLINE, x, yy);
	}
	m_attribs = attrback;
}

#ifdef DEBUG
void GC::ValidateMem() const
{
	ASSERT_MEM( m_cells, sizeof(TerminalCell) * m_width * m_height );
}

void GC::CheckMem() const
{
	DEBUG_NOTE_MEM_ALLOCATION( m_cells );
}
#endif

TerminalWidget::TerminalWidget
(
	int taborder,
	TerminalFrame *frame,
	int attribs, 
	AnsiTerm::TermForeColor forecolor, 
	AnsiTerm::TermBackColor backcolor, 
	int x, 
	int y, 
	int width, 
	int height
)
: m_ancLeft(false), m_ancRight(false), m_ancTop(false), m_ancBottom(false)
{
	ASSERT( x + width < 81 && y + height < 26 );
	m_taborder = taborder;
	m_frame = frame;
	m_canFocus = false;
	m_focus = false;
	m_x = x;
	m_y = y;
	m_width = width;
	m_height = height;
	m_zorder = 0;
	m_attribs = attribs;
	m_forecolor = forecolor;
	m_backcolor = backcolor;
	m_dirty = true;
}

TerminalWidget::~TerminalWidget()
{
}

bool TerminalWidget::HandleKey( GC *gc, char key ) { return false; }
bool TerminalWidget::HandleKeyCtrl( SpecialKey key ) { return false; }
bool TerminalWidget::HandleKeyCmd( GC *gc, SpecialKey key ) { return false; }
bool TerminalWidget::TrapTabKey() { return false; }
bool TerminalWidget::TrapLF() { return false; }

bool TerminalWidget::SetFocus(GC *gc)
{
	if ( m_canFocus )
	{
		m_focus = true;
		return true;
	}
	return false;
}

void TerminalWidget::LostFocus( GC *gc ) 
{
	m_focus = false;
}

void TerminalWidget::CursorPosition( int *x, int *y ) const
{ 
	*x = 0; 
	*y = 0; 
}

void TerminalWidget::ResizeColumns(int oldScreenWidth, int newScreenWidth)
{
	if ( ! (IsAnchorLeft() || IsAnchorRight()) )
	{
		return;
	}
	m_dirty = true;
	if ( IsAnchorLeft() )
	{
		// shouldn't need to do anything?
	}
	if ( IsAnchorRight() )
	{
		m_width = newScreenWidth - (oldScreenWidth - (m_x + m_width));
	}
}

void TerminalWidget::ResizeRows(int oldScreenHeight, int newScreenHeight)
{
	if ( ! (IsAnchorTop() || IsAnchorBottom()) )
	{
		return;
	}
	m_dirty = true;
	if ( IsAnchorTop() && IsAnchorBottom() )
	{
		// stretch
		m_height = newScreenHeight - (oldScreenHeight - m_height);
	}
	else if ( IsAnchorTop() )
	{
		// shouldn't need to do anything?
	}
	else if ( IsAnchorBottom() )
	{
		m_y = newScreenHeight - (oldScreenHeight - m_y);
	}
}

void TerminalWidget::WriteMask( BitMask *bm )
{
}

void TerminalWidget::AssertMask( BitMask *bm )
{
}

#if defined(DEBUG) || defined(_DEBUG)
void TerminalWidget::ValidateMem() const
{
	ASSERT_PTR( m_frame );
}

void TerminalWidget::CheckMem() const
{
}
#endif

/********** MENU ***********/

TerminalWidgetMenuBarItem::TerminalWidgetMenuBarItem
( 
	int x,
	int y,
	char *name, 
	int id, 
	EVENTHANDLER handler 
)
: m_name()
{
	m_x = x;
	m_y = y;
	m_id = id;
	m_handler = handler;
	SetText( name );
}

TerminalWidgetMenuBarItem::~TerminalWidgetMenuBarItem()
{
}

void TerminalWidgetMenuBarItem::SetText( const char *text )
{
	m_name.Set( text );
	int pos = m_name.IndexOf('@');
	if ( pos < 0 )
	{
		m_ctrlkey = SKEY_NULL;
	}
	else
	{
		m_ctrlkey = (SpecialKey)(toupper(m_name[pos+1]) - 'A' + SKEY_CTRLA);
	}
}

bool TerminalWidgetMenuBarItem::OnEvent(TerminalWidget *parent)
{
	return (*m_handler)(parent, m_id, parent->GetFrame()->GetFrameManager());
}

void TerminalWidgetMenuBarItem::Draw( GC *gc )
{
	int baseAttribs = gc->GetAttributes();
	bool clearAttr = false;
	int pos = 0;
	for ( int x = 0; x < m_name.Length(); x++ )
	{
		if ( m_name[x] == '@' )
		{
			if ( (baseAttribs & TerminalCell::TA_BOLD) == 0 )
			{
				gc->SetAttributes(baseAttribs | TerminalCell::TA_BOLD);
			}
			else
			{
				gc->SetAttributes(baseAttribs & ~TerminalCell::TA_BOLD);
			}
			x++;
			clearAttr = true;
		}
		gc->WriteText( m_name[x], m_x + pos++, m_y );
		if ( clearAttr )
		{
			gc->SetAttributes( baseAttribs );
			clearAttr = false;
		}
	}
}

#ifdef DEBUG
void TerminalWidgetMenuBarItem::ValidateMem() const
{
	m_name.ValidateMem();
}

void TerminalWidgetMenuBarItem::CheckMem() const
{
	m_name.CheckMem();
}
#endif

TerminalWidgetMenuBar::TerminalWidgetMenuBar
(
	char *title, 
	TerminalFrame *frame,
	int attribs,
	AnsiTerm::TermForeColor forecolor,
	AnsiTerm::TermBackColor backcolor,
	int x, 
	int y, 
	int width
)
	: TerminalWidget(0, frame, attribs, forecolor, backcolor, x, y, width, 1)
{
	strncpy( m_title, title, 80 );
	m_canFocus = false;
	m_newItemPos = 0;

	m_ancLeft = true;
	m_ancRight = true;
	if ( y == 0 )
	{
		m_ancTop = true;
	}
	else
	{
		m_ancBottom = true;
	}
}

TerminalWidgetMenuBar::~TerminalWidgetMenuBar()
{
	for ( int x = 0; x < m_items.Count(); x++ )
	{
		TerminalWidgetMenuBarItem *item = m_items.ElementAt(x);
		ASSERT_MEM( item, sizeof(TerminalWidgetMenuBarItem) );
		item->ValidateMem();
		delete item;
	}
}

void TerminalWidgetMenuBar::AddItem( char *name, int id, EVENTHANDLER handler )
{
	TerminalWidgetMenuBarItem *item = new TerminalWidgetMenuBarItem(m_newItemPos, m_y, name, id, handler);
	m_newItemPos += item->Width();
	m_items.Add( item );
	m_dirty = true;
}

void TerminalWidgetMenuBar::SetItemText( int idx, char *name )
{
	TerminalWidgetMenuBarItem *item = m_items.ElementAt( idx );
	if ( NULL == item )
	{
		return;
	}
	item->SetText( name );
	m_dirty = true;
}

void TerminalWidgetMenuBar::Draw( GC *gc )
{
	gc->SetAttributes( m_attribs );
	gc->SetForeColor( m_forecolor );
	gc->SetBackColor( m_backcolor );
	int len = (int)strlen( m_title );
	gc->RepeatChar( m_width, ' ', m_x, m_y );
	gc->WriteText( m_title, m_x + m_width - len, m_y );
	for (int x = 0; x < m_items.Count(); x++ )
	{
		m_items.ElementAt(x)->Draw( gc );
	}
	m_dirty = false;
}

bool TerminalWidgetMenuBar::HandleKeyCtrl( SpecialKey key )
{
	for ( int x = 0; x < m_items.Count(); x++ )
	{
		TerminalWidgetMenuBarItem *item = m_items.ElementAt(x);
		if ( item->HotKey() == key )
		{
			if ( item->OnEvent( this ) )
			{
				return true;
			}
		}
	}
	return false;
}

#ifdef DEBUG
void TerminalWidgetMenuBar::ValidateMem() const
{
	TerminalWidget::ValidateMem();
	m_items.ValidateMem();
	int count = m_items.Count();
	for ( int x = 0; x < count; x++ )
	{
		ASSERT_MEM( m_items.ElementAt(x), sizeof(TerminalWidgetMenuBarItem) );
		m_items.ElementAt(x)->CheckMem();
	}
}

void TerminalWidgetMenuBar::CheckMem() const
{
	TerminalWidget::CheckMem();
	m_items.CheckMem();
	int count = m_items.Count();
	for ( int x = 0; x < count; x++ )
	{
		DEBUG_NOTE_MEM_ALLOCATION( m_items.ElementAt(x) );
		m_items.ElementAt(x)->CheckMem();
	}
}
#endif

/********** STATUS BAR ***********/

TerminalWidgetStatusBarPanel::TerminalWidgetStatusBarPanel( int len )
{
	ASSERT( len < 81 );
	m_len = len;
	padchar( m_text, ' ', len );
}

TerminalWidgetStatusBarPanel::~TerminalWidgetStatusBarPanel()
{
}

void TerminalWidgetStatusBarPanel::SetText( const char *text )
{
	int len = (int)strlen( text );
	padchar( m_text, ' ', m_len );
	insertin( m_text, text, 0, len );
}

#ifdef DEBUG
void TerminalWidgetStatusBarPanel::ValidateMem() const
{
}

void TerminalWidgetStatusBarPanel::CheckMem() const
{
}
#endif

TerminalWidgetStatusBar::TerminalWidgetStatusBar
(
	TerminalFrame *frame, 
	int attribs, 
	AnsiTerm::TermForeColor forecolor, 
	AnsiTerm::TermBackColor backcolor, 
	int x, 
	int y, 
	int width
)
:	TerminalWidget(0, frame, attribs, forecolor, backcolor, x, y, width, 1), 
	m_panels()
{
	m_ancLeft = true;
	m_ancRight = true;
	if ( y == 0 )
	{
		m_ancTop = true;
	}
	else
	{
		m_ancBottom = true;
	}
}

TerminalWidgetStatusBar::~TerminalWidgetStatusBar()
{
	for ( int x = 0; x < m_panels.Count(); x++ )
	{
		TerminalWidgetStatusBarPanel *panel = m_panels.ElementAt(x);
		ASSERT_MEM( panel, sizeof(TerminalWidgetStatusBarPanel) );
		panel->ValidateMem();
		delete panel;
	}
}

int TerminalWidgetStatusBar::AddPanel( int width )
{
	ValidateMem();

	TerminalWidgetStatusBarPanel *panel = new TerminalWidgetStatusBarPanel( width );
	if ( NULL == panel )
	{
		throw OutOfMemoryException();
	}
	m_panels.Add( panel );
	m_dirty = true;
	return m_panels.Count()-1;
}

void TerminalWidgetStatusBar::SetPanelText( int panelNum, const char *text )
{
	ValidateMem();

	TerminalWidgetStatusBarPanel *panel = m_panels.ElementAt(panelNum);
	if ( NULL == panel )
	{
		throw new Exception( "Illegal panelNum" );
	}
	panel->SetText( text );
	m_dirty = true;
}

void TerminalWidgetStatusBar::Draw( GC *gc )
{
	ValidateMem();

	int x;
	gc->SetAttributes( m_attribs );
	gc->SetForeColor( m_forecolor );
	gc->SetBackColor( m_backcolor );
	for ( x = 0; x < m_width; x++ )
	{
		gc->WriteText(' ', x + m_x, m_y);
	}
	int xpos = m_x;
	for ( x = 0; x < m_panels.Count(); x++ )
	{
		TerminalWidgetStatusBarPanel *panel = m_panels.ElementAt(x);
		ASSERT_MEM( panel, sizeof(TerminalWidgetStatusBarPanel) );
		panel->ValidateMem();
		gc->WriteText( panel->GetText(), xpos, m_y );
		xpos += panel->Length();
	}
	m_dirty = false;
}

#ifdef DEBUG
void TerminalWidgetStatusBar::ValidateMem() const
{
	TerminalWidget::ValidateMem();
	m_panels.ValidateMem();
	for ( int x = 0; x < m_panels.Count(); x++ )
	{
		ASSERT_MEM( m_panels.ElementAt(x), sizeof(TerminalWidgetStatusBarPanel) );
		m_panels.ElementAt(x)->ValidateMem();
	}
}

void TerminalWidgetStatusBar::CheckMem() const
{
	TerminalWidget::CheckMem();
	m_panels.CheckMem();
	for ( int x = 0; x < m_panels.Count(); x++ )
	{
		DEBUG_NOTE_MEM_ALLOCATION( m_panels.ElementAt(x) );
		m_panels.ElementAt(x)->CheckMem();
	}
}
#endif

/********** LABEL ***********/

TerminalWidgetLabel::TerminalWidgetLabel
(
	bool centered,
	char *text, 
	TerminalFrame *frame, 
	int attribs, 
	AnsiTerm::TermForeColor forecolor, 
	AnsiTerm::TermBackColor backcolor, 
	int x, 
	int y, 
	int width
)	: TerminalWidget(0, frame, attribs, forecolor, backcolor, x, y, width, 1)
{
	ASSERT( width <= 80 );
	StrCpyLen(m_text, text, width);
	m_centered = centered;
}

TerminalWidgetLabel::~TerminalWidgetLabel()
{
}

void TerminalWidgetLabel::Draw( GC *gc )
{
	gc->SetAttributes(m_attribs);
	gc->SetBackColor(m_backcolor);
	gc->SetForeColor(m_forecolor);

	if ( m_centered )
	{
		char buf[81];
		int len = (int)strlen(m_text);
		if ( len >= m_width )
		{
			m_text[m_width] = '\0';
			len = m_width;
		}
		int start = (m_width - len)/2;
		padchar(buf, ' ', m_width);
		insertin(buf, m_text, start, len);
		gc->WriteText(buf, m_x, m_y);
	}
	else
	{
		gc->WriteText(m_text, m_x, m_y);
	}
	m_dirty = false;
}

void TerminalWidgetLabel::SetText( const char *text )
{
	StrCpyLen(m_text, text, m_width);
}

#ifdef DEBUG
void TerminalWidgetLabel::ValidateMem() const
{
	TerminalWidget::ValidateMem();
}

void TerminalWidgetLabel::CheckMem() const
{
	TerminalWidget::CheckMem();
}
#endif

/********** TEXT BOX ***********/

TerminalWidgetTextBox::TerminalWidgetTextBox
(
	int taborder,
	TerminalFrame *frame, 
	int attribs, 
	AnsiTerm::TermForeColor forecolor, 
	AnsiTerm::TermBackColor backcolor, 
	int activeattribs,
	AnsiTerm::TermForeColor activeforecolor, 
	AnsiTerm::TermBackColor activebackcolor, 
	int x, 
	int y, 
	int width
)
	: TerminalWidget(taborder, frame, attribs, forecolor, backcolor, x-1, y, width+1, 1),
	m_text()
{
	m_text.Fill(' ', width);
	m_text.ValidateMem();
	m_canFocus = true;
	m_activeforecolor = activeforecolor;
	m_activebackcolor = activebackcolor;
	m_activeattribs = activeattribs;
	m_caretPos = 0;
}

TerminalWidgetTextBox::~TerminalWidgetTextBox()
{
}

void TerminalWidgetTextBox::CursorPosition( int *x, int *y ) const { *x = m_x + m_caretPos+1; *y = m_y; }

bool TerminalWidgetTextBox::SetFocus( GC *gc )
{
	TerminalWidget::SetFocus( gc );
	if ( NULL != gc )
	{
		Draw( gc );
	}
	return true;
}

void TerminalWidgetTextBox::LostFocus( GC *gc )
{
	TerminalWidget::LostFocus( gc );
	if ( NULL != gc )
	{
		Draw( gc );
	}
}

void TerminalWidgetTextBox::Draw( GC *gc )
{
	if ( m_focus )
	{
		gc->SetForeColor(m_activeforecolor);
		gc->SetBackColor(AnsiTerm::BACK_BLACK);
		//if ( gc->GetTermcap()->IsDOS() )
		//{
		//	gc->WriteText( '>', m_x, m_y );
		//	gc->WriteText( '<', m_x+m_width, m_y );
		//}
		//else
		//{
			gc->SetAttributes(TerminalCell::TA_ACS);
			gc->WriteText( RARROW, m_x, m_y );
			gc->WriteText( LARROW, m_x+m_width, m_y );
		//}
		gc->SetBackColor(m_activebackcolor);
		gc->SetForeColor(m_activeforecolor);
		gc->SetAttributes(m_activeattribs);
	}
	else
	{
		gc->SetAttributes(0);
		gc->SetForeColor(m_forecolor);
		gc->SetBackColor(AnsiTerm::BACK_BLACK);
		gc->WriteText( ' ', m_x, m_y );
		gc->WriteText( ' ', m_x+m_width, m_y );

		gc->SetBackColor(m_backcolor);
		gc->SetForeColor(m_forecolor);
		gc->SetAttributes(m_attribs);
	}
	gc->WriteText( m_text.GetChars(), m_x+1, m_y );
	for ( int x = m_text.Length(); x < m_width-1; x++ )
	{
		gc->WriteText( ' ', m_x+1+x, m_y );
	}
	m_dirty = false;
}

bool TerminalWidgetTextBox::HandleKey( GC *gc, char key )
{
	if ( m_caretPos >= m_width-1 )
	{
		m_frame->HandleKeyTab();
		if ( m_frame->ControlWithFocus() != this )
		{
			m_frame->HandleKey(key);
		}
		return true;
	}
	gc->SetAttributes(m_activeattribs);
	gc->SetForeColor(m_activeforecolor);
	gc->SetBackColor(m_activebackcolor);
	m_text.SetCharAt(key, m_caretPos);
	gc->WriteText(key, m_caretPos++ + m_x+1, m_y);
	if ( m_caretPos >= m_width-1 )
	{
		m_frame->HandleKeyTab();
	}
	return true;
}

bool TerminalWidgetTextBox::HandleKeyCmd( GC *gc, SpecialKey key )
{
	switch ( key )
	{
	case SKEY_BKSP:
		if ( m_caretPos > 0 )
		{
			m_text.RemoveCharAt(--m_caretPos);
			//m_text.SetCharAt( '\0', --m_caretPos );
			//gc->WriteText(' ', m_caretPos + m_x+1, m_y);
		}
		break;
	case SKEY_RIGHT:
		if ( m_caretPos < m_width )
		{
			m_caretPos++;
			if ( m_text.CharAt(m_caretPos) == '\0' )
			{
				m_text.SetCharAt(' ', m_caretPos);
				m_text.SetCharAt('\0', m_caretPos+1);
			}
			return true;
		}
		break;
	case SKEY_LEFT:
		if ( m_caretPos > 0 )
		{
			m_caretPos--;
		}
		break;
	case SKEY_INS:
		m_text.InsertCharAt(' ', m_caretPos);
		break;
	case SKEY_DEL:
		m_text.RemoveCharAt(m_caretPos);
		break;
	}
	return false;
}

void TerminalWidgetTextBox::ClearText()
{
	while ( m_caretPos > 0 )
	{
		m_text.SetCharAt( '\0', m_caretPos-- );
	}
	m_text.SetCharAt( '\0', 0 );
}

#ifdef DEBUG
void TerminalWidgetTextBox::ValidateMem() const
{
	TerminalWidget::ValidateMem();
	m_text.ValidateMem();
}

void TerminalWidgetTextBox::CheckMem() const
{
	TerminalWidget::CheckMem();
	m_text.CheckMem();
}
#endif

/********* ACTION TEXT BOX *********/

TerminalWidgetActionTextBox::TerminalWidgetActionTextBox
(
	EVENTHANDLER handler, 
	int taborder, 
	TerminalFrame *frame, 
	int attribs, 
	AnsiTerm::TermForeColor forecolor, 
	AnsiTerm::TermBackColor backcolor, 
	int activeattribs, 
	AnsiTerm::TermForeColor activeforecolor, 
	AnsiTerm::TermBackColor activebackcolor, 
	int x, 
	int y, 
	int width
)
	: TerminalWidgetTextBox(taborder, frame, attribs, forecolor, backcolor, activeattribs, activeforecolor, activebackcolor, x, y, width)
{
	m_handler = handler;
}

TerminalWidgetActionTextBox::~TerminalWidgetActionTextBox()
{
}

void TerminalWidgetActionTextBox::LostFocus( GC *gc )
{
	TerminalWidgetTextBox::LostFocus( gc );
}

bool  TerminalWidgetActionTextBox::TrapLF() { return true; }

bool TerminalWidgetActionTextBox::HandleKey( GC *gc, char key )
{
	if ( '\n' == key )
	{
		return m_handler(this, 0, m_frame->GetFrameManager());
	}
	else
	{
		return TerminalWidgetTextBox::HandleKey( gc, key );
	}
}

/********** BUTTON ***********/

TerminalWidgetButton::TerminalWidgetButton
(
	int taborder, 
	EVENTHANDLER handler, 
	char *text, 
	TerminalFrame *frame, 
	int attribs, 
	AnsiTerm::TermForeColor forecolor, 
	AnsiTerm::TermBackColor backcolor, 
	int activeattribs, 
	AnsiTerm::TermForeColor activeforecolor, 
	AnsiTerm::TermBackColor activebackcolor, 
	int x, 
	int y, 
	int width
)
	: TerminalWidget(taborder, frame, attribs, forecolor, backcolor, x, y, width, 1)
{
	m_canFocus = true;
	m_handler = handler;

	int len = (int)strlen(text);
	padchar(m_text, ' ', width);
	insertin(m_text, text, (width - len)/2, len);

	int pos = IndexOfCh(m_text, '@');
	if ( pos >= 0 )
	{
		m_hotkey = (SpecialKey)(toupper(m_text[pos+1]) - 'A' + SKEY_CTRLA);
	}
	else
	{
		m_hotkey = SKEY_NULL;
	}
	m_activeattribs = activeattribs;
	m_activeforecolor = activeforecolor;
	m_activebackcolor = activebackcolor;
}

TerminalWidgetButton::~TerminalWidgetButton()
{
}

bool TerminalWidgetButton::SetFocus( GC *gc )
{
	TerminalWidget::SetFocus(gc);
	if ( NULL == gc )
	{
		return true;
	}

	gc->SetBackColor(AnsiTerm::BACK_BLACK);
	gc->SetForeColor(m_activeforecolor);
	//if ( gc->GetTermcap()->IsDOS() )
	//{
	//	gc->WriteText('>', m_x-1, m_y);
	//	gc->WriteText('<', m_x+m_width-1, m_y);
	//}
	//else
	//{
		gc->SetAttributes(TerminalCell::TA_ACS);
		gc->WriteText(RARROW, m_x-1, m_y);
		gc->WriteText(LARROW, m_x+m_width-1, m_y);
	//}

	if ( m_activebackcolor != m_backcolor ||
		m_activeforecolor != m_forecolor ||
		m_activeattribs != m_attribs )
	{
		Draw( gc );
	}
	return true;
}

void TerminalWidgetButton::LostFocus( GC *gc )
{
	TerminalWidget::LostFocus(gc);
	if ( NULL == gc )
	{
		return;
	}
	gc->SetBackColor(AnsiTerm::BACK_BLACK);
	gc->SetAttributes(0);
	gc->WriteText(' ', m_x-1, m_y);
	gc->WriteText(' ', m_x+m_width-1, m_y);

	if ( m_activebackcolor != m_backcolor ||
		m_activeforecolor != m_forecolor ||
		m_activeattribs != m_attribs )
	{
		Draw( gc );
	}
}

bool TerminalWidgetButton::TrapLF() { return true; }
void TerminalWidgetButton::CursorPosition( int *x, int *y ) const { *x = m_x + (m_width-(int)strlen(m_text)); *y = m_y; }

void TerminalWidgetButton::Draw( GC *gc )
{
	if ( m_focus )
	{
		gc->SetBackColor(m_activebackcolor);
		gc->SetForeColor(m_activeforecolor);
		gc->SetAttributes(m_activeattribs);
	}
	else
	{
		gc->SetBackColor(m_backcolor);
		gc->SetForeColor(m_forecolor);
		gc->SetAttributes(m_attribs);
	}

	int baseAttribs = gc->GetAttributes();
	bool clearAttr = false;
	int pos = 0;
	int len = (int)strlen(m_text);
	for ( int x = 0; x < len; x++ )
	{
		if ( m_text[x] == '@' )
		{
			if ( (baseAttribs & TerminalCell::TA_BOLD) == 0 )
			{
				gc->SetAttributes(baseAttribs | TerminalCell::TA_BOLD);
			}
			else
			{
				gc->SetAttributes(baseAttribs & ~TerminalCell::TA_BOLD);
			}
			x++;
			clearAttr = true;
		}
		gc->WriteText( m_text[x], m_x + pos++, m_y );
		if ( clearAttr )
		{
			gc->SetAttributes( baseAttribs );
			clearAttr = false;
		}
	}
	m_dirty = false;
}

bool TerminalWidgetButton::HandleKeyCtrl( SpecialKey key )
{
	if ( key == m_hotkey )
	{
		return (*m_handler)(this, key, m_frame->GetFrameManager());
	}
	return false;
}

bool TerminalWidgetButton::HandleKey( GC *gc, char key )
{
	if ( '\n' == key )
	{
		return HandleKeyCtrl( m_hotkey );
	}
	return false;
}

#ifdef DEBUG
void TerminalWidgetButton::ValidateMem() const
{
}

void TerminalWidgetButton::CheckMem() const
{
}
#endif

/*********** SPINNER ***********/

TerminalWidgetSpinner::TerminalWidgetSpinner
(
	int taborder, 
	TerminalFrame *frame, 
	int attribs, 
	AnsiTerm::TermForeColor forecolor, 
	AnsiTerm::TermBackColor backcolor, 
	int activeattribs, 
	AnsiTerm::TermForeColor activeforecolor, 
	AnsiTerm::TermBackColor activebackcolor, 
	int x, 
	int y, 
	int width
)
	: TerminalWidget(taborder, frame, attribs, forecolor, backcolor, x, y, width+1, 1),
	m_options(), m_curopt(0)
{
	m_activeattribs = activeattribs;
	m_activebackcolor = activebackcolor;
	m_activeforecolor = activeforecolor;
	m_canFocus = true;
}

TerminalWidgetSpinner::~TerminalWidgetSpinner()
{
}

void TerminalWidgetSpinner::CursorPosition( int *x, int *y ) const { *x = m_x; *y = m_y; }

bool TerminalWidgetSpinner::SetFocus( GC *gc )
{
	TerminalWidget::SetFocus( gc );
	if ( NULL != gc )
	{
		Draw( gc );  
	}
	return true; 
}

void TerminalWidgetSpinner::LostFocus( GC *gc )
{
	TerminalWidget::LostFocus( gc );
	if ( NULL != gc )
	{
		Draw( gc ); 
	}
}

void TerminalWidgetSpinner::Draw( GC *gc )
{
	gc->SetBackColor(m_frame->GetBackgroundColor());
	gc->SetAttributes(0);
	gc->WriteText(18, m_x+m_width-1, m_y);
	if ( m_focus )
	{
		gc->SetAttributes(TerminalCell::TA_ACS);
		gc->WriteText( RARROW, m_x-1, m_y );
		gc->WriteText( LARROW, m_x+m_width, m_y );
		gc->SetBackColor(m_activebackcolor);
		gc->SetForeColor(m_activeforecolor);
		gc->SetAttributes(m_activeattribs);
	}
	else
	{
		gc->SetAttributes(0);
		gc->WriteText(' ', m_x+m_width, m_y);
		gc->WriteText(' ', m_x-1, m_y);
		gc->SetBackColor(m_backcolor);
		gc->SetForeColor(m_forecolor);
		gc->SetAttributes(m_attribs);
	}
	if ( m_options.Count() > 0 )
	{
		gc->WriteText( m_options.ElementAt(m_curopt)->GetChars(), m_x, m_y );
	}
}

bool TerminalWidgetSpinner::HandleKey( GC *gc, char key )
{
	if ( ++m_curopt >= m_options.Count() )
	{
		m_curopt = 0;
	}
	Draw( gc );
	return true;
}

bool TerminalWidgetSpinner::HandleKeyCmd( GC *gc, SpecialKey key )
{
	return false;
}

void TerminalWidgetSpinner::AddOption( const char *val )
{
	StringBuffer sp( val );
	if ( sp.Length() >= (m_width-1) )
	{
		sp.SetCharAt('\0', m_width-1);
	}
	m_options.Add(sp.ToString());
}

#ifdef DEBUG
void TerminalWidgetSpinner::ValidateMem() const
{
	TerminalWidget::ValidateMem();
	m_options.ValidateMem();
}

void TerminalWidgetSpinner::CheckMem() const
{
	TerminalWidget::CheckMem();
	m_options.CheckMem();
}
#endif

/********* SCROLL TEXT ********/

TerminalWidgetScrollText::TerminalWidgetScrollText
(
	int scrollbacklen, 
	TerminalFrame *frame, 
	int attribs, 
	AnsiTerm::TermForeColor forecolor, 
	AnsiTerm::TermBackColor backcolor, 
	int x, 
	int y, 
	int width, 
	int height
)
	: TerminalWidget(0, frame, attribs, forecolor, backcolor, x, y, width, height),
	m_lines()
{
	m_scrolllen = scrollbacklen;
}

TerminalWidgetScrollText::~TerminalWidgetScrollText()
{
	Clear();
}

void TerminalWidgetScrollText::Append( char *txt )
{
	m_dirty = true;
	AppendLine( StringPtr(new String( txt )) );
}

void TerminalWidgetScrollText::AppendLine( StringPtr line )
{
	line.ValidateMem();
	if ( line->Length() < TextWidth() && line->IndexOf('\n') < 0 )
	{
		m_lines.Add( line );
	}
	else
	{
		int start = 0;
		int lastspace = 0;
		int count = line->Length();
		for ( int x = 0; x < count; x++ )
		{
			if ( line->CharAt(x) == ' ' )
			{
				lastspace = x;
			}
			else if ( line->CharAt(x) == '\n' )
			{
				StringPtr str = line->Mid(start, x);
				start = x+1;
				m_lines.Add( str );		
			}
			else if ( (x - start) == TextWidth() )
			{
				StringPtr str = line->Mid(start, lastspace);
				start = lastspace + 1;
				m_lines.Add( str );
			}
		}
		if ( start < count )
		{
			m_lines.Add( line->Substring(start) );
		}
	}
}

void TerminalWidgetScrollText::Clear()
{
	m_lines.ValidateMem();
	//m_lines.Begin();
	m_lines.Clear();
	m_dirty = true;
}

void TerminalWidgetScrollText::Draw( GC *gc )
{
	gc->SetBackColor(m_backcolor);
	gc->SetForeColor(m_forecolor);
	gc->SetAttributes(m_attribs);

	List<StringPtr>::Iterator litr = m_lines.Begin();

	for ( int y = TextY() + TextHeight()-1; y >= 0; y-- )
	{
		StringPtr str = litr.Current();
		litr.Next();

		if ( str.IsNull() )
		{
			gc->RepeatChar( TextWidth(), ' ', TextX(), y );
			break;
		}
		str.ValidateMem();
		gc->WriteText( str->GetChars(), TextX(), y );
		if ( TextWidth() > str->Length() )
		{
			gc->RepeatChar( TextWidth() - str->Length(), ' ', TextX()+str->Length(), y );
		}
	}
	m_dirty = false;
}

#ifdef DEBUG
void TerminalWidgetScrollText::ValidateMem() const
{
	TerminalWidget::ValidateMem();
	m_lines.ValidateMem();

	for ( int x = 0; x < m_lines.Count(); x++ )
	{
		StringPtr str = m_lines.ElementAt(x);
		str.ValidateMem();
	}
}

void TerminalWidgetScrollText::CheckMem() const
{
	TerminalWidget::CheckMem();
	m_lines.CheckMem();

	for ( int x = 0; x < m_lines.Count(); x++ )
	{
		StringPtr str = m_lines.ElementAt(x);
		str.CheckMem();
	}
}
#endif

/********** WRAP TEXT ************/

TerminalWidgetWrapText::TerminalWidgetWrapText
(
	TerminalFrame *frame, 
	int attribs, 
	AnsiTerm::TermForeColor forecolor, 
	AnsiTerm::TermBackColor backcolor, 
	int x, 
	int y, 
	int width, 
	int height
)
	: TerminalWidget(0, frame, attribs, forecolor, backcolor, x, y, width, height),
	m_text("")
{
}

TerminalWidgetWrapText::~TerminalWidgetWrapText()
{
}

void TerminalWidgetWrapText::SetText( const char *txt )
{
	m_dirty = true;
	m_text.Set( txt );
}

void TerminalWidgetWrapText::Draw( GC *gc )
{
	gc->SetBackColor(m_backcolor);
	gc->SetForeColor(m_forecolor);
	gc->SetAttributes(m_attribs);

	int lastspace = -1;
	int col = 0;
	int row = 0;
	int count = m_text.Length();
	for ( int i = 0; i < count && row < m_height; i++ )
	{
		int ch = m_text.CharAt(i);
		ASSERT( ch != '\r' );
		if ( '\n' == ch )
		{
			row++;
			col = 0;
			continue;
		}
		if ( ' ' == ch )
		{
			lastspace = i;
		}
		gc->WriteText( ch, col++ + m_x, row + m_y );
		
		if ( col > m_width )
		{
			if ( lastspace != i )
			{
				// backup and re-write
				for( int j = lastspace; j <= i; j++ )
				{
					gc->WriteText( ' ', (col + m_x) - (j - lastspace ), row + m_y );
				}
				row++;
				col = 0;
				if ( row < m_height )
				{
					for( int j = lastspace+1; j <= i; j++ )
					{
						gc->WriteText( m_text.CharAt(j), col++ + m_x, row + m_y );
					}
				}
			}
			else
			{
				col = 0;
				row++;
			}
		}
	}
}

#ifdef DEBUG
void TerminalWidgetWrapText::ValidateMem() const
{
	TerminalWidget::ValidateMem();
	m_text.ValidateMem();
}

void TerminalWidgetWrapText::CheckMem() const
{
	TerminalWidget::CheckMem();
	m_text.CheckMem();
}
#endif

/************ Text Panel **************/

TerminalWidgetTextPanel::TerminalWidgetTextPanel
(
	TerminalFrame *frame, 
	int attribs, 
	AnsiTerm::TermForeColor forecolor, 
	AnsiTerm::TermBackColor backcolor, 
	int x, 
	int y, 
	int width, 
	int height
)
: TerminalWidget(0, frame, attribs, forecolor, backcolor, x, y, width, height),
m_evkey(NULL), m_evctl(NULL), m_evcmd(NULL), m_cx(x), m_cy(y)
{
	if ( NULL == (m_text = new TerminalCell[width * height]) )
	{
		throw OutOfMemoryException();
	}
	Clear();
	m_canFocus = true;
}

TerminalWidgetTextPanel::~TerminalWidgetTextPanel()
{
	delete[] m_text;
}

void TerminalWidgetTextPanel::SetCursor( int x, int y ) { m_cx = x; m_cy = y; }

void TerminalWidgetTextPanel::CursorPosition(int *x, int *y) const
{
	*x = m_cx;
	*y = m_cy;
}

void TerminalWidgetTextPanel::SetText( char ch, int x, int y, int attribs )
{
	TerminalCell *cell = GetCell(x, y);
	cell->SetChar(ch);
	cell->SetAttributes( attribs );
	m_cx = x;
	m_cy = y;
}

StringPtr TerminalWidgetTextPanel::GetText()
{
	ValidateMem();

	StringBuffer buf;

	for (int y = 0; y < m_height; y++)
	{
		for (int x = 0; x < m_width; x++ )
		{
			buf.Append(GetCell(x, y)->GetChar());
		}
		buf.Append("\r\n");
	}

	return buf.ToString();
}

void TerminalWidgetTextPanel::Clear()
{
	ValidateMem();
	for (int x = 0; x < m_width * m_height; x++ )
	{
		m_text[x].Clear();
		m_text[x].SetAttributes( m_attribs );
	}
	m_cx = m_x;
	m_cy = m_y;
}

void TerminalWidgetTextPanel::Draw( GC *gc )
{
	gc->SetBackColor( m_backcolor );
	gc->SetForeColor( m_forecolor );

	for ( int c = 0; c < m_width; c++ )
	{
		for ( int r = 0; r < m_height; r++ )
		{
			TerminalCell *cell = GetCell(c, r);
			if ( cell->IsDirty() )
			{
				gc->SetAttributes( cell->GetAttributes() );
				gc->WriteText( cell->GetChar(), c + m_x, r + m_y );
				cell->SetDirty(false);
			}
		}
	}
}

bool TerminalWidgetTextPanel::HandleKey( GC *gc, char key )
{
	if ( NULL != m_evkey )
	{
		return m_evkey(this, (int)key, m_frame->GetFrameManager());
	}
	return false;
}

bool TerminalWidgetTextPanel::HandleKeyCtrl( SpecialKey key )
{
	if ( NULL != m_evctl )
	{
		return m_evctl(this, (int)key, m_frame->GetFrameManager());
	}
	return false;
}

bool TerminalWidgetTextPanel::HandleKeyCmd( GC *gc, SpecialKey key )
{
	if ( NULL != m_evcmd )
	{
		return m_evcmd(this, (int)key, m_frame->GetFrameManager());
	}
	return false;
}

bool TerminalWidgetTextPanel::TrapTabKey() { return true; }
bool TerminalWidgetTextPanel::TrapLF() { return true; }

#ifdef DEBUG
void TerminalWidgetTextPanel::ValidateMem() const
{
	TerminalWidget::ValidateMem();
	ASSERT_MEM( m_text, sizeof(TerminalCell) * m_width * m_height );
}

void TerminalWidgetTextPanel::CheckMem() const
{
	TerminalWidget::CheckMem();
	DEBUG_NOTE_MEM_ALLOCATION( m_text );
}
#endif

/********* BOX ***********/

TerminalWidgetBox::TerminalWidgetBox
(
	TerminalFrame *frame, 
	int attribs, 
	AnsiTerm::TermForeColor forecolor, 
	AnsiTerm::TermBackColor backcolor, 
	int x, 
	int y, 
	int width, 
	int height
)
	: TerminalWidget(0, frame, attribs, forecolor, backcolor, x, y, width, height)
{
}

TerminalWidgetBox::TerminalWidgetBox
(
	TerminalFrame *frame, 
	int attribs, 
	AnsiTerm::TermForeColor forecolor, 
	AnsiTerm::TermBackColor backcolor, 
	TerminalWidget *wrapme
)
	: TerminalWidget(0, frame, attribs, forecolor, backcolor, wrapme->GetX()-1, wrapme->GetY()-1, wrapme->GetWidth()+2, wrapme->GetHeight()+1)
{
}

TerminalWidgetBox::~TerminalWidgetBox()
{
}

void TerminalWidgetBox::Draw( GC *gc )
{
	gc->SetAttributes( m_attribs );
	gc->SetBackColor( m_backcolor );
	gc->SetForeColor( m_forecolor );
	gc->DrawBox( m_x, m_y, m_width, m_height );
}

#ifdef DEBUG
void TerminalWidgetBox::ValidateMem() const { TerminalWidget::ValidateMem(); }
void TerminalWidgetBox::CheckMem() const { TerminalWidget::CheckMem(); }
#endif

/********** LIST BOX *********/

TerminalWidgetListBoxItem::TerminalWidgetListBoxItem( const String& text, int listid )
	: m_text(text), m_listid(listid), m_selected(false)
{
}
TerminalWidgetListBoxItem::TerminalWidgetListBoxItem( const char *text, int listid )
	: m_text(text), m_listid(listid), m_selected(false)
{
}
TerminalWidgetListBoxItem::~TerminalWidgetListBoxItem()
{
}

#ifdef DEBUG
void TerminalWidgetListBoxItem::ValidateMem() const { m_text.ValidateMem(); }
void TerminalWidgetListBoxItem::CheckMem() const { m_text.CheckMem(); }
#endif

TerminalWidgetListBox::TerminalWidgetListBox
(
	int taborder,
	TerminalFrame *frame, 
	int attribs, 
	AnsiTerm::TermForeColor forecolor, 
	AnsiTerm::TermBackColor backcolor, 
	int x, 
	int y, 
	int width, 
	int height,
	int colwidth,
	EVENTHANDLER activate
)
: TerminalWidget(taborder, frame, attribs, forecolor, backcolor, x, y, width, height),
m_items(), m_selected(NULL), m_colwidth(colwidth), m_startrow(0),
m_cx(x), m_cy(y), m_evcmd(activate)
{
	m_canFocus = true;
	int numcolumns = width / m_colwidth;
	if ( m_colwidth * numcolumns + (numcolumns-1) != width )
	{
		throw new InvalidArgumentException("Column width must be a multiple of width, plus a space between each column.");
	}
}

TerminalWidgetListBox::~TerminalWidgetListBox()
{
	List<TerminalWidgetListBoxItem *>::Iterator litr = m_items.Begin();

	while ( litr.Next() )
	{
		TerminalWidgetListBoxItem *item = litr.Current();
		delete item;
	}
	m_items.Clear();
}

void TerminalWidgetListBox::AddItem( const char *text, int id )
{
	TerminalWidgetListBoxItem *item = new TerminalWidgetListBoxItem(text, id);
	if ( NULL == item )
	{
		throw OutOfMemoryException();
	}
	m_items.Add( item );
	m_dirty = true;
}

TerminalWidgetListBoxItem *TerminalWidgetListBox::FindItem(const char *text)
{
	List<TerminalWidgetListBoxItem *>::Iterator litr = m_items.Begin();

	while ( litr.Next() )
	{
		TerminalWidgetListBoxItem *item = litr.Current();
		if ( item->GetText() == text )
		{
			return item;
		}
	}
	return NULL;
}

TerminalWidgetListBoxItem *TerminalWidgetListBox::FindItem(int id)
{
	List<TerminalWidgetListBoxItem *>::Iterator litr = m_items.Begin();

	while ( litr.Next() )
	{
		TerminalWidgetListBoxItem *item = litr.Current();
		if ( item->GetListId() == id )
		{
			return item;
		}
	}
	return NULL;
}

void TerminalWidgetListBox::RemoveItem( const String& text )
{
	List<TerminalWidgetListBoxItem *>::Iterator litr = m_items.Begin();

	while ( litr.Next() )
	{
		TerminalWidgetListBoxItem *item = litr.Current();
		if ( item->GetText() == text )
		{
			m_items.RemoveCurrent(litr);

			if ( item == m_selected )
			{
				m_selected = NULL;
			}
			m_dirty = true;
			return;
		}
	}
}

void TerminalWidgetListBox::RemoveItem( int id )
{
	List<TerminalWidgetListBoxItem *>::Iterator litr = m_items.Begin();

	while ( litr.Next() )
	{
		TerminalWidgetListBoxItem *item = litr.Current();

		if ( item->GetListId() == id )
		{
			m_items.RemoveCurrent(litr);
			if ( item == m_selected )
			{
				m_selected = NULL;
			}
			m_dirty = true;
			return;
		}
	}
}

void TerminalWidgetListBox::RemoveAll()
{
	m_selected = NULL;
	List<TerminalWidgetListBoxItem *>::Iterator litr = m_items.Begin();

	while ( litr.Next() )
	{
		TerminalWidgetListBoxItem *item = litr.Current();
		delete item;
	}
	m_items.Clear();
	m_dirty = true;
}

void TerminalWidgetListBox::SelectItem( const String& text )
{
	TerminalWidgetListBoxItem *item = FindItem( text );
	if ( NULL == item )
	{
		return;
	}
	if ( NULL != m_selected )
	{
		m_selected->UnSelect();
	}
	item->Select();
	m_selected = item;
	m_dirty = true;
}

void TerminalWidgetListBox::SelectItem( int id )
{
	TerminalWidgetListBoxItem *item = FindItem( id );
	if ( NULL == item )
	{
		return;
	}
	if ( NULL != m_selected )
	{
		m_selected->UnSelect();
	}
	item->Select();
	m_selected = item;
	m_dirty = true;
}

int TerminalWidgetListBox::Selected()
{
	if ( NULL == m_selected )
	{
		return -1;
	}
	return m_selected->GetListId();
}

String TerminalWidgetListBox::SelectedText()
{
	if ( NULL == m_selected )
	{
		return String();
	}
	return m_selected->GetText();
}

void TerminalWidgetListBox::CursorPosition(int *x, int *y) const
{
	*x = m_cx;
	*y = m_cy;
}

bool TerminalWidgetListBox::SetFocus( GC *gc )
{
	TerminalWidget::SetFocus( gc );
	/*if ( NULL == m_selected && 0 != m_items.Count() )
	{
		m_selected = m_items.Item(0);
		m_selected->Select();
	}*/
	m_dirty = true;
	return true;
}

bool TerminalWidgetListBox::TrapLF() { return true; }

bool TerminalWidgetListBox::HandleKey( GC *gc, char key )
{
	if ( key == LF )
	{
		if ( m_items.Count() == 0 )
		{
			return true;
		}
		if ( NULL != m_evcmd )
		{
			(*m_evcmd)(this, Selected(), m_frame);
		}
		if ( GetFrame()->IsVisible() )
		{
			Draw( gc );
		}
		return true;
	}
	return false;
}

bool TerminalWidgetListBox::HandleKeyCtrl( SpecialKey key )
{
	return false;
}

int TerminalWidgetListBox::SelectedIndex()
{
	if ( NULL == m_selected )
	{
		return -1;
	}
	int pos = 0;
	List<TerminalWidgetListBoxItem *>::Iterator litr = m_items.Begin();

	while ( m_selected != litr.Current() )
	{
		litr.Next();
		pos++;
	}

	return pos;
}

void TerminalWidgetListBox::SelectedRowCol(int *row, int *col)
{
	ASSERT( NULL != m_selected );
	int index = SelectedIndex();
	if ( index >= 0 )
	{
		*row = (index-m_startrow) % m_height;
		*col = (index-m_startrow) / m_height;
	}
}

void TerminalWidgetListBox::Page()
{
	int numcolumns = m_width / m_colwidth;
	int maxitems = numcolumns * m_height;
	m_startrow += maxitems;
	if ( m_startrow > m_items.Count() )
	{
		m_startrow = 0;	
	}
	else if ( m_startrow + maxitems >= m_items.Count() )
	{
		if ( 0 > (m_startrow = (m_items.Count() - maxitems)) )
		{
			m_startrow = 0;
		}
		else
		{
			m_startrow = m_startrow/m_height;
			m_startrow *= m_height;
		}
	}
	ASSERT( (m_startrow % m_height) == 0 );
	SelectRowCol(0, 0);
}

void TerminalWidgetListBox::SelectRowCol(int row, int col)
{
	ASSERT( (m_startrow % m_height) == 0 );
	if ( NULL != m_selected )
	{
		m_selected->UnSelect();
	}
	int maxColumn = m_items.Count() / m_height;
	if ( (m_items.Count() % m_height) != 0 )
	{
		maxColumn++;
	}
	int visiableColumns = m_width / m_colwidth;
	int visiableItems = visiableColumns * m_height;
	if ( row >= m_height )
	{
		// scroll to the top of the next column
		row = 0;
		col++;
	}
	else if ( 0 > row )
	{
		// scroll to the bottom of the prev column
		row = m_height-1;
		col--;
	}
	int firstVisiableColumn = m_startrow / m_height;
	int lastVisiableColumn = (m_startrow + visiableItems) / m_height;
	if ( col >= visiableColumns )
	{
		if ( col + firstVisiableColumn >= maxColumn )
		{
			if ((col+firstVisiableColumn) * m_height + row >= m_items.Count() )
			{
				row = 0;
			}
			m_startrow = 0;
			col = 0;
		}
		else
		{
			m_startrow += m_height;
			col--;
		}
		firstVisiableColumn = m_startrow / m_height;
		lastVisiableColumn = (m_startrow + visiableItems) / m_height;
	}
	else if ( col < 0 )
	{
		if ( firstVisiableColumn == 0 )
		{
			col = visiableColumns-1;
			m_startrow = maxColumn * m_height - visiableItems;
		}
		else
		{
			col = 0;
			m_startrow -= m_height;
		}
		firstVisiableColumn = m_startrow / m_height;
		lastVisiableColumn = (m_startrow + visiableItems) / m_height;
	}
	ASSERT( (m_startrow % m_height) == 0 );

	ASSERT( (m_startrow % m_height) == 0 );
	ASSERT ( col >= 0 && col < visiableColumns );

	int pos = (col+firstVisiableColumn) * m_height + row;
	if ( pos >= m_items.Count() )
	{
		pos = m_items.Count()-1;
	}
	if ( pos < m_startrow )
	{
		m_startrow -= m_height;
	}
	if ( pos - m_startrow >= visiableItems )
	{
		m_startrow += m_height;
	}
	ASSERT ( pos < m_items.Count() );

	m_selected = m_items.ElementAt(pos);
	m_selected->Select();
	m_cx = m_x + col * m_colwidth + (col);
	m_cy = m_y + row;
	ASSERT( (m_startrow % m_height) == 0 );
}

bool TerminalWidgetListBox::HandleKeyCmd( GC *gc, SpecialKey key )
{
	if ( m_items.Count() == 0 )
	{
		return false;
	}
	ASSERT( (m_startrow % m_height) == 0 );
	int row, col;
	SelectedRowCol(&row, &col);

	switch ( key )
	{
	case SKEY_DOWN:
		SelectRowCol(row+1, col);
		Draw( gc );
		return true;
	case SKEY_UP:
		SelectRowCol(row-1, col);
		Draw( gc );
		return true;
	case SKEY_LEFT:
		SelectRowCol(row, col-1);
		Draw( gc );
		return true;
	case SKEY_RIGHT:
		SelectRowCol(row, col+1);
		Draw( gc );
		return true;
	case SKEY_PGDN:
		Page();
		return true;
	case SKEY_PGUP:
		SelectRowCol(0, col-m_width/m_colwidth);
		return true;
	}
	return false;
}

void TerminalWidgetListBox::Draw( GC *gc )
{
	int columns = m_width / m_colwidth;
	int basex = m_x;
	int y = 0;
	int outcount = 0;
	int colwidth = m_width/columns;

	if ( NULL == m_selected && m_focus && m_items.Count() > 0 )
	{
		m_selected = m_items.ElementAt(0);
		m_selected->Select();
	}
	ASSERT( (m_startrow % m_height) == 0 );

	gc->SetAttributes( m_attribs );
	gc->SetBackColor( m_backcolor );
	gc->SetForeColor( m_forecolor );

	List<TerminalWidgetListBoxItem *>::Iterator litr = m_items.Begin();
	for ( int x = 0; x < m_startrow; x++ )
	{
		litr.Next();
	}

	for ( int c = 0; c < columns && outcount < m_items.Count(); c++ )
	{
		int hpos = m_height*(c+1);
		int count = (m_items.Count() > hpos) ? m_height : m_items.Count() - (m_height*c);
		ASSERT( count >= 0 );
		for ( y = 0; y < count; y++ )
		{
			outcount++;
			TerminalWidgetListBoxItem *item = litr.Current();
			litr.Next();

			if ( NULL == item )
			{
				ASSERT( c == columns-1 );
				while ( y < count )
				{
					gc->RepeatChar(colwidth + 1, ' ', basex, m_y+y );
					y++;
				}
				break;
			}

			ASSERT_MEM( item, sizeof(TerminalWidgetListBoxItem) );
			if ( m_focus && item->IsSelected() )
			{
				m_cx = basex;
				m_cy = m_y+y;
				gc->SetBackColor( TerminalColor::Green.AsTerminalBackColor() );
				gc->SetForeColor( TerminalColor::Black.AsTerminalForeColor() );
				//gc->SetAttributes(TerminalCell::TA_BOLD | m_attribs);
			}

			gc->WriteText( item->GetText().GetChars(), basex, m_y+y );
			int textlen = item->GetText().Length();
			if ( textlen < colwidth )
			{
				gc->RepeatChar(colwidth - textlen + 1, ' ', basex+textlen, m_y+y );
			}
			
			if ( m_focus && item->IsSelected() )
			{
				gc->SetAttributes( m_attribs );
				gc->SetBackColor( m_backcolor );
				gc->SetForeColor( m_forecolor );
			}
		}
		if ( count < m_height )
		{
			for ( y = count; y < m_height; y++ )
			{
				gc->RepeatChar(m_width, ' ', basex, m_y+y );
			}
		}
		basex += m_colwidth + 1;
		ASSERT( (m_startrow % m_height) == 0 );
	}
}

#ifdef DEBUG
void TerminalWidgetListBox::ValidateMem() const
{ 
	TerminalWidget::ValidateMem(); 
	m_items.ValidateMem();
	for( int x = 0; x < m_items.Count(); x++ )
	{
		TerminalWidgetListBoxItem *item = m_items.ElementAt(x);
		ASSERT_MEM( item, sizeof(TerminalWidgetListBoxItem) );
		item->ValidateMem();
	}
}

void TerminalWidgetListBox::CheckMem() const
{ 
	TerminalWidget::CheckMem(); 
	m_items.CheckMem();
	for( int x = 0; x < m_items.Count(); x++ )
	{
		TerminalWidgetListBoxItem *item = m_items.ElementAt(x);
		DEBUG_NOTE_MEM_ALLOCATION( item );
		item->CheckMem();
	}
}

#endif

/********** FRAME ***********/
class TerminalFrameManager;

TerminalFrame::TerminalFrame(TerminalFrameManager *cp, int w, int h, AnsiTerm::TermBackColor background)
: m_background(background), m_sendbuf(), m_mtx()
{
	ASSERT( w == 80 && h == 25 );
	m_width = w;
	m_height = h;
	m_fman = cp;
	memset( m_fnHandlers, 0, sizeof( m_fnHandlers ) );
	m_hasFocus = NULL;
	memset( m_taborder, 0, sizeof(m_taborder) );
}

TerminalFrame::~TerminalFrame()
{
	DEBUG_VALIDATE();

	m_widgets.ValidateMem();
	int count = m_widgets.Count();
	for ( int x = 0; x < count; x++ )
	{
		TerminalWidget *ctl = m_widgets.ElementAt(x);
		ASSERT_PTR( ctl );
		ctl->ValidateMem();
		delete ctl;
	}
	m_widgets.Clear();
	m_widgets.ValidateMem();

	DEBUG_VALIDATE();
}

void TerminalFrame::SendToTerm( IStream& sp, bool forceupdate )
{
	m_mtx.Lock();
	ValidateMem();
	m_sendbuf.Clear();
	//if ( sp->IsClosed() )
	//{
	//	m_mtx.Unlock();
	//	return;
	//}
	GC *gc = m_fman->GetGC();
	int count = m_widgets.Count();
	for ( int x = 0; x < count; x++ )
	{
		TerminalWidget *ctl = m_widgets.ElementAt(x);
		if ( ctl->IsDirty() || forceupdate )
		{
			ctl->Draw( gc );
		}
	}
	gc->UpdateToString( &m_sendbuf );
	//gc->SetCursor(&m_sendbuf, 0, 0);
	if ( NULL != m_hasFocus )
	{
		int x;
		int y;
		m_hasFocus->CursorPosition(&x, &y);
		gc->SetCursor(&m_sendbuf, x, y);
	}
	sp.Write( m_sendbuf.ToByteArray() );
	m_mtx.Unlock();
}

void TerminalFrame::Redraw()
{
	m_mtx.Lock();
	ValidateMem();
	GC *gc = m_fman->GetGC();
	gc->SetAttributes( 0 );
	gc->SetForeColor( AnsiTerm::FORE_GREEN );
	gc->SetBackColor( m_background );
	gc->ClearScreen();
	int count = m_widgets.Count();
	for ( int x = 0; x < count; x++ )
	{
		m_widgets.ElementAt(x)->Draw( gc );
	}
	m_mtx.Unlock();
}

void TerminalFrame::Resize(int minx, int maxx, int miny, int maxy)
{
	if ( maxx == m_width && maxy == m_height )
	{
		return;
	}
	m_mtx.Lock();

	//BitMask mask(maxy, maxx);
	int count = m_widgets.Count();
	int x;
	for ( x = 0; x < count; x++ )
	{
		TerminalWidget *widget = m_widgets.ElementAt(x);
		widget->ResizeColumns(m_width, maxx);
		//widget->AssertMask( &mask );
		//widget->WriteMask( &mask );
	}
	//mask.Clear();
	for ( x = 0; x < count; x++ )
	{
		TerminalWidget *widget = m_widgets.ElementAt(x);
		widget->ResizeRows(m_height, maxy);
		//widget->AssertMask( &mask );
		//widget->WriteMask( &mask );
	}
	m_width = maxx;
	m_height = maxy;
	m_mtx.Unlock();
}

void TerminalFrame::Invalidate() 
{ 
	m_mtx.Lock();
	int count = m_widgets.Count();
	for ( int x = 0; x < count; x++ )
	{
		m_widgets.ElementAt(x)->SetDirty();
	}
	m_mtx.Unlock();
}

bool TerminalFrame::IsVisible() 
{ 
	return m_fman->CurrentFrame() == this; 
}

void TerminalFrame::SetFocus( TerminalWidget *control, bool redraw )
{
	GC *gc = (redraw) ? m_fman->GetGC() : NULL;
	if ( NULL != m_hasFocus )
	{
		m_hasFocus->LostFocus( gc );
	}
	m_hasFocus = control;
	m_hasFocus->SetFocus( gc );
}

void TerminalFrame::HandleKeyTab()
{
	ValidateMem();
	int x;
	int pos = 0;
	GC *gc = m_fman->GetGC();
	if ( NULL != m_hasFocus )
	{
		if( m_hasFocus->TrapTabKey() )
		{
			m_hasFocus->HandleKeyCmd( gc, SKEY_TAB );
			return;
		}
		m_hasFocus->LostFocus( gc );
		pos = m_hasFocus->GetTabOrder() - 1;
	}
	for ( x = pos+1; x < 255; x++ )
	{
		if ( NULL != (m_hasFocus = m_taborder[x]) )
		{
			m_hasFocus->SetFocus( gc );
			return;
		}
	}
	for ( x = 0; x <= pos; x++ )
	{
		if ( NULL != (m_hasFocus = m_taborder[x]) )
		{
			m_hasFocus->SetFocus( gc );
			return;
		}
	}
}

void TerminalFrame::HandleKeyControl( SpecialKey ch )
{
	ValidateMem();
	int count = m_widgets.Count();
	for ( int x = 0; x < count; x++ )
	{
		TerminalWidget *tw = m_widgets.ElementAt(x);
		if ( tw->HandleKeyCtrl( ch ) )
		{
			return;
		}
	}
}

void TerminalFrame::HandleKey( char ch )
{
	ValidateMem();
	if ( NULL != m_hasFocus )
	{
		m_hasFocus->HandleKey( m_fman->GetGC(), ch );
	}
}

void TerminalFrame::HandleKeyCmd( SpecialKey ch )
{
	GC *gc = m_fman->GetGC();
	ValidateMem();
	if ( NULL != m_hasFocus )
	{
		if ( ! m_hasFocus->HandleKeyCmd( gc, ch ) )
		{
			switch ( ch )
			{
			case SKEY_DOWN:
				this->HandleKeyTab();
				break;
			case SKEY_UP:
				int x;
				int pos = 0;
				if ( NULL != m_hasFocus )
				{
					m_hasFocus->LostFocus( gc );
					pos = m_hasFocus->GetTabOrder() - 1;
				}
				for ( x = pos+1; x >= 0; x-- )
				{
					if ( NULL != (m_hasFocus = m_taborder[x]) )
					{
						m_hasFocus->SetFocus( gc );
						return;
					}
				}
				for ( x = 255; x >= pos; x-- )
				{
					if ( NULL != (m_hasFocus = m_taborder[x]) )
					{
						m_hasFocus->SetFocus( gc );
						return;
					}
				}
				break;
			}
		}
	}
}

void TerminalFrame::HandleKeyFN( SpecialKey key )
{
	ValidateMem(); 
	int index = (int)(key - SKEY_F1);
	ASSERT( index > -1 && index < 24 );
	if( m_fnHandlers[index] != NULL ) 
	{
		(*m_fnHandlers[index])(NULL, key, m_fman);
	}
}

void TerminalFrame::HandleKeyLF()
{
	ValidateMem();
	if ( NULL == m_hasFocus )
	{
		return;
	}
	if ( ! m_hasFocus->TrapLF() )
	{
		HandleKeyTab();
	}
	else
	{
		m_hasFocus->HandleKey(m_fman->GetGC(), '\n');
	}
}

void TerminalFrame::AddWidget( TerminalWidget *control )
{
	ValidateMem();
	control->SetZorder( m_widgets.Count() );
	/*GC *gc = m_fman->GetGC();*/
	if ( control->SetFocus(NULL) )
	{
		if ( NULL != m_hasFocus )
		{
			m_hasFocus->LostFocus(NULL);
		}
		m_hasFocus = control;
	}
	m_widgets.Add( control );
	FixupTabOrder();
}

void TerminalFrame::RemoveWidget( TerminalWidget *control )
{
	ValidateMem();
	ASSERT_PTR( control );
	control->ValidateMem();

	m_widgets.RemoveAt( control->GetZorder() );
	delete control;

	//Redraw();
	FixupTabOrder();
}

void TerminalFrame::RemoveAllWidgets()
{
	ValidateMem();
	m_widgets.ValidateMem();
	int count = m_widgets.Count();
	for ( int x = 0; x < count; x++ )
	{
		TerminalWidget *ctl = m_widgets.ElementAt(x);
		ASSERT_PTR( ctl );
		ctl->ValidateMem();
		delete ctl;
	}
	m_widgets.Clear();
	m_widgets.ValidateMem();

	//Redraw();
	FixupTabOrder();
}

void TerminalFrame::WidgetToFront( TerminalWidget *control )
{
	ValidateMem();
	m_widgets.RemoveAt( control->GetZorder() );
	control->SetZorder( m_widgets.Count() );
	m_widgets.Add( control );
	GC *gc = m_fman->GetGC();
	if ( control->SetFocus(gc) )
	{
		if ( NULL != m_hasFocus )
		{
			m_hasFocus->LostFocus(gc);
		}
		m_hasFocus = control;
	}
	control->Draw( gc );
}

void TerminalFrame::FixupTabOrder()
{
	ValidateMem();
	memset( m_taborder, 0, sizeof(m_taborder) );
	int count = m_widgets.Count();
	for ( int x = 0; x < count; x++ )
	{
		TerminalWidget *tw = m_widgets.ElementAt(x);
		ASSERT( tw->GetTabOrder() >= 0 && tw->GetTabOrder() < 255 );
		if ( tw->GetTabOrder() > 0 )
		{
			m_taborder[tw->GetTabOrder()-1] = tw;
		}
	}
}

#ifdef DEBUG
void TerminalFrame::ValidateMem() const
{
	if ( NULL != m_hasFocus )
	{
		ASSERT_PTR( m_hasFocus );
	}
	m_sendbuf.ValidateMem();
	ASSERT_PTR( m_fman );
	m_widgets.ValidateMem();
	int count = m_widgets.Count();
	for ( int x = 0; x < count; x++ )
	{
		TerminalWidget *tw = m_widgets.ElementAt(x);
		ASSERT_PTR( tw );
		tw->ValidateMem();
	}
}

void TerminalFrame::CheckMem() const
{
	m_sendbuf.CheckMem();
	m_widgets.CheckMem();
	int count = m_widgets.Count();
	for ( int x = 0; x < count; x++ )
	{
		TerminalWidget *tw = m_widgets.ElementAt(x);
		DEBUG_NOTE_MEM_ALLOCATION( tw );
		tw->CheckMem();
	}
}
#endif

TerminalFrameManager::TerminalFrameManager( TcpSocketPtr sock, SocketSet& ss ) 
:	m_strArgs(), 
	m_frames(), 
	m_framestack(), 
	m_vframes(), 
	m_gc(NULL, 80, 25)
{
	m_curframe = NULL;
	m_buf8Csi[0] = ESC;
	m_buf8Csi[1] = CSI;

	m_term = new AnsiTerm(sock, ss, this);
}

TerminalFrameManager::TerminalFrameManager( IStreamPtr conn ) 
:	m_strArgs(), 
	m_frames(), 
	m_framestack(), 
	m_vframes(), 
	m_gc(NULL, 80, 25)
{
	m_curframe = NULL;
	m_buf8Csi[0] = ESC;
	m_buf8Csi[1] = CSI;

	m_term = new AnsiTerm(conn, this);
}

TerminalFrameManager::TerminalFrameManager( ITerminal *term ) 
:	m_strArgs(), 
	m_frames(), 
	m_framestack(), 
	m_vframes(), 
	m_gc(NULL, 80, 25)
{
	m_curframe = NULL;
	m_buf8Csi[0] = ESC;
	m_buf8Csi[1] = CSI;

	ASSERT_PTR( term );
	term->ValidateMem();

	m_term = term;
	m_term->AddListener( this );
}

TerminalFrameManager::~TerminalFrameManager()
{
	ClearFrames();
	delete m_term;
}

void TerminalFrameManager::ClearFrames()
{
	int fcount = m_vframes.Count();
	for ( int x = 0; x < fcount; x++ )
	{
		TerminalFrame *tf = m_vframes.ElementAt(x);
		ASSERT_PTR( tf );
		delete tf;
	}
	m_vframes.Clear();
	m_frames.Clear();
	m_framestack.Clear();
	m_gc.ClearScreen();
	m_curframe = NULL;
}

void TerminalFrameManager::AddFrame( TerminalFrame *frame, const String name)
{
	ASSERT_PTR( frame );
	m_gc.SetTermCaps(m_term->GetTermCaps());
	m_frames.Set( name, frame );
	m_vframes.Add( frame );
}

/*
void TerminalFrameManager::OnNegotationComplete()
{
	ASSERT( m_caps.m_eos == 0xcd );

	DEBUG_CLEAR_MEM_CHECK_POINTS();
	DEBUG_DUMP_MEM_LEAKS();
	DEBUG_ASSERT_MEM_NOTED();

	ASSERT_MEM( this, sizeof(TerminalFrameManager) );
	ValidateMem();

	//setTermModeAnsi();
	QueryTermType();
	SetTextModeColor80X25();
	//setTextModeGfx();
	DisableLineWrap();
	EnableKeyPadTransmit();

	ASSERT( m_caps.m_eos == 0Xcd );
	ASSERT_PTR( &m_caps );
	ASSERT_MEM( this, sizeof(TerminalFrameManager) );
	ValidateMem();

	if ( m_caps.m_hasS0S1 != 0 )
	{
		SetCharSetS1();
	}
}*/

void TerminalFrameManager::OnTermOpen()
{
}

void TerminalFrameManager::OnTermClose()
{
}

void TerminalFrameManager::OnTermError(const String msg)
{
}

void TerminalFrameManager::OnTermKey(SpecialKey ch)
{
	ValidateMem();
	if ( NULL == m_curframe )
	{
		return;
	}
	ASSERT_PTR( m_curframe );

	switch ( ch )
	{
	case SKEY_TAB:
		m_curframe->HandleKeyTab();
		break;
	case SKEY_LF:
		if ( m_cr == '\n' )
		{
			m_curframe->HandleKeyLF(  );
		}
		break;
	case SKEY_CR:
		if ( m_cr == '\r' )
		{
			m_curframe->HandleKeyLF(  );
		}
		break;
	case SKEY_CTRLA:
	case SKEY_CTRLB:
	case SKEY_CTRLC:
	case SKEY_CTRLD:
	case SKEY_CTRLE:
	case SKEY_CTRLF:
	case SKEY_CTRLG:
	case SKEY_CTRLH:
	case SKEY_CTRLI:
	case SKEY_CTRLJ:
	case SKEY_CTRLK:
	case SKEY_CTRLL:
	case SKEY_CTRLM:
	case SKEY_CTRLN:
	case SKEY_CTRLO:
	case SKEY_CTRLP:
	case SKEY_CTRLQ:
	case SKEY_CTRLR:
	case SKEY_CTRLS:
	case SKEY_CTRLT:
	case SKEY_CTRLU:
	case SKEY_CTRLV:
	case SKEY_CTRLW:
	case SKEY_CTRLX:
	case SKEY_CTRLY:
	case SKEY_CTRLZ:
		m_curframe->HandleKeyControl( ch );
		break;
	case SKEY_F1:
	case SKEY_F2:
	case SKEY_F3:
	case SKEY_F4:
	case SKEY_F5:
	case SKEY_F6:
	case SKEY_F7:
	case SKEY_F8:
	case SKEY_F9:
	case SKEY_F10:
	case SKEY_F11:
	case SKEY_F12:
		m_curframe->HandleKeyFN( ch );
		break;
	case SKEY_SF1:
	case SKEY_SF2:
	case SKEY_SF3:
	case SKEY_SF4:
	case SKEY_SF5:
	case SKEY_SF6:
	case SKEY_SF7:
	case SKEY_SF8:
	case SKEY_SF9:
	case SKEY_SF10:
	case SKEY_SF11:
	case SKEY_SF12:
		m_curframe->HandleKeyFN( ch );
		break;
	default:
		m_curframe->HandleKeyCmd( ch );
		break;
	}
}

void TerminalFrameManager::OnTermKey(char ch)
{
	if ( NULL == m_curframe )
	{
		return;
	}
	ASSERT_PTR( m_curframe );
	m_curframe->HandleKey( ch );
}

void TerminalFrameManager::OnTermKeys(const char *buf, const int len)
{
	if ( NULL == m_curframe )
	{
		return;
	}
	ASSERT_PTR( m_curframe );
	for ( int x = 0; x < len; x++ )
	{
		m_curframe->HandleKey( buf[x] );
	}
}

void TerminalFrameManager::OnTermSize( int rows, int cols )
{
}

void TerminalFrameManager::OnTermType(const String termname)
{
	char buf[3];

	GetTermCaps()->SetTerm(termname);
	m_term->IndentifyTermCaps();
	if ( GetTermCaps()->GetCapString(TCAP_cr, buf, 3) )
	{
		m_cr = buf[0];
	}
	else
	{
		m_cr = '\r';
	}
	m_gc.SetTermCaps(m_term->GetTermCaps());

	//m_term->SendEnableKeyPadTransmit();
	//if ( m_term->GetTermCaps()->HasS0S1() )
	//{
	//	m_term->SendSetCharSetS0();
	//}
	m_term->SendNAWS();
	m_term->SendCharAtaTimeMode();
	m_term->SendDisableLineWrap();
}

void TerminalFrameManager::OnTermCursorPos(int row, int col)
{
	Log::WriteInfo("TerminalFrameManager::OnTermCursorPos(%d,%d)", row, col);
}

void TerminalFrameManager::OnTermChanged()
{
	if ( NULL != m_curframe )
	{
		ASSERT_PTR(m_curframe);
		m_curframe->UpdateTerm( (spl::IStream&)m_term->GetStream() );
	}
}

void TerminalFrameManager::OnTermWindowSize(int minx, int maxx, int miny, int maxy)
{
	if ( minx != 0 || miny != 0 )
	{
		Log::WriteWarn("Resize(%d,%d,%d,%d)", minx, maxx, miny, maxy);
	}
	if ( m_gc.Width() == maxx && m_gc.Height() == maxy )
	{
		return;
	}
	m_gc.Resize(maxx, maxy);

	// reconfigure frames
	int count = m_vframes.Count();
	for ( int x = 0; x < count; x++ )
	{
		TerminalFrame *frame = m_vframes.ElementAt(x);
		frame->Resize(minx, maxx, miny, maxy);
	}
	ASSERT_PTR( m_curframe );
	m_curframe->Redraw();
}

void TerminalFrameManager::PushFrame( const String frametoshow )
{
	ValidateMem();

	TerminalFrame *frame = m_frames.Get( frametoshow );
	if ( NULL == frame )
	{
		throw new InvalidArgumentException("Undefined frame");
	}
	ASSERT_PTR(frame);
	m_term->SendClearScreen();
	ASSERT_PTR( m_curframe );
	m_framestack.Add( m_curframe );
	m_curframe = frame;
	ASSERT_PTR( m_curframe );
	m_gc.ClearScreen();
	ASSERT_PTR( m_curframe );
	m_curframe->Invalidate();
}

void TerminalFrameManager::PopFrame()
{
	ValidateMem();

	m_term->SendClearScreen();
	m_curframe = m_framestack.Pop();
	ASSERT_PTR( m_curframe );
	m_gc.ClearScreen();
	m_curframe->Invalidate();
}

void TerminalFrameManager::SetFrame( const String frametoshow )
{
	ValidateMem();

	TerminalFrame *frame = m_frames.Get( frametoshow );
	if ( NULL == frame )
	{
		throw new InvalidArgumentException("Undefined frame");
	}
	m_term->SendClearScreen();
	m_framestack.Clear();
	m_curframe = frame;
	ASSERT_PTR( m_curframe );
	m_curframe->Invalidate();
	m_gc.ClearScreen();
}

TerminalFrame *TerminalFrameManager::GetFrame( const String framename )
{
	return m_frames.Get( framename );
}

void TerminalFrameManager::Send( const byte *b, const int len )
{
	m_term->Send(b, len);
}

#ifdef DEBUG
void TerminalFrameManager::ValidateMem() const
{
	ASSERT_PTR( m_term );
	m_term->ValidateMem();
	
	m_strArgs.ValidateMem();
	m_frames.ValidateMem();
	m_vframes.ValidateMem();
	m_framestack.ValidateMem();
	m_gc.ValidateMem();
	for ( int x = 0; x < m_vframes.Count(); x++ )
	{
		ASSERT_PTR ( m_vframes.ElementAt(x) );
		m_vframes.ElementAt(x)->ValidateMem();
	}
	if ( NULL != m_curframe )
	{
		ASSERT_PTR( m_curframe );
	}
}

void TerminalFrameManager::CheckMem() const
{
	DEBUG_VALIDATE();

	//DEBUG_NOTE_MEM_ALLOCATION( m_term );
	//m_term->CheckMem();

	m_strArgs.CheckMem();
	m_vframes.CheckMem();
	m_frames.CheckMem();
	m_framestack.CheckMem();
	m_gc.CheckMem();

	for ( int x = 0; x < m_vframes.Count(); x++ )
	{
		DEBUG_NOTE_MEM_ALLOCATION ( m_vframes.ElementAt(x) );
		m_vframes.ElementAt(x)->CheckMem();
	}
}
#endif

