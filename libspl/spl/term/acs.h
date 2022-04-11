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
#ifndef _acs_h
#define _acs_h

namespace spl
{

enum AltCharSet
{
	ULCORNER =	1, /* upper left corner */
	LLCORNER =	2, /* lower left corner */
	URCORNER =	3, /* upper right corner */
	LRCORNER =	4, /* lower right corner */
	LTEE =		5, /* tee pointing right */
	RTEE =		6, /* tee pointing left */
	BTEE =		7, /* tee pointing up */
	TTEE =		8, /* tee pointing down */
	HLINE =		9, /* horizontal line */
	VLINE =		10, /* vertical line */
	PLUS =		11, /* large plus or crossover */
	S1 =		12, /* scan line 1 */
	S9 =		13, /* scan line 9 */
	DIAMOND =	14, /* diamond */
	CKBOARD =	15, /* checker board (stipple) */
	DEGREE =	16, /* degree symbol */
	PLMINUS =	17, /* plus/minus */
	BULLET =	18, /* bullet */
	LARROW =	19, /* arrow pointing left */
	RARROW =	20, /* arrow pointing right */
	DARROW =	21, /* arrow pointing down */
	UARROW =	22, /* arrow pointing up */
	BOARD =		23, /* board of squares */
	LANTERN =	24, /* lantern symbol */
	BLOCK =		25, /* solid square block */
	S3 =		26, /* scan line 3 */
	S7 =		27, /* scan line 7 */
	LEQUAL =	28, /* less/equal */
	GEQUAL =	29, /* greater/equal */
	PI =		30, /* Pi */
	NEQUAL =	31, /* not equal */
	STERLING =	32 /* UK pound sign */
};

/* VT100 symbols begin here */
#define ACS_ULCORNER	'l' /* upper left corner */
#define ACS_LLCORNER	'm' /* lower left corner */
#define ACS_URCORNER	'k' /* upper right corner */
#define ACS_LRCORNER	'j' /* lower right corner */
#define ACS_LTEE		't' /* tee pointing right */
#define ACS_RTEE		'u' /* tee pointing left */
#define ACS_BTEE		'v' /* tee pointing up */
#define ACS_TTEE		'w' /* tee pointing down */
#define ACS_HLINE		'q' /* horizontal line */
#define ACS_VLINE		'x' /* vertical line */
#define ACS_PLUS		'n' /* large plus or crossover */
#define ACS_S1			'o' /* scan line 1 */
#define ACS_S9			's' /* scan line 9 */
#define ACS_DIAMOND		'`' /* diamond */
#define ACS_CKBOARD		'a' /* checker board (stipple) */
#define ACS_DEGREE		'f' /* degree symbol */
#define ACS_PLMINUS		'g' /* plus/minus */
#define ACS_BULLET		'~' /* bullet */
/* Teletype 5410v1 symbols begin here */
#define ACS_LARROW		',' /* arrow pointing left */
#define ACS_RARROW		'+' /* arrow pointing right */
#define ACS_DARROW		'.' /* arrow pointing down */
#define ACS_UARROW		'-' /* arrow pointing up */
#define ACS_BOARD		'h' /* board of squares */
#define ACS_LANTERN		'i' /* lantern symbol */
#define ACS_BLOCK		'0' /* solid square block */
/*
 * These aren't documented, but a lot of System Vs have them anyway
 * (you can spot pprryyzz{{||}} in a lot of AT&T terminfo strings).
 * The ACS_names may not match AT&T's, our source didn't know them.
 */
#define ACS_S3			'p' /* scan line 3 */
#define ACS_S7			'r' /* scan line 7 */
#define ACS_LEQUAL		'y' /* less/equal */
#define ACS_GEQUAL		'z' /* greater/equal */
#define ACS_PI			'{' /* Pi */
#define ACS_NEQUAL		'|' /* not equal */
#define ACS_STERLING	'}' /* UK pound sign */

/* ANSI Terminals */
#define ANSI_ULCORNER	(char)108 /* upper left corner */
#define ANSI_LLCORNER	(char)109 /* lower left corner */
#define ANSI_URCORNER	(char)107 /* upper right corner */
#define ANSI_LRCORNER	(char)106 /* lower right corner */
#define ANSI_LTEE		(char)116 /* tee pointing right */
#define ANSI_RTEE		(char)117 /* tee pointing left */
#define ANSI_BTEE		(char)118 /* tee pointing up */
#define ANSI_TTEE		(char)119 /* tee pointing down */
#define ANSI_HLINE		(char)113 /* horizontal line */
#define ANSI_VLINE		(char)120 /* vertical line */
#define ANSI_PLUS		(char)110 /* large plus or crossover */
#define ANSI_S1			(char)111 /* scan line 1 */
#define ANSI_S9			(char)114 /* scan line 9 */
#define ANSI_DIAMOND	(char)4	  /* diamond */
#define ANSI_CKBOARD	(char)97  /* checker board (stipple) */
#define ANSI_DEGREE		(char)102 /* degree symbol */
#define ANSI_PLMINUS	(char)103 /* plus/minus */
#define ANSI_BULLET		(char)126 /* bullet */
/* Teletype 5410v1 symbols begin here */
#define ANSI_LARROW		(char)17 /* arrow pointing left */
#define ANSI_RARROW		(char)16 /* arrow pointing right */
#define ANSI_DARROW		(char)31 /* arrow pointing down */
#define ANSI_UARROW		(char)30 /* arrow pointing up */
#define ANSI_BOARD		(char)35 /* board of squares */
#define ANSI_LANTERN	(char)21 /* lantern symbol */
#define ANSI_BLOCK		(char)22 /* solid square block */
/*
 * These aren't documented, but a lot of System Vs have them anyway
 * (you can spot pprryyzz{{||}} in a lot of AT&T terminfo strings).
 * The ACS_names may not match AT&T's, our source didn't know them.
 */
#define ANSI_S3			(char)113 /* scan line 3 */
#define ANSI_S7			(char)114 /* scan line 7 */
#define ANSI_LEQUAL		(char)121 /* less/equal */
#define ANSI_GEQUAL		(char)122 /* greater/equal */
#define ANSI_PI			(char)123 /* Pi */
#define ANSI_NEQUAL		(char)124 /* not equal */
#define ANSI_STERLING	(char)125 /* UK pound sign */

/* DOS */
#define DOS_ULCORNER	(char)218 /* upper left corner */
#define DOS_LLCORNER	(char)192 /* lower left corner */
#define DOS_URCORNER	(char)191 /* upper right corner */
#define DOS_LRCORNER	(char)217 /* lower right corner */
#define DOS_LTEE		(char)195 /* tee pointing right */
#define DOS_RTEE		(char)180 /* tee pointing left */
#define DOS_BTEE		(char)193 /* tee pointing up */
#define DOS_TTEE		(char)194 /* tee pointing down */
#define DOS_HLINE		(char)196 /* horizontal line */
#define DOS_VLINE		(char)179 /* vertical line */
#define DOS_PLUS		(char)197 /* large plus or crossover */
#define DOS_S1			(char)45 /* scan line 1 */
#define DOS_S9			(char)95 /* scan line 9 */
#define DOS_DIAMOND		(char)4 /* diamond */
#define DOS_CKBOARD		(char)177 /* checker board (stipple) */
#define DOS_DEGREE		(char)248 /* degree symbol */
#define DOS_PLMINUS		(char)241 /* plus/minus */
#define DOS_BULLET		(char)249 /* bullet */
/* Teletype 5410v1 symbols begin here */
#define DOS_LARROW		(char)'<' /* arrow pointing left */
#define DOS_RARROW		(char)'>' /* 16 arrow pointing right */
#define DOS_DARROW		(char)25 /* arrow pointing down */
#define DOS_UARROW		(char)30 /* arrow pointing up */
#define DOS_BOARD		(char)176 /* board of squares */
#define DOS_LANTERN		(char)21 /* lantern symbol */
#define DOS_BLOCK		(char)254 /* solid square block */
/*
 * These aren't documented, but a lot of System Vs have them anyway
 * (you can spot pprryyzz{{||}} in a lot of AT&T terminfo strings).
 * The ACS_names may not match AT&T's, our source didn't know them.
 */
#define DOS_S3			(char)45 /* scan line 3 */
#define DOS_S7			(char)45 /* scan line 7 */
#define DOS_LEQUAL		(char)243 /* less/equal */
#define DOS_GEQUAL		(char)242 /* greater/equal */
#define DOS_PI			(char)227 /* Pi */
#define DOS_NEQUAL		(char)273 /* not equal */
#define DOS_STERLING	(char)156 /* UK pound sign */

}

#endif
