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
#ifndef _textterm_h
#define _textterm_h

#include <spl/types.h>
#include <spl/Debug.h>

#ifdef _WINDOWS
#include <spl/configwin32.h>
#else
#include <spl/autoconf/config.h>
#endif

#include <spl/math/BitMask.h>
#include <spl/Memory.h>
#include <spl/net/Socket.h>
#include <spl/io/IStream.h>
#include <spl/term/ansicodes.h>
#include <spl/text/StringBuffer.h>
#include <spl/collection/Vector.h>
#include <spl/collection/Hashtable.h>
#include <spl/collection/List.h>
#include <spl/io/log/Log.h>
#include <spl/Exception.h>
#include <spl/threading/Mutex.h>
#include <spl/term/TermCap.h>
#include <spl/term/Telnet.h>
#include <spl/term/AnsiTerm.h>

namespace spl
{

class TerminalCell
{
protected:
	char m_ch;
	int m_attribs;
	unsigned m_bold : 1;
	unsigned m_under : 1;
	unsigned m_blink : 1;
	unsigned m_reverse : 1;
	unsigned m_invis : 1;
	unsigned m_acs : 1;

	bool m_updated;

	AnsiTerm::TermForeColor m_foreColor;
	AnsiTerm::TermBackColor m_backColor;

public:
	TerminalCell();
	virtual ~TerminalCell();

	inline void SetChar( char ch )
	{
		if ( ch != m_ch )
		{
			m_ch = ch;
			m_updated = true;
		}
	}

	inline char GetChar() const
	{
		return m_ch;
	}

	typedef enum _TermCellAttrib {
		TA_BOLD = (1<<1),
		TA_UNDER = (1<<2),
		TA_BLINK = (1<<3),
		TA_REVRS = (1<<4),
		TA_INVIS = (1<<5),
		TA_ACS = (1<<6)
	} TermCellAttrib;

	inline int GetAttributes() const
	{
		ASSERT ( m_attribs == ((m_bold?TA_BOLD:0) | (m_under?TA_UNDER:0) | 	(m_blink?TA_BLINK:0) | (m_reverse?TA_REVRS:0) | (m_invis?TA_INVIS:0) | (m_acs?TA_ACS:0)) );
		return m_attribs;
	}
	
	inline AnsiTerm::TermForeColor GetForeColor() const { return m_foreColor; }
	inline void SetForeColor( AnsiTerm::TermForeColor color ) { if ( m_foreColor == color ) return; m_foreColor = color; m_updated = true; }
	inline AnsiTerm::TermBackColor GetBackColor() const { return m_backColor; }
	inline void SetBackColor( AnsiTerm::TermBackColor color ) { if ( m_backColor == color ) return; m_backColor = color; m_updated = true; }
	inline bool IsDirty() { return m_updated; }
	inline void SetDirty(bool val) { m_updated = val; }

	void SetAttributes( int attr );
	void Clear();
	void WriteText( StringBuffer *strbuf, TermCap *termcaps );
	void WriteAttributes( StringBuffer *strbuf, TermCap *termcaps, char *tmpbuf, int tmpbuflen );
};

#define GC_CMD_BUF_SIZE 128

class GC : public IMemoryValidate
{
protected:
	void SetCell( TerminalCell *cell );

	TermCap *m_termcaps;
	char m_cmdbuf[GC_CMD_BUF_SIZE];
	TerminalCell *m_cells;
	int m_width;
	int m_height;
	int m_attribs;
	AnsiTerm::TermForeColor m_forecolor;
	AnsiTerm::TermBackColor m_backcolor;

public:
	GC( TermCap *capsbuf, int width = 80, int height = 25 );
	virtual ~GC();

	inline int Width() { return m_width; }
	inline int Height() { return m_height; }
	inline int GetAttributes() const { return m_attribs; }
	inline void SetAttributes( int attr ) { m_attribs = attr; }
	inline AnsiTerm::TermForeColor GetForeColor() const	{ return m_forecolor; }
	inline void SetForeColor( AnsiTerm::TermForeColor color ) { m_forecolor = color; }
	inline AnsiTerm::TermBackColor GetBackColor() const { return m_backcolor; }
	inline void SetBackColor( AnsiTerm::TermBackColor color ) { m_backcolor = color; }
	inline TermCap *GetTermcap() { return m_termcaps; }
	inline void SetTermCaps(TermCap *caps) { m_termcaps = caps; }
	
	void Resize( int width, int height );
	void ClearScreen();
	void Invalidate();
	void DrawBox( int x, int y, int width, int height );
	void DrawHLine( int x, int y, int length ); 
	void DrawVLine( int x, int y, int length );
	void WriteText( const char *ch, int x, int y );
	inline void WriteText( const String& str, int x, int y ) { WriteText(str.GetChars(), x, y); }
	void WriteText( byte ch, int x, int y );
	void RepeatChar( int count, char ch, int x, int y );
	void UpdateToString( StringBuffer *sbuf );

	void SetCursor( StringBuffer *sbuf, int x, int y );
	inline char CharAt( int x, int y ) { return m_cells[y * m_width + x].GetChar(); }

#ifdef DEBUG
	void ValidateMem() const;
	void CheckMem() const;
#else
	inline void ValidateMem() const {}
	inline void CheckMem() const {}
#endif
};

class TerminalFrame;
class TerminalFrameManager;

class TerminalWidget : public IMemoryValidate
{
protected:
	TerminalFrame *m_frame;
	int m_taborder;
	bool m_canFocus;
	bool m_focus;
	int m_x;
	int m_y;
	int m_width;
	int m_height;
	int m_zorder;
	int m_attribs;
	AnsiTerm::TermForeColor m_forecolor;
	AnsiTerm::TermBackColor m_backcolor;
	byte m_bufcsi[32];
	bool m_dirty;
	bool m_ancLeft, m_ancRight, m_ancTop, m_ancBottom;

public:
	TerminalWidget(
		int taborder,			// zero means no tabbing
		TerminalFrame *frame, 
		int attribs, 
		AnsiTerm::TermForeColor forecolor, 
		AnsiTerm::TermBackColor backcolor, 
		int x, 
		int y, 
		int width, 
		int height
	);
	virtual ~TerminalWidget();

	inline void SetZorder(int zorder) { m_zorder = zorder; }
	inline int GetZorder() { return m_zorder; }
	inline int GetTabOrder() { return m_taborder; }
	inline bool CanGetFocus() { return m_canFocus; }
	inline bool IsDirty() { return m_dirty; }
	inline void SetDirty() { m_dirty = true; }
	inline TerminalFrame *GetFrame() { return m_frame; }

	virtual void Draw( GC *gc ) = 0;

	virtual bool HandleKey( GC *gc, const char key );
	virtual bool HandleKeyCtrl( SpecialKey key );
	virtual bool HandleKeyCmd( GC *gc, SpecialKey key );
	virtual bool TrapTabKey();
	virtual bool TrapLF();

	virtual bool SetFocus( GC *gc );
	virtual void LostFocus( GC *gc );
	virtual void CursorPosition( int *x, int *y ) const;

	inline int GetX() { return m_x; }
	inline int GetY() { return m_y; }
	inline int GetWidth() { return m_width; }
	inline int GetHeight() { return m_height; }

	inline void SetAnchors(bool left, bool right, bool top, bool bottom) {m_ancLeft = left; m_ancRight = right; m_ancTop = top; m_ancBottom = bottom; }
	inline bool IsAnchored() {return m_ancLeft || m_ancRight || m_ancTop || m_ancBottom; }
	inline bool IsAnchorLeft() { return m_ancLeft; }
	inline bool IsAnchorRight() { return m_ancRight; }
	inline bool IsAnchorTop() { return m_ancTop; }
	inline bool IsAnchorBottom() { return m_ancBottom; }

	void ResizeColumns(int oldScreenWidth, int newScreenWidth);
	void ResizeRows(int oldScreenHeight, int newScreenHeight);

	void WriteMask( BitMask *bm );
	void AssertMask( BitMask *bm );

#ifdef DEBUG
	virtual void ValidateMem() const = 0;
	virtual void CheckMem() const = 0;
#else
	inline void ValidateMem() const {}
	inline void CheckMem() const {}
#endif
};

typedef bool (*EVENTHANDLER)( TerminalWidget *source, int id, void *arg );

class TerminalWidgetMenuBarItem : public IMemoryValidate
{
public:
	TerminalWidgetMenuBarItem( int x, int y, char *name, int id, EVENTHANDLER handler );
	virtual ~TerminalWidgetMenuBarItem();

	inline SpecialKey HotKey() { return m_ctrlkey; }
	bool OnEvent(TerminalWidget *parent);
	inline int Width() { return m_name.Length() - 1 + 4; }
	void Draw( GC *gc );
	void SetText( const char *text );

#ifdef DEBUG
	virtual void ValidateMem() const;
	virtual void CheckMem() const;
#else
	inline void ValidateMem() const {}
	inline void CheckMem() const {}
#endif

protected:
	int m_x, m_y;
	SpecialKey m_ctrlkey;
	String m_name;
	int m_id;
	EVENTHANDLER m_handler;
};

class TerminalWidgetMenuBar : public TerminalWidget
{
public:
	TerminalWidgetMenuBar
	(
		char *title, 
		TerminalFrame *frame, 
		int attribs, 
		AnsiTerm::TermForeColor forecolor, 
		AnsiTerm::TermBackColor backcolor, 
		int x, 
		int y, 
		int width
	);
	virtual ~TerminalWidgetMenuBar();

	void AddItem( char *name, int id, EVENTHANDLER handler );
	void SetItemText( int idx, char *name ); 
	void Draw( GC *gc );
	bool HandleKeyCtrl( SpecialKey key );

#ifdef DEBUG
	virtual void ValidateMem() const;
	virtual void CheckMem() const;
#else
	inline void ValidateMem() const {}
	inline void CheckMem() const {}
#endif

protected:
	char m_title[81];
	int m_newItemPos;
	Vector<TerminalWidgetMenuBarItem *>m_items;
};

class TerminalWidgetStatusBarPanel : public IMemoryValidate
{
public:
	TerminalWidgetStatusBarPanel( int len );
	virtual ~TerminalWidgetStatusBarPanel();

	void SetText( const char *text );
	inline char *GetText( ) { return m_text; }
	inline int Length() { return m_len; }

#ifdef DEBUG
	virtual void ValidateMem() const;
	virtual void CheckMem() const;
#else
	inline void ValidateMem() const {}
	inline void CheckMem() const {}
#endif

protected:
	int m_len;
	char m_text[81];
};

class TerminalWidgetStatusBar : public TerminalWidget
{
public:
	TerminalWidgetStatusBar
	(
		TerminalFrame *frame, 
		int attribs, 
		AnsiTerm::TermForeColor forecolor, 
		AnsiTerm::TermBackColor backcolor, 
		int x, 
		int y, 
		int width
	);
	virtual ~TerminalWidgetStatusBar();
	
	int AddPanel( int width );
	void SetPanelText( int panel, const char *text );
	void Draw( GC *gc );

#ifdef DEBUG
	virtual void ValidateMem() const;
	virtual void CheckMem() const;
#else
	inline void ValidateMem() const {}
	inline void CheckMem() const {}
#endif

protected:
	Vector<TerminalWidgetStatusBarPanel *>m_panels;
};

class TerminalWidgetLabel : public TerminalWidget
{
public:
	TerminalWidgetLabel
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
	);
	virtual ~TerminalWidgetLabel();
	void Draw( GC *gc );
	void SetText( const char *text );

#ifdef DEBUG
	virtual void ValidateMem() const;
	virtual void CheckMem() const;
#else
	inline void ValidateMem() const {}
	inline void CheckMem() const {}
#endif

protected:
	char m_text[81];
	bool m_centered;
};

class TerminalWidgetTextBox : public TerminalWidget
{
protected:
	int m_caretPos;
	StringBuffer m_text;
	int m_activeattribs;
	AnsiTerm::TermForeColor m_activeforecolor;
	AnsiTerm::TermBackColor m_activebackcolor;

public:
	TerminalWidgetTextBox
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
	);
	virtual ~TerminalWidgetTextBox();

	bool SetFocus( GC *gc );
	void LostFocus( GC *gc );
	void Draw( GC *gc );
	virtual bool HandleKey( GC *gc, const char key );
	virtual bool HandleKeyCmd( GC *gc, SpecialKey key );

	virtual void CursorPosition( int *x, int *y ) const;

	inline StringPtr GetText() { return m_text.ToString()->RTrim(); }
	void ClearText();

#ifdef DEBUG
	virtual void ValidateMem() const;
	virtual void CheckMem() const;
#endif
};

class TerminalWidgetActionTextBox : public TerminalWidgetTextBox
{
public:
	TerminalWidgetActionTextBox
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
	);
	virtual ~TerminalWidgetActionTextBox();

	virtual bool TrapLF();
	virtual void LostFocus( GC *gc );
	bool HandleKey( GC *gc, const char key );

protected:
	EVENTHANDLER m_handler;
};

class TerminalWidgetButton : public TerminalWidget
{
public:
	TerminalWidgetButton
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
	);
	virtual ~TerminalWidgetButton();

	bool SetFocus( GC *gc );
	void LostFocus( GC *gc );
	void Draw( GC *gc );
	bool HandleKey( GC *gc, const char key );
	bool HandleKeyCtrl( SpecialKey key );
	bool TrapLF();

	void CursorPosition( int *x, int *y ) const;

#ifdef DEBUG
	virtual void ValidateMem() const;
	virtual void CheckMem() const;
#else
	inline void ValidateMem() const {}
	inline void CheckMem() const {}
#endif

protected:
	char m_text[81];
	SpecialKey m_hotkey;
	EVENTHANDLER m_handler;
	int m_activeattribs;
	AnsiTerm::TermForeColor m_activeforecolor;
	AnsiTerm::TermBackColor m_activebackcolor;
};

class TerminalWidgetSpinner : public TerminalWidget
{
public:
	TerminalWidgetSpinner
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
	);
	virtual ~TerminalWidgetSpinner();
	bool SetFocus( GC *gc );
	void LostFocus( GC *gc );
	void Draw( GC *gc );
	bool HandleKey( GC *gc, const char key );
	bool HandleKeyCmd( GC *gc, SpecialKey key );
	void AddOption( const char *val );
	inline void AddOption( const String& val ) { AddOption(val.GetChars()); }

	void CursorPosition( int *x, int *y ) const;
	inline StringPtr GetText() { return m_options.ElementAt(m_curopt); }

#ifdef DEBUG
	virtual void ValidateMem() const;
	virtual void CheckMem() const;
#else
	inline void ValidateMem() const {}
	inline void CheckMem() const {}
#endif

protected:
	int m_activeattribs;
	AnsiTerm::TermForeColor m_activeforecolor;
	AnsiTerm::TermBackColor m_activebackcolor;
	Vector<StringPtr>m_options;
	int m_curopt;
};

class TerminalWidgetScrollText : public TerminalWidget
{
protected:
	int m_scrolllen;
	List<StringPtr> m_lines;

	void AppendLine( StringPtr line );

public:
	TerminalWidgetScrollText
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
	);
	virtual ~TerminalWidgetScrollText();
	void Append( char *txt );
	void Draw( GC *gc );
	void Clear();

	inline int TextX() { return m_x/*+1*/; }
	inline int TextY() { return m_y/*+1*/; }
	inline int TextWidth() { return m_width/*-2*/; }
	inline int TextHeight() { return m_height/*-2*/; }

#ifdef DEBUG
	virtual void ValidateMem() const;
	virtual void CheckMem() const;
#endif
};

class TerminalWidgetWrapText : public TerminalWidget
{
protected:
	String m_text;

public:
	TerminalWidgetWrapText
	(
		TerminalFrame *frame, 
		int attribs, 
		AnsiTerm::TermForeColor forecolor, 
		AnsiTerm::TermBackColor backcolor, 
		int x, 
		int y, 
		int width, 
		int height
	);
	virtual ~TerminalWidgetWrapText();
	void SetText( const char *txt );
	void Draw( GC *gc );

#ifdef DEBUG
	virtual void ValidateMem() const;
	virtual void CheckMem() const;
#else
	inline void ValidateMem() const {}
	inline void CheckMem() const {}
#endif
};

class TerminalWidgetTextPanel : public TerminalWidget
{
protected:
	TerminalCell *m_text;
	EVENTHANDLER m_evkey;
	EVENTHANDLER m_evctl;
	EVENTHANDLER m_evcmd;
	int m_cx, m_cy;
		
	inline TerminalCell *GetCell(int x, int y) { return &m_text[y * m_width + x]; }

public:
	TerminalWidgetTextPanel
	(
		TerminalFrame *frame, 
		int attribs, 
		AnsiTerm::TermForeColor forecolor, 
		AnsiTerm::TermBackColor backcolor, 
		int x, 
		int y, 
		int width, 
		int height
	);
	virtual ~TerminalWidgetTextPanel();

	// x & y are relative to m_x and m_y.
	void SetText( char ch, int x, int y, int attribs );
	inline void SetText( char ch, int x, int y ) { SetText(ch, x, y, m_attribs); }
	StringPtr GetText();

	void Clear();
	void Draw( GC *gc );
	virtual void CursorPosition(int *x, int *y) const;
	void SetCursor( int x, int y );
	int GetCursorX() { return m_cx; }
	int GetCursorY() { return m_cy; }

	virtual bool HandleKey( GC *gc, const char key );
	virtual bool HandleKeyCtrl( SpecialKey key );
	virtual bool HandleKeyCmd( GC *gc, SpecialKey key );
	virtual bool TrapTabKey();
	virtual bool TrapLF();

	inline void SetKeyHandler(EVENTHANDLER ev) { m_evkey = ev; }
	inline void SetCtlHandler(EVENTHANDLER ev) { m_evctl = ev; }
	inline void SetCmdHandler(EVENTHANDLER ev) { m_evcmd = ev; }

#ifdef DEBUG
	virtual void ValidateMem() const;
	virtual void CheckMem() const;
#endif
};

class TerminalWidgetBox : public TerminalWidget
{
public:
	TerminalWidgetBox
	(
		TerminalFrame *frame, 
		int attribs, 
		AnsiTerm::TermForeColor forecolor, 
		AnsiTerm::TermBackColor backcolor, 
		int x, 
		int y, 
		int width, 
		int height
	);
	TerminalWidgetBox
	(
		TerminalFrame *frame, 
		int attribs, 
		AnsiTerm::TermForeColor forecolor, 
		AnsiTerm::TermBackColor backcolor, 
		TerminalWidget *wrapme
	);
	virtual ~TerminalWidgetBox();
	void Draw( GC *gc );
#ifdef DEBUG
	void ValidateMem() const;
	void CheckMem() const;
#else
	inline void ValidateMem() const {}
	inline void CheckMem() const {}
#endif
};

class TerminalWidgetListBoxItem
{
protected:
	String m_text;
	int m_listid;
	bool m_selected;

public:
	TerminalWidgetListBoxItem( const String& text, int listid );
	TerminalWidgetListBoxItem( const char *text, int listid );
	virtual ~TerminalWidgetListBoxItem();

	inline String& GetText() { return m_text; }
	inline int GetListId() { return m_listid; }
	inline bool IsSelected() { return m_selected; }
	inline void Select() { m_selected = true; }
	inline void UnSelect() { m_selected = false; }

#ifdef DEBUG
	void ValidateMem() const;
	void CheckMem() const;
#else
	inline void ValidateMem() const {}
	inline void CheckMem() const {}
#endif
};

class TerminalWidgetListBox : public TerminalWidget
{
protected:
	int m_colwidth;
	int m_startrow;
	int m_cx, m_cy;
	List<TerminalWidgetListBoxItem *>m_items;
	TerminalWidgetListBoxItem *m_selected;
	EVENTHANDLER m_evcmd;

	TerminalWidgetListBoxItem *FindItem(const char *text);
	inline TerminalWidgetListBoxItem *FindItem(const String& text) { return FindItem(text.GetChars()); }
	TerminalWidgetListBoxItem *FindItem(int id);

	void SelectedRowCol(int *row, int *col);
	void SelectRowCol(int row, int col);
	void Page();

public:
	TerminalWidgetListBox
	(
		int taborder,			// zero means no tabbing
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
	);
	virtual ~TerminalWidgetListBox();

	virtual void Draw( GC *gc );
	virtual bool HandleKey( GC *gc, const char key );
	virtual bool HandleKeyCtrl( SpecialKey key );
	virtual bool HandleKeyCmd( GC *gc, SpecialKey key );
	virtual void CursorPosition(int *x, int *y) const;
	virtual bool TrapLF();
	virtual bool SetFocus( GC *gc );

	void AddItem( const char *text, int id );
	inline void AddItem( const String& text, int id ) { AddItem(text.GetChars(), id); }
	void RemoveAll();
	void RemoveItem( const String& text );
	void RemoveItem( int id );
	void SelectItem( const String& text );
	void SelectItem( int id );
	int Selected();
	String SelectedText();
	int SelectedIndex();

#ifdef DEBUG
	void ValidateMem() const;
	void CheckMem() const;
#endif
};

class TerminalFrame : public IMemoryValidate
{
protected:
	virtual void SendToTerm( spl::IStream& sp, bool forceupdate );
	void FixupTabOrder();

	int m_width;
	int m_height;
	Vector<TerminalWidget *>m_widgets;
	TerminalWidget *m_taborder[255];
	TerminalWidget *m_hasFocus;
	TerminalFrameManager *m_fman;
	EVENTHANDLER m_fnHandlers[24];
	AnsiTerm::TermBackColor m_background;
	StringBuffer m_sendbuf;
	Mutex m_mtx;

public:
	TerminalFrame(TerminalFrameManager *cproxy, int w, int h, AnsiTerm::TermBackColor background);
	virtual ~TerminalFrame();

	inline int Width() const { return m_width; }
	inline int Height() const { return m_height; }
	inline TerminalFrameManager *GetFrameManager() { return m_fman; }
	bool IsVisible();

	inline void UpdateTerm( spl::IStream& sp ) { SendToTerm( sp, false ); }
	inline void RedrawTerm( spl::IStream& sp ) { SendToTerm( sp, true ); }
	
	void Redraw();
	void Resize(int minx, int maxx, int miny, int maxy);

	void AddWidget( TerminalWidget *control );
	void RemoveWidget( TerminalWidget *control );
	void RemoveAllWidgets();
	void WidgetToFront( TerminalWidget *control );
	void SetFocus( TerminalWidget *control, bool redraw );
	inline TerminalWidget *ControlWithFocus() { return m_hasFocus; }

	virtual void HandleKeyTab();
	virtual void HandleKeyControl( SpecialKey ch );
	virtual void HandleKey( const char ch );
	virtual void HandleKeyCmd( SpecialKey ch );
	virtual void HandleKeyFN( SpecialKey ch );
	virtual void HandleKeyLF();

	inline void Invalidate();
	inline void AddFnHandler( SpecialKey fn, EVENTHANDLER handler ) { m_fnHandlers[fn-SKEY_F1] = handler; }
	inline AnsiTerm::TermBackColor GetBackgroundColor() { return m_background; }

#ifdef DEBUG
	virtual void ValidateMem() const;
	virtual void CheckMem() const;
#endif
};

inline void ValidateType( TerminalFrame *tf )
{
#ifdef DEBUG
	if ( NULL == tf )
	{
		// List defaults to NULL
		return;
	}
	ASSERT_PTR( tf );
	tf->ValidateMem();
	DEBUG_NOTE_MEM( tf );
	tf->CheckMem();
#endif
}

class TerminalFrameManager : public ITermListener
{
protected:
	ITerminal *m_term;
	byte m_buf8Csi[8];
	StringBuffer m_strArgs;
	TerminalFrame *m_curframe;
	Hashtable<String, TerminalFrame *>m_frames;
	Vector<TerminalFrame *> m_vframes;
	List<TerminalFrame *>m_framestack;
	void *m_data;
	char m_cr;
	GC m_gc;

public:
	TerminalFrameManager( TcpSocketPtr sock, SocketSet& ss );
	TerminalFrameManager( spl::IStreamPtr conn );
	TerminalFrameManager( ITerminal *term );
	virtual ~TerminalFrameManager();

	inline ITerminal *GetTerminal() { return m_term; }
	inline TermCap *GetTermCaps() { ASSERT_PTR(m_term); return m_term->GetTermCaps(); }
	inline void SetCR(char ch) { m_cr = ch; }

	virtual void OnTermOpen();
	virtual void OnTermClose();
	virtual void OnTermError(const String msg);

	virtual void OnTermKey(SpecialKey ch);
	virtual void OnTermKey(char ch);
	virtual void OnTermKeys(const char *buf, const int len);
	virtual void OnTermSize( int rows, int cols );
	virtual void OnTermType(const String termname);
	virtual void OnTermCursorPos(int row, int col);
	virtual void OnTermChanged();
	virtual void OnTermWindowSize( int minx, int maxx, int miny, int maxy );

	void ClearFrames();
	void PushFrame( const String frametoshow );
	void PopFrame();
	void SetFrame( const String frametoshow );
	void AddFrame( TerminalFrame *frame, const String name );
	inline TerminalFrame *CurrentFrame() { ASSERT_PTR(m_curframe); return m_curframe; }
	inline bool CurrentFrameIs(const String framename) { return m_curframe == m_frames.Get(String(framename)); }
	TerminalFrame *GetFrame( const String framename );
	inline GC *GetGC() { return &m_gc; }

	inline void *GetData() { return m_data; }
	inline void SetData(void *data) { m_data = data; }

#ifdef DEBUG
	virtual void ValidateMem() const;
	virtual void CheckMem() const;
#else
	inline void ValidateMem() const {}
	inline void CheckMem() const {}
#endif

protected:
	void Send( const byte *b, const int len );
};

}

#endif
