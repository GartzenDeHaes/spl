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
#include <ctype.h>
#include <math.h>
#include <spl/Debug.h>
#include <spl/Date.h>
#include <spl/DateTime.h>
#include <spl/Int32.h>
#include <spl/math/Math.h>
#include <spl/text/StringBuffer.h>

using namespace spl;

static int _daysPerMonth[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

inline bool _IsLeapYear(int year)
{
	if ((year % 4) != 0)
	{
		return false;
	}
	else if  ((year % 400) == 0)
	{
		return true;
	}
	else if ((year % 100) == 0)
	{
		return false;
	}
	else
	{
		return true;
	}
}

uint64 Date::YmdToJd( const int iYear, const int iMonth, const int iDay )
{
    uint64 jul_day;

#ifndef JULDATE_USE_ALTERNATE_METHOD

    int a,b;
    int year = iYear, month = iMonth, day = iDay;
    float year_corr;

    if ( year < 0 )
        year++;
    year_corr = (float)( year > 0 ? 0.0 : 0.75 );
    if ( month <= 2 )
    {
        year--;
        month += 12;
    }
    b = 0;
    if ( year * 10000.0 + month * 100.0 + day >= 15821015.0 )
    {
        a = year / 100;
        b = 2 - a + a / 4;
    }
    jul_day = (uint64) ( 365.25 * year - year_corr ) +
              (uint64) ( 30.6001 * ( month + 1 ) ) + day + 1720995L + b;

#else

    uint64 lmonth = (uint64) iMonth, lday = (uint64) iDay, lyear = (uint64) iYear;

    // Adjust BC years
    if ( lyear < 0 )
        lyear++;

    jul_day = lday - 32075L +
        1461L * ( lyear + 4800L + ( lmonth - 14L ) / 12L ) / 4L +
        367L * ( lmonth - 2L - ( lmonth - 14L ) / 12L * 12L ) / 12L -
        3L * ( ( lyear + 4900L + ( lmonth - 14L ) / 12L ) / 100L ) / 4L;

#endif

    return jul_day;
}

void Date::JdToYmd( const uint64 lJD, int *piYear, int *piMonth, int *piDay )
{
#ifndef JULDATE_USE_ALTERNATE_METHOD

    uint64 a, b, c, d, e, z, alpha;

    z = lJD;
    if ( z < 2299161L )
        a = z;
    else
    {
        alpha = (uint64) ( ( z - 1867216.25 ) / 36524.25 );
        a = z + 1 + alpha - alpha / 4;
    }
    b = a + 1524;
    c = (uint64) ( ( b - 122.1 ) / 365.25 );
    d = (uint64) ( 365.25 * c );
    e = (uint64) ( ( b - d ) / 30.6001 );
    *piDay = (int) b - d - (uint64) ( 30.6001 * e );
    *piMonth = (int) ( e < 13.5 ) ? e - 1 : e - 13;
    *piYear = (int) ( *piMonth > 2.5 ) ? ( c - 4716 ) : c - 4715;
    if ( *piYear <= 0 )
        *piYear -= 1;

#else

    uint64 t1, t2, yr, mo;

    t1 = lJD + 68569L;
    t2 = 4L * t1 / 146097L;
    t1 = t1 - ( 146097L * t2 + 3L ) / 4L;
    yr = 4000L * ( t1 + 1L ) / 1461001L;
    t1 = t1 - 1461L * yr / 4L + 31L;
    mo = 80L * t1 / 2447L;
    *piDay = (int) ( t1 - 2447L * mo / 80L );
    t1 = mo / 11L;
    *piMonth = (int) ( mo + 2L - 12L * t1 );
    *piYear = (int) ( 100L * ( t2 - 49L ) + yr + t1 );

    // Correct for BC years
    if ( *piYear <= 0 )
        *piYear -= 1;

#endif

    return;
}

void Date::Init(time_t t)
{
	struct tm *ptm;
	ptm = localtime( &t );
	m_y = ptm->tm_year + 1900;
	m_m = ptm->tm_mon + 1;
	m_d = ptm->tm_mday;
	m_lJulianDay = YmdToJd( m_y, m_m, m_d );
}

Date::Date()
{
	time_t t;
	time( &t );
	Init(t);
}

Date::Date(int year, int mo, int day)
: m_y(year), m_m(mo), m_d(day)
{
	if ( year < 100 )
	{
		year += 2000;
		m_y = year;
	}
	ASSERT( mo > 0 && mo < 13 );
	ASSERT( day > 0 && day < 32 );
	m_lJulianDay = YmdToJd( year, mo, day );
}

Date::Date( time_t tt )
{
	Init( tt );
}

Date::Date(const Date& dt)
{
	m_lJulianDay = dt.m_lJulianDay;
   	JdToYmd( m_lJulianDay, &m_y, &m_m, &m_d );
}

Date Date::Parse(const String& str)
{
	RefCountPtr<Vector<StringPtr> > dateparts;
	int mpos = 0;
	int dpos = 1;
	int ypos = 2;
	if ( str.IndexOf('/') > -1 )
	{
		dateparts = str.Split("/");
	}
	else
	{
		dateparts = str.Split("-");
		if (dateparts->Count() > 0 && dateparts->ElementAtRef(0)->Length() == 4)
		{
			ypos = 0;
			mpos = 1;
			dpos = 2;
		}
		else
		{
			mpos = 1;
			dpos = 0;
		}
	}
	if ( dateparts->Count() != 3 )
	{
		throw new InvalidArgumentException("Invalid date");
	}

	int m = Int32::Parse(*dateparts->ElementAt(mpos));
	int d = Int32::Parse(*dateparts->ElementAt(dpos));
	int y = Int32::Parse(*dateparts->ElementAt(ypos));

	return Date(y, m, d);
}

bool Date::IsDate(const String& str)
{
	try
	{
		Date::Parse(str);
		return true;
	}
	catch (Exception *ex)
	{
		delete ex;
		return false;
	}
}

Date::~Date()
{
}

void Date::AddMonths( int months )
{
	if ( months > 12 )
	{
		AddYears( months / 12 );
		months %= 12;
	}
	m_m += months;
	if ( m_m > 12 )
	{
		m_y++;
		m_m -= 12;
	}
	m_lJulianDay = YmdToJd( m_y, m_m, m_d );
   	JdToYmd( m_lJulianDay, &m_y, &m_m, &m_d );
}

uint32 Date::HashCode() const
{
#ifdef _WINDOWS
	return Math::Hash( (m_lJulianDay & 0xFFFFFFFFL) ^ (m_lJulianDay / (int64)pow(2.0, 32)) );
#else
	return Math::Hash( (uint32)((m_lJulianDay & 0xFFFFFFFFL) ^ (m_lJulianDay >> 32L)) );
#endif
}

bool Date::Equals(const IComparable *a) const
{
	if (a->MajicNumber() != MajicNumber())
	{
		return 1;
	}
	const Date *vp = static_cast<const Date *>(a);
	return Equals(vp);
}

int Date::Compare(const IComparable *a) const
{
	if (a->MajicNumber() != MajicNumber())
	{
		return 1;
	}
	const Date *vp = static_cast<const Date *>(a);
	return (int)(m_lJulianDay - vp->m_lJulianDay);
}

bool Date::Equals(const IComparable& a) const
{
	if (a.MajicNumber() != MajicNumber())
	{
		return 1;
	}
	return Equals(static_cast<const Date &>(a));
}

int Date::Compare(const IComparable& a) const
{
	if (a.MajicNumber() != MajicNumber())
	{
		return 1;
	}
	const Date& vp = static_cast<const Date&>(a);
	return (int)(m_lJulianDay - vp.m_lJulianDay);
}

int32 Date::MajicNumber() const
{
	return DATE_MAJIC;
}

/*
bool Date::IsHoliday()
{
	switch (m_m)
	{
		case 1:
			//	New Years
			//	RULE
			//	January 1.  If on a Saturday, the previous friday is off. If on a Sunday then Monday is off
			if (m_d == 1 && (DayOfWeek() != DayOfWeek::SATURDAY && DayOfWeek() != DayOfWeek::SUNDAY))
			{
				return true;
			}
			if (m_d == 2 && DayOfWeek == DayOfWeek.Monday)
				return true;

			//	MLK Day
			//	RULE
			//	celebrated as third Monday in January, King's birthday is Jan 18.
			if (DayOfWeek == DayOfWeek.Monday)
			{
				Date mlkDay = new Date(Year, 1, 1);
				while (mlkDay.DayOfWeek != DayOfWeek.Monday)
				{
					mlkDay = mlkDay.AddDays(1);
				}
				mlkDay = mlkDay.AddDays(14);

				if (this.Equals(mlkDay))
				{
					return true;
				}
			}
			break;
		case 2:
			//	Presidents' Day
			//	RULE
			//	Third Monday in February.  Replaced Lincoln and Washington's birthday in 1971.
			if (DayOfWeek == DayOfWeek.Monday)
			{
				Date presDay = new Date(Year, 2, 1);
				while (presDay.DayOfWeek != DayOfWeek.Monday)
				{
					presDay = presDay.AddDays(1);
				}
				presDay = presDay.AddDays(14);

				if (this.Equals(presDay))
				{
					return true;
				}
			}
			break;
		case 5:
			//	Memorial Day
			//	RULE
			//	Last Monday in May. Originally May 31
			if (DayOfWeek == DayOfWeek.Monday)
			{
				if (AddDays(7).Month() == 6)
				{
					return true;
				}
			}
			break;
		case 7:
			// July 4th
			// RULE
			//	If on a Saturday, then friday is off
			//	If on a Sunday, then monday is off
			if (m_d == 4 && (DayOfWeek != DayOfWeek.Saturday && DayOfWeek != DayOfWeek.Sunday))
			{
				return true;
			}
			if (m_d == 3 && DayOfWeek == DayOfWeek.Friday)
			{
				return true;
			}
			if (m_d == 5 && DayOfWeek == DayOfWeek.Monday)
			{
				return true;
			}
			break;
		case 9:
			//	Labor Day
			//	RULE
			//	First Monday in September. In 1882..1883 it was celebrated on September 5.
			if (DayOfWeek == DayOfWeek.Monday)
			{
				if (AddDays(-7).Month == 8)
				{
					return true;
				}
			}
			break;
		case 10:
			//	Columbus Day
			//	RULE
			//	1905 .. 1970 -> October 12
			//	1971 .. now  -> Second Monday in October.
			if (DayOfWeek == DayOfWeek.Monday)
			{
				Date columbusDay = new Date(Year, 10, 1);
				while (columbusDay.DayOfWeek != DayOfWeek.Monday)
				{
					columbusDay = columbusDay.AddDays(1);
				}
				columbusDay = columbusDay.AddDays(7);

				if (this.Equals(columbusDay))
				{
					return true;
				}
			}
			break;
		case 11:
			// Veterans' Day
			// RULE
			// November 11.  If on a Saturday, the previous friday is off. If on a Sunday then Monday is off
			if (m_d == 11 && DayOfWeek != DayOfWeek.Saturday && DayOfWeek != DayOfWeek.Sunday)
			{
				return true;
			}
			if (m_d == 10 && DayOfWeek == DayOfWeek.Friday)
			{
				return true;
			}
			if (m_d == 12 && DayOfWeek == DayOfWeek.Monday)
			{
				return true;
			}

			//  Thanksgiving
			//	RULE
			//	1621         -> first Thanksgiving, precise date unknown.
			//	1622         -> was no Thanksgiving.
			//	1623 .. 1675 -> precise date unknown.
			//	1676 .. 1862 -> June 29.
			//	1863 .. 1938 -> last Thursday of November.
			//	1939 .. 1941 -> 2nd to last Thursday of November.
			//	1942 .. now  -> 4th Thursday of November.
			if (DayOfWeek == DayOfWeek.Thursday)
			{
				DateTime thanksgivingDay = new DateTime(Year, 11, 1);
				while (thanksgivingDay.DayOfWeek != DayOfWeek.Thursday)
				{
					thanksgivingDay = thanksgivingDay.AddDays(1);
				}
				thanksgivingDay = thanksgivingDay.AddDays(21);

				if (this.Equals(thanksgivingDay.Date))
				{
					return true;
				}
			}
			else if (DayOfWeek == DayOfWeek.Friday)
			{
				// the day after is also a bank holiday
				Date dayAfterTg = new Date(Year, 11, 1);

				while (dayAfterTg.DayOfWeek != DayOfWeek.Thursday)
				{
					dayAfterTg = dayAfterTg.AddDays(1);
				}
				dayAfterTg = dayAfterTg.AddDays(22);

				if (this.Equals(dayAfterTg))
				{
					return true;
				}
			}
			break;
		case 12:
			//	Christmas
			//	RULE
			//	 If on a saturday, then friday is off
			//	 If on a sunday, then monday is off
			if (m_d == 25 && (DayOfWeek != DayOfWeek.Saturday && DayOfWeek != DayOfWeek.Sunday))
			{
				return true;
			}

			if (m_d == 24 && DayOfWeek == DayOfWeek.Friday)
			{
				// the 25th is on saturday
				return true;
			}

			if (m_d == 26 && DayOfWeek == DayOfWeek.Monday)
			{
				// the 25th is on sunday
				return true;
			}

			//	New Years
			//	RULE
			//	January 1.  If on a Saturday, the previous friday is off
			if (m_d == 31 && DayOfWeek == DayOfWeek.Friday)
			{
				// the friday of Dec 31 is a holiday (jan 1 is on saturday)
				return true;
			}
			break;
	}
	return false;
}
*/

#ifdef DEBUG
void Date::ValidateMem() const
{
}

void Date::CheckMem() const
{
}
#endif

DateTime::DateTime()
{
	time(&m_timet);
	m_dtm = *localtime( &m_timet );
}

DateTime::DateTime( const time_t t )
{
	m_timet = t;
	m_dtm = *localtime( &t );
}

DateTime::DateTime( int year, int month, int day )
{
	m_dtm.tm_year = year - 1900 ;
	m_dtm.tm_mon = month-1;
	m_dtm.tm_mday = day;
	m_dtm.tm_hour = 0;
	m_dtm.tm_min = 0;
	m_dtm.tm_sec = 0;
	m_dtm.tm_isdst = 0;

	m_timet = mktime( (struct tm *)&m_dtm );
}

DateTime::DateTime(const Date& dt)
{
	m_dtm.tm_year = dt.Year() ;
	m_dtm.tm_mon = dt.Month();
	m_dtm.tm_mday = dt.Day();
	m_dtm.tm_hour = 0;
	m_dtm.tm_min = 0;
	m_dtm.tm_sec = 0;
	m_dtm.tm_isdst = 0;

	m_timet = mktime( (struct tm *)&m_dtm );
}

DateTime::DateTime( int year, int month, int day, int hour, int minute, int second )
{
	ASSERT( hour > -1 && hour < 24 && minute > -1 && minute < 60 && second > -1 && second < 60 );
	m_dtm.tm_year = year - 1900 ;
	m_dtm.tm_mon = month-1;
	m_dtm.tm_mday = day;
	m_dtm.tm_hour = hour;
	m_dtm.tm_min = minute;
	m_dtm.tm_sec = second;
	m_dtm.tm_isdst = 0;

	m_timet = mktime( (struct tm *)&m_dtm );
}

DateTime::DateTime(const DateTime& dtm)
{
	m_dtm = dtm.m_dtm;
	m_timet = dtm.m_timet;
}

DateTime::~DateTime()
{
}

DateTime DateTime::Parse(const String& str)
{
	RefCountPtr<Vector<StringPtr> > dtparts = str.Split(" ");
	if (dtparts->Count() == 0)
	{
		throw new InvalidArgumentException("Invalid time format");
	}

	// process date
	Date dt(Date::Parse(dtparts->ElementAt(0)));

	int m = dt.Month();
	int d = dt.Day();
	int y = dt.Year();

	if ( dtparts->Count() == 1 )
	{
		return DateTime(y, m, d);
	}

	int h, min, s = 0, mm = 0;
	char buf[3];
	int pos = 0;
	StringPtr time = dtparts->ElementAt(1);
	if ( dtparts->ElementAt(1)->IndexOf(':') > -1 )
	{
		// AM/PM format
		int bpos = 0;
		buf[bpos++] = time->CharAt(pos++);
		if ( time->CharAt(pos) != ':' )
		{
			buf[bpos++] = time->CharAt(pos++);
		}
		buf[bpos] = '\0';
		h = Int32::Parse(buf);

		pos++;	// skip :

		bpos = 0;
		buf[bpos++] = time->CharAt(pos++);
		if ( time->CharAt(pos) != ':' )
		{
			buf[bpos++] = time->CharAt(pos++);
		}
		buf[bpos] = '\0';
		min = Int32::Parse(buf);

		if ( time->CharAt(pos) == ':' )
		{
			// seconds
			pos++;	// skip :

			bpos = 0;
			buf[bpos++] = time->CharAt(pos++);
			if ( isdigit(time->CharAt(pos)) )
			{
				buf[bpos++] = time->CharAt(pos++);
			}
			buf[bpos] = '\0';
			s = Int32::Parse(buf);

			if ( time->CharAt(pos) == '.' )
			{
				// millis
				pos++;	// skip .

				StringPtr smm = time->Substring(pos);
				mm = Int32::Parse(*smm);
			}

			while ( time->CharAt(pos) == ' ' )
			{
				pos++;
			}
			if ( 'P' == time->CharAt(pos) )
			{
				// PM
				h += 12;
			}
		}
	}
	else
	{
		// military
		if ( 3 == dtparts->Count() )
		{
			throw new InvalidArgumentException("Invalid time format");
		}
		buf[0] = time->CharAt(pos++);
		buf[1] = time->CharAt(pos++);
		buf[2] = '\0';
		h = Int32::Parse(buf);
		buf[0] = time->CharAt(pos++);
		buf[1] = time->CharAt(pos++);
		buf[2] = '\0';
		min = Int32::Parse(buf);
	}

	return DateTime(y, m, d, h, min, s/*, mm*/);
}

bool DateTime::IsDateTime(const String& str)
{
	try
	{
		DateTime::Parse(str);
		return true;
	}
	catch (Exception *ex)
	{
		delete ex;
		return false;
	}
}

DateTime DateTime::AddSeconds(int64 seconds) const
{
	///@TODO optimize
	DateTime dt1(2000, 1, 1, 1, 1, 0);
	DateTime dt2(2000, 1, 1, 1, 1, 1);

	double timetSecond = difftime(dt2.m_timet, dt1.m_timet);
	ASSERT(0 < timetSecond);
	return DateTime( (time_t)((time_t)(seconds * timetSecond) + m_timet) );
}

DateTime DateTime::AddMinutes(int minutes) const
{
	return AddSeconds(minutes * 60);
}

DateTime DateTime::AddHours(int hours) const
{
	return AddSeconds(hours * 60L * 60L);
}

DateTime DateTime::AddDays(int days) const
{
	return AddSeconds(days * 24L * 60L * 60L);
}

DateTime DateTime::AddMonths(int months) const
{
	int year = Year() + months / 12;
	int day = Day();
	int month = Month() + months % 12;

	if (_daysPerMonth[month-1] > day)
	{
		if (Date::IsLeapYear(year) && month == 2)
		{
			if (day > 29)
			{
				day = 29;
			}
		}
		else
		{
			day = _daysPerMonth[month-1];
		}
	}
	return DateTime(year, month, day, Hour(), Minutes(), Seconds());
}

DateTime DateTime::AddYears(int years) const
{
	int year = Year() + years;
	int month = Month();
	int day = Day();

	if (!Date::IsLeapYear(year) && Month() == 2)
	{
		if (day > 28)
		{
			day = 28;
		}
	}
	return DateTime(year, month, day, Hour(), Minutes(), Seconds());
}

uint32 DateTime::HashCode() const
{
	return Math::Hash(((m_dtm.tm_year - 1900) + m_dtm.tm_yday * 1000) ^ ((m_dtm.tm_hour + m_dtm.tm_min * 60 + m_dtm.tm_sec * 3600) << 16));
}

bool DateTime::Equals(const IComparable *a) const
{
	if (a->MajicNumber() != MajicNumber())
	{
		return false;
	}
	const DateTime *vp = static_cast<const DateTime *>(a);
	return Equals(vp);
}

int DateTime::Compare(const IComparable *a) const
{
	if (a->MajicNumber() != MajicNumber())
	{
		return 1;
	}
	const DateTime *vp = static_cast<const DateTime *>(a);
	if (Equals(vp))
	{
		return 0;
	}
	return (int)DiffInSeconds(*vp);
}

bool DateTime::Equals(const IComparable& a) const
{
	if (a.MajicNumber() != MajicNumber())
	{
		return false;
	}
	const DateTime& vp = static_cast<const DateTime&>(a);
	return Equals(vp);
}

int DateTime::Compare(const IComparable& a) const
{
	if (a.MajicNumber() != MajicNumber())
	{
		return 1;
	}
	const DateTime& vp = static_cast<const DateTime&>(a);
	if (Equals(vp))
	{
		return 0;
	}
	return (int)DiffInSeconds(vp);
}

int32 DateTime::MajicNumber() const
{
	return DATETIME_MAJIC;
}

StringPtr DateTime::ToStringISO8601() const
{
	StringBuffer buf(21);
	buf.Append(Int32::ToString(Year()));
	buf.Append('-');
	int i = Month();
	if (i < 10)
	{
		buf.Append('0');
	}
	buf.Append(Int32::ToString(i));
	buf.Append('-');
	if ((i = Day()) < 10)
	{
		buf.Append('0');
	}
	buf.Append(Int32::ToString(i));
	buf.Append(' ');
	if ((i = Hour()) < 10)
	{
		buf.Append('0');
	}
	buf.Append(Int32::ToString(i));
	buf.Append(':');
	if ((i = Minutes()) < 10)
	{
		buf.Append('0');
	}
	buf.Append(Int32::ToString(i));
	buf.Append(':');
	if ((i = Seconds()) < 10)
	{
		buf.Append('0');
	}
	buf.Append(Int32::ToString(i));

	return buf.ToString();
}

StringPtr DateTime::ToTimeString() const
{
	StringBuffer buf(11);
	int i;
	if ((i = Hour()) < 10)
	{
		buf.Append('0');
	}
	buf.Append(Int32::ToString(i));
	buf.Append(':');
	if ((i = Minutes()) < 10)
	{
		buf.Append('0');
	}
	buf.Append(Int32::ToString(i));
	buf.Append(':');
	if ((i = Seconds()) < 10)
	{
		buf.Append('0');
	}
	buf.Append(Int32::ToString(i));

	return buf.ToString();
}

#ifdef DEBUG
void DateTime::ValidateMem() const
{
}

void DateTime::CheckMem() const
{
}
#endif
