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
#ifndef _Date_h
#define _Date_h

#include <spl/types.h>

#include <stdio.h>
#include <stdlib.h>

#ifdef HAVE_SYS_TIME_H
#include <sys/time.h>
#endif
#ifdef HAVE_TIME_H
#include <time.h>
#endif

#include <spl/Debug.h>
#include <spl/Exception.h>
#include <spl/Memory.h>
#include <spl/String.h>

namespace spl
{
/**
 * @defgroup types Types
 * @ingroup core
 * @{
 */

#define DATE_MAJIC 0xA011		//< Majic number for ASSERT's in Compare and Convert

/** @brief A date with no time.
 *	Most operations involve a single long value, so performance is generally good.
 */
class Date : public IMemoryValidate, public IComparable
{
private:
	static uint64 YmdToJd( const int iYear, const int iMonth, const int iDay );
	static void JdToYmd( const uint64 lJD, int *piYear, int *piMonth, int *piDay );

	void Init(time_t t);

protected:
    uint64 m_lJulianDay;
	int m_y, m_m, m_d;

public:
	enum DayOfWeek
	{
		SUNDAY = 0,
		MONDAY = 1,
		TUESDAY = 2,
		WEDNESDAY = 3,
		THURSDAY = 4,
		FRIDAY = 5,
		SATURDAY = 6
	};

	Date();
	Date(const Date& dt);
	Date(int year, int mo, int day);
	Date( time_t tt );
	virtual ~Date();

	inline bool IsLeapYear()
	{
		uint64 jd1, jd2;
		JdToYmd( m_lJulianDay, &m_y, &m_m, &m_d );
		jd1 = YmdToJd( m_y, 2, 28 );
		jd2 = YmdToJd( m_y, 3, 1 );
		return ( ( jd2 - jd1 ) > 1 );
	}

	inline int Year() const
	{
		ASSERT ( m_y > 0 );
		return m_y;
	}

	inline int Month() const
	{
		ASSERT ( m_m > 0 );
		return m_m;
	}

	inline int Day() const
	{
		ASSERT ( m_d > 0 );
		return m_d;
	}

	inline enum DayOfWeek DayOfWeek() const
	{
		return (enum DayOfWeek)( ( ( (int) ( m_lJulianDay % 7L ) ) + 1 ) % 7 );
	}

	inline int DayOfYear()
	{
		uint64 soy;

		ASSERT ( m_y > 0 );
		soy = YmdToJd( m_y, 1, 1 );
		return (int) ( m_lJulianDay - soy + 1 );
	}

	inline int DiffInDays( const Date *dt ) const
	{
		return (int)(m_lJulianDay - dt->m_lJulianDay);
	}

	inline int DiffInDays( const Date& dt ) const
	{
		return (int)(m_lJulianDay - dt.m_lJulianDay);
	}

	inline void AddDays( const int days )
	{
		m_lJulianDay += days;
	}

	void AddMonths( int months );

	inline void AddYears( int years )
	{
		m_y += years;
		m_lJulianDay = YmdToJd( m_y, m_m, m_d );
	}

	bool IsHoliday() const;

	inline void ToString( char *buf, const int buflen ) const
	{
		ASSERT ( m_y > 0 );
#if defined(_MSC_VER) && _MSC_VER >= 1400
		sprintf_s(buf, buflen, "%d/%d/%d", m_m, m_d, m_y);
#else
		sprintf(buf, "%d/%d/%d", m_m, m_d, m_y);
#endif
	}

	inline StringPtr ToString() const
	{
		char buf[24];
		ToString(buf, 24);
		return StringPtr(new String((char *)buf));
	}

	inline bool Equals( const Date *dt ) const
	{
		return m_lJulianDay == dt->m_lJulianDay;
	}

	inline bool Equals( const Date& dt ) const
	{
		return m_lJulianDay == dt.m_lJulianDay;
	}

	inline Date& operator =(const Date& dt )
	{
		m_lJulianDay = dt.m_lJulianDay;
		return *this;
	}

	inline bool operator ==(const Date& dt ) const
	{
		return m_lJulianDay == dt.m_lJulianDay;
	}

	inline bool operator !=(const Date& dt ) const
	{
		return m_lJulianDay != dt.m_lJulianDay;
	}

	inline bool operator >(const Date& dt) const
	{
		return m_lJulianDay > dt.m_lJulianDay;
	}

	inline bool operator <(const Date& dt) const
	{
		return m_lJulianDay < dt.m_lJulianDay;
	}

	inline bool operator >=(const Date& dt) const
	{
		return m_lJulianDay >= dt.m_lJulianDay;
	}

	inline bool operator <=(const Date& dt) const
	{
		return m_lJulianDay <= dt.m_lJulianDay;
	}

	inline time_t ToSysTime(  )
	{
		struct tm tmRep;
		time_t t;

		if ( m_y < 0 )
		{
			JdToYmd( m_lJulianDay, &m_y, &m_m, &m_d );
		}
		if ( m_y < 1970 )
		{
			m_y = 70;
		}
		tmRep.tm_year = m_y - 1900 ;
		tmRep.tm_mon = m_m - 1;
		tmRep.tm_mday = m_d;
		tmRep.tm_hour = 0;
		tmRep.tm_min = 0;
		tmRep.tm_sec = 0;
		tmRep.tm_isdst = 0;

		t = mktime( &tmRep );
		return t;
	}

	inline int ToRevInt( ) const
	{
		return Year() * 10000 + Month() * 100 + Day();
	}

	virtual uint32 HashCode() const;
	virtual bool Equals( const IComparable *a ) const;
	virtual int Compare( const IComparable *a ) const;
	virtual bool Equals( const IComparable& a ) const;
	virtual int Compare( const IComparable& a ) const;
	virtual int32 MajicNumber() const;

	static Date Parse(const String& str);
	static bool IsDate(const String& str);

	inline static Date Now() { return Date(); }

	inline static bool IsLeapYear(int year)
	{
		return ((year % 4) == 0 && (year % 100) != 0) || (year % 400) == 0;
	}

#ifdef DEBUG
	virtual void ValidateMem() const;
	virtual void CheckMem() const;
#endif
};

REGISTER_TYPEOF( 558, Date );

/** @} */
}
#endif
