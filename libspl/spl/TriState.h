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
#ifndef _tristate_h
#define _tristate_h

namespace spl
{
/** 
 * @defgroup types Types
 * @ingroup core
 * @{
 */

class TriState
{
public:
	enum {
		TS_HI = 1,
		TS_LOW = -1,
		TS_UNSET = 0
	};

	inline byte GetState() { return m_state; }
	inline void SetState(byte val) { m_state = val; }
	inline bool IsSet() { return m_state != TS_UNSET; }
	inline bool IsHigh() { return m_state == TS_HI; }
	inline bool IsLow() { return m_state == TS_LOW; }
	inline void SetHi() { m_state = TS_HI; }
	inline void SetLow() { m_state = TS_LOW; }
	inline void Clear() { m_state = TS_UNSET; }

protected:
	char m_state;
};

/** @} */
}
#endif
