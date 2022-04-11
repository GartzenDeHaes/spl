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
#ifndef _Dates_h
#define _Dates_h

#include <stdlib.h>

#include <spl/types.h>
#include <spl/Debug.h>

#ifdef HAVE_SYS_TIME_H
#include <sys/time.h>
#endif
#ifdef HAVE_TIME_H
#include <time.h>
#endif

#include <spl/Date.h>
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

#define DATETIME_MAJIC 0x41FA		//< Majic number for ASSERT's in Compare and Convert

/** @ A date and time.
 *
 */
class DateTime : public IMemoryValidate, public IComparable
{
protected:
    struct tm m_dtm;
	time_t m_timet;

public:
	DateTime();
	DateTime(const DateTime& dtm);
	DateTime(const Date& dt);
	DateTime( int year, int month, int day );
	DateTime( int year, int month, int day, int hour, int minute, int second );
	DateTime( const time_t t );
	virtual ~DateTime();

	///@brief The currrent date and time (not a high resolution timer).
	inline static DateTime Now()
	{
		return DateTime();
	}

	inline int Year() const
	{
		return m_dtm.tm_year + 1900;
	}
	
	inline int Month() const
	{
		return m_dtm.tm_mon + 1;
	}
	
	inline int Day() const
	{
		return m_dtm.tm_mday;
	}
	
	inline int Hour() const
	{
		return m_dtm.tm_hour;
	}

	inline int Minutes() const
	{
		return m_dtm.tm_min;
	}

	inline int Seconds() const
	{
		return m_dtm.tm_sec;
	}

	inline bool IsDayLightSavingsTime() const
	{
		return m_dtm.tm_isdst != 0;
	}

	inline int JulianDate() const
	{
		return m_dtm.tm_yday + 1;
	}

	///@ brief Day of week (1 is sunday).
	inline enum Date::DayOfWeek DayOfWeek() const
	{
		return (enum Date::DayOfWeek)m_dtm.tm_wday;
	}

	inline Date DatePart() const
	{
		return Date( Year(), Month(), Day() );
	}

	inline bool IsLeapYear() const
	{
		int year = Year();
		if ( (year % 4) == 0 )
		{
			if ( (year % 100) == 0 )
			{
				return (int)((year % 400) == 0);
			}
			return true;
		}
		return false;
	}

	inline time_t ToSysTime() const
	{
		// cheat on const
		return m_timet;
	}

	inline void ToString( char *buf, int buflen ) const
	{
#if defined(_MSC_VER) && _MSC_VER >= 1400
		asctime_s(buf, buflen, &m_dtm);
#else
		char *cp = asctime( &m_dtm );
		spl::StrCpyLen( buf, cp, buflen );
#endif
	}

	inline StringPtr ToString() const
	{
		char buf[128];
		ToString(buf, 128);
		return StringPtr(new String(buf));
	}
	
	/** @brief YYYY-MM-DD HH:MM:SS */
	StringPtr ToStringISO8601() const;
	
	/** @brief HH:MM:SS */
	StringPtr ToTimeString() const;

	DateTime AddSeconds(int64 seconds) const;
	DateTime AddMinutes(int minutes) const;
	DateTime AddHours(int hours) const;
	DateTime AddDays(int days) const;
	DateTime AddMonths(int months) const;
	DateTime AddYears(int years) const;

	inline int64 DiffInSeconds( const DateTime dtm ) const
	{
		return (int64)difftime(m_timet, dtm.m_timet);
	}

	inline int64 DiffInMinutes( const DateTime dtm ) const
	{
		return DiffInSeconds(dtm) / (60);
	}

	inline int64 DiffInHours( const DateTime dtm ) const
	{
		return DiffInSeconds(dtm) / (60 * 60);
	}

	inline int64 DiffInDays( const DateTime dtm ) const
	{
		return DiffInSeconds(dtm) / (60 * 60 * 24);
	}

	inline bool Equals( const DateTime& dt ) const
	{
		return 0 == memcmp(&m_dtm, &dt.m_dtm, sizeof(struct tm));
	}

	inline bool Equals( const DateTime* dt ) const
	{
		return 0 == memcmp(&m_dtm, &dt->m_dtm, sizeof(struct tm));
	}

	inline DateTime& operator =(const DateTime& dtm )
	{
		m_dtm = dtm.m_dtm;
		m_timet = dtm.m_timet;
		return *this;
	}

	inline bool operator <(const DateTime& dt) const
	{
		return ToSysTime() < dt.ToSysTime();
	}

	inline bool operator <=(const DateTime& dt) const
	{
		return ToSysTime() <= dt.ToSysTime();
	}

	inline bool operator >(const DateTime& dt) const
	{
		return ToSysTime() > dt.ToSysTime();
	}

	inline bool operator >=(const DateTime& dt) const
	{
		return ToSysTime() >= dt.ToSysTime();
	}

	inline bool operator ==(const DateTime& dt) const
	{
		return ToSysTime() == dt.ToSysTime();
	}

	inline bool operator !=(const DateTime& dt) const
	{
		return ToSysTime() != dt.ToSysTime();
	}

	virtual uint32 HashCode() const;
	virtual bool Equals( const IComparable *a ) const;
	virtual int Compare( const IComparable *a ) const;
	virtual bool Equals( const IComparable& a ) const;
	virtual int Compare( const IComparable& a ) const;
	virtual int32 MajicNumber() const;

	static DateTime Parse(const String& str);
	static bool IsDateTime(const String& str);

#ifdef DEBUG
	virtual void ValidateMem() const;
	virtual void CheckMem() const;
#endif
};

/** @} */
}
#endif
