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
#ifndef _RGB_H
#define _RGB_H

#include <stdio.h>
#include <string.h>
#include <spl/types.h>
#include <spl/Debug.h>
#include <spl/term/AnsiTerm.h>
#include <spl/Memory.h>

namespace spl
{

class TerminalForeColors
{
public:
	enum 
	{
		// Foreground colors
		BLACK = AnsiTerm::FORE_BLACK,
		RED = AnsiTerm::FORE_RED,
		GREEN = AnsiTerm::FORE_GREEN,
		YELLOW = AnsiTerm::FORE_YELLOW,
		BLUE = AnsiTerm::FORE_BLUE,
		MAGENTA = AnsiTerm::FORE_MAG,
		CYAN = AnsiTerm::FORE_CYAN,
		GREY = AnsiTerm::FORE_GREY
	};
};

class TerminalBackColors
{ 
public:
	enum 
	{
		// background colors
		BLACK = AnsiTerm::BACK_BLACK,
		RED = AnsiTerm::BACK_RED,
		GREEN = AnsiTerm::BACK_GREEN,
		YELLOW = AnsiTerm::BACK_YELLOW,
		BLUE = AnsiTerm::BACK_BLUE,
		MAGENTA = AnsiTerm::BACK_MAG,
		CYAN = AnsiTerm::BACK_CYAN,
		GREY = AnsiTerm::BACK_GREY
	};
};

class TerminalColor : public IMemoryValidate
{
protected:
	byte m_a;
	byte m_r;
	byte m_g;
	byte m_b;

	static uint32 m_black;
	static uint32 m_red;
	static uint32 m_green;
	static uint32 m_yellow;
	static uint32 m_blue;
	static uint32 m_magenta;
	static uint32 m_cyan;
	static uint32 m_grey;

public:
	TerminalColor();
	TerminalColor(byte alpha, byte red, byte green, byte blue);	
	TerminalColor(byte red, byte green, byte blue);
	TerminalColor(int colorref);
	virtual ~TerminalColor();

	inline byte GetAlpha() const { return m_a; }
	inline byte GetRed() const { return m_r; }
	inline byte GetGreen() const { return m_g; }
	inline byte GetBlue() const { return m_b; }

	inline uint32 AsInt() const
	{
		return ((uint32)m_a) << 24 | ((int)m_r) << 16 | ((int)m_g) << 8 | (int)m_b;
	}

	inline int AsRGB24() const
	{ 
		return (m_r << 16) | (m_g << 8) | (m_b); 
	}

	inline uint32 AsRGBA32() const
	{ 
		return (m_a << 24) | (m_r << 16) | (m_g << 8) | (m_b); 
	}

	inline void AsHexString(char *buf, const int buflen) const
	{
#if defined(_MSC_VER) && _MSC_VER >= 1400
		sprintf_s(buf, buflen, "%X", AsInt());
#else
		sprintf(buf, "%X", AsInt());
#endif
	}

	inline TerminalColor *Brighter() const
	{
		int rr = m_r + (int)(m_r * .1F);
		int gg = m_g + (int)(m_g * .1F);
		int bb = m_b + (int)(m_b * .1F);
		return new TerminalColor(m_a, (rr>0xFF)?0xFF:rr, (gg>0xFF)?0xFF:gg, (bb>0xFF)?0xFF:bb);
	}

	inline bool operator == (TerminalColor &color) const
	{
		return m_b == color.m_b && m_g == color.m_g && m_r == color.m_r && m_a == color.m_a;
	}

	inline bool operator != (TerminalColor &color) const
	{
		return m_b != color.m_b || m_g != color.m_g || m_r != color.m_r || m_a != color.m_a;
	}

	AnsiTerm::TermForeColor AsTerminalForeColor() const;
	AnsiTerm::TermBackColor AsTerminalBackColor() const;

	virtual void ValidateMem() const
	{
	}
	
	virtual void CheckMem() const
	{
	}

	static TerminalColor Black;
	static TerminalColor Red;
	static TerminalColor Green;
	static TerminalColor Yellow;
	static TerminalColor Blue;
	static TerminalColor Magenta;
	static TerminalColor Cyan;
	static TerminalColor Grey;

	static TerminalColor Parse(char *color);
};

union signexthelper
{
	uint16 us;
	int32 i;
	uint32 ui;
	int64 l;
};

inline int64 UIntToLongNoSignExt(uint32 i)
{
	ASSERT(sizeof(int64) == 8);
	signexthelper u;
	u.l = 0L;
	u.ui = i;
	return u.l;
}

}

#endif

