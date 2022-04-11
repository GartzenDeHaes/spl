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
#include <math.h>
#include <spl/term/Color.h>

using namespace spl;

TerminalColor TerminalColor::Black(0x00, 0x00, 0x00);
TerminalColor TerminalColor::Red(0xFF, 0x00, 0x00);
TerminalColor TerminalColor::Green(0x00, 0xFF, 0x00);
TerminalColor TerminalColor::Yellow(0xFF, 0xFF, 0x00);
TerminalColor TerminalColor::Blue(0x00, 0x00, 0xFF);
TerminalColor TerminalColor::Magenta(0xFF, 0x00, 0xFF);
TerminalColor TerminalColor::Cyan(0x00, 0xFF, 0xFF);
TerminalColor TerminalColor::Grey(0xF0, 0xF0, 0xF0);

uint32 TerminalColor::m_black = (0xFF <<24) | (0x00 << 16) | (0x00 << 8) | (0x00);
uint32 TerminalColor::m_red = (0xFF <<24) | (0xFF << 16) | (0x00 << 8) | (0x00);
uint32 TerminalColor::m_green = (0xFF <<24) | (0x00 << 16) | (0xFF << 8) | (0x00);
uint32 TerminalColor::m_yellow = (0xFF <<24) | (0xFF << 16) | (0xFF << 8) | (0x00);
uint32 TerminalColor::m_blue = (0xFF <<24) | (0x00 << 16) | (0x00 << 8) | (0xFF);
uint32 TerminalColor::m_magenta = (0xFF <<24) | (0xFF << 16) | (0x00 << 8) | (0xFF);
uint32 TerminalColor::m_cyan = (0xFF <<24) | (0x00 << 16) | (0xFF << 8) | (0xFF);
uint32 TerminalColor::m_grey = (0xFF <<24) | (0xF0 << 16) | (0xF0 << 8) | (0xF0);

TerminalColor::TerminalColor() 
{
	m_a = 255;
	m_r = 0;
	m_g = 0;
	m_b = 0;
}

TerminalColor::TerminalColor(byte alpha, byte red, byte green, byte blue) 
{
	m_a = alpha;
	m_r = red;
	m_g = green;
	m_b = blue;
}

TerminalColor::TerminalColor(byte red, byte green, byte blue) 
{
	m_a = 255;
	m_r = red;
	m_g = green;
	m_b = blue;
}

TerminalColor::TerminalColor(int colorref)
{
	m_a = 255; //(byte)(colorref >> 24;
	m_r = (byte)((colorref & 0xFF0000) >> 16);
	m_g = (byte)((colorref & 0xFF00) >> 8);
	m_b = (byte)(colorref & 0xFF);
}

TerminalColor::~TerminalColor() 
{
}

TerminalColor TerminalColor::Parse(char *color)
{
	if (strcmp("GREEN", color) == 0)
		return TerminalColor(0, 240, 0);
		
	else if (strcmp("BLACK", color) == 0)
		return TerminalColor(0,0,0);
		
	else if (strcmp("BLUE", color) == 0)
		return TerminalColor(0, 0, 240);
		
	else if (strcmp("LTGRAY", color) == 0)
		return TerminalColor(200, 200, 200);
			
	else if (strcmp("GRAY", color) == 0)
		return TerminalColor(140, 140, 140);
		
	else if (strcmp("WHITE", color) == 0)
		return TerminalColor(255, 255, 255);
			
	else if (strcmp("YELLOW", color) == 0)
		return TerminalColor(0, 255, 255);
			
	else if (strcmp("RED", color) == 0)
		return TerminalColor(255, 0, 0);
			
	else if (strcmp("PINK", color) == 0)
		return TerminalColor(230, 100, 100);
			
	else if (strcmp("MAGENTA", color) == 0)
		return TerminalColor(230, 0, 230);
			
	else if (strcmp("ORANGE", color) == 0)
		return TerminalColor(255, 200, 0);
			
	else
		return TerminalColor(80, 80, 80);
}

AnsiTerm::TermForeColor TerminalColor::AsTerminalForeColor() const
{
	AnsiTerm::TermForeColor rettermcolor;
	int64 retdiff = 0xFFFFFFFFL;
	int diff = 0;

	int64 argb = UIntToLongNoSignExt(AsRGBA32());

	if ( (diff = abs((int)(argb - m_black))) < retdiff )
	{
		retdiff = diff;
		rettermcolor = (AnsiTerm::TermForeColor)TerminalForeColors::BLACK;
	}
	if ( (diff = abs((int)(argb - m_red))) < retdiff )
	{
		retdiff = diff;
		rettermcolor = (AnsiTerm::TermForeColor)TerminalForeColors::RED;
	}
	if ( (diff = abs((int)(argb - m_green))) < retdiff )
	{
		retdiff = diff;
		rettermcolor = (AnsiTerm::TermForeColor)TerminalForeColors::GREEN;
	}
	if ( (diff = abs((int)(argb - m_yellow))) < retdiff )
	{
		retdiff = diff;
		rettermcolor = (AnsiTerm::TermForeColor)TerminalForeColors::YELLOW;
	}
	if ( (diff = abs((int)(argb - m_blue))) < retdiff )
	{
		retdiff = diff;
		rettermcolor = (AnsiTerm::TermForeColor)TerminalForeColors::BLUE;
	}
	if ( (diff = abs((int)(argb - m_magenta))) < retdiff )
	{
		retdiff = diff;
		rettermcolor = (AnsiTerm::TermForeColor)TerminalForeColors::MAGENTA;
	}
	if ( (diff = abs((int)(argb - m_cyan))) < retdiff )
	{
		retdiff = diff;
		rettermcolor = (AnsiTerm::TermForeColor)TerminalForeColors::CYAN;
	}
	if ( (diff = abs((int)(argb - m_grey))) < retdiff )
	{
		retdiff = diff;
		rettermcolor = (AnsiTerm::TermForeColor)TerminalForeColors::GREY;
	}
	return rettermcolor;
}

AnsiTerm::TermBackColor TerminalColor::AsTerminalBackColor() const
{
	AnsiTerm::TermBackColor rettermcolor;
	int64 retdiff = 0xFFFFFFFFL;
	int diff = 0;

	int64 argb = UIntToLongNoSignExt(AsRGBA32());

	if ( (diff = abs((int)(argb - m_black))) < retdiff )
	{
		retdiff = diff;
		rettermcolor = (AnsiTerm::TermBackColor)TerminalBackColors::BLACK;
	}
	if ( (diff = abs((int)(argb - m_red))) < retdiff )
	{
		retdiff = diff;
		rettermcolor = (AnsiTerm::TermBackColor)TerminalBackColors::RED;
	}
	if ( (diff = abs((int)(argb - m_green))) < retdiff )
	{
		retdiff = diff;
		rettermcolor = (AnsiTerm::TermBackColor)TerminalBackColors::GREEN;
	}
	if ( (diff = abs((int)(argb - m_yellow))) < retdiff )
	{
		retdiff = diff;
		rettermcolor = (AnsiTerm::TermBackColor)TerminalBackColors::YELLOW;
	}
	if ( (diff = abs((int)(argb - m_blue))) < retdiff )
	{
		retdiff = diff;
		rettermcolor = (AnsiTerm::TermBackColor)TerminalBackColors::BLUE;
	}
	if ( (diff = abs((int)(argb - m_magenta))) < retdiff )
	{
		retdiff = diff;
		rettermcolor = (AnsiTerm::TermBackColor)TerminalBackColors::MAGENTA;
	}
	if ( (diff = abs((int)(argb - m_cyan))) < retdiff )
	{
		retdiff = diff;
		rettermcolor = (AnsiTerm::TermBackColor)TerminalBackColors::CYAN;
	}
	if ( (diff = abs((int)(argb - m_grey))) < retdiff )
	{
		retdiff = diff;
		rettermcolor = (AnsiTerm::TermBackColor)TerminalBackColors::GREY;
	}
	return rettermcolor;
}
