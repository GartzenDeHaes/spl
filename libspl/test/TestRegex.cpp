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
#include <spl/text/Regex.h>

#ifdef DEBUG

#include <spl/io/log/Log.h>

using namespace spl;

static void _TestRegexIsMatch()
{
	{
		Regex rex("^-?\\d+(\\.\\d{2})?$");

		DEBUG_CLEAR_MEM_CHECK_POINTS();
		rex.CheckMem();
		DEBUG_DUMP_MEM_LEAKS();
		UNIT_ASSERT_MEM_NOTED("Regex 1.0");

		UNIT_ASSERT("-42", rex.IsMatch("-42"));
		UNIT_ASSERT("19.99", rex.IsMatch("19.99"));
		UNIT_ASSERT("0.001", !rex.IsMatch("0.001"));
		UNIT_ASSERT("100 USD", !rex.IsMatch("100 USD"));
		UNIT_ASSERT(".34", !rex.IsMatch(".34"));
		UNIT_ASSERT("0.34", rex.IsMatch("0.34"));
		UNIT_ASSERT("1,052.21", !rex.IsMatch("1,052.21"));

		DEBUG_CLEAR_MEM_CHECK_POINTS();
		rex.CheckMem();
		DEBUG_DUMP_MEM_LEAKS();
		UNIT_ASSERT_MEM_NOTED("Regex 1.0");
	}

	DEBUG_CLEAR_MEM_CHECK_POINTS();
	DEBUG_DUMP_MEM_LEAKS();
	UNIT_ASSERT_MEM_NOTED("Regex 1.2");
	Log::SWriteOkFail( "Regex test IsMatch" );
}

static void _TestRegexMatch()
{
	{
		Regex rex("^-?\\d+(\\.\\d{2})?$");
		
		StringPtr match = rex.Match("100 USD");
		UNIT_ASSERT("100 USD", match->Length() == 0);

		match = rex.Match("100 USD-42");
		UNIT_ASSERT("100 USD", match->Length() == 0);

		match = rex.Match("-42");
		UNIT_ASSERT("100 USD-42", match->Length() == 3);
		UNIT_ASSERT("100 USD-42", match->Equals("-42"));
	}

	DEBUG_CLEAR_MEM_CHECK_POINTS();
	DEBUG_DUMP_MEM_LEAKS();
	UNIT_ASSERT_MEM_NOTED("Regex 1.1");

	{
		Regex rex("-?\\d+(\\.\\d{2})?$");
		
		StringPtr match = rex.Match("100 USD");
		UNIT_ASSERT("100 USD", match->Length() == 0);

		match = rex.Match("100 USD-42");
		UNIT_ASSERT("100 USD-42", match->Length() == 3);
		UNIT_ASSERT("100 USD-42", match->Equals("-42"));
	}

	DEBUG_CLEAR_MEM_CHECK_POINTS();
	DEBUG_DUMP_MEM_LEAKS();
	UNIT_ASSERT_MEM_NOTED("Regex 1.2");
	Log::SWriteOkFail( "Regex test Match" );
}

static void _TestRegexMatches()
{
	{
		Regex rex("-?\\d+(\\.\\d{2})?$");
		
		RefCountPtr<Array<StringPtr> > matches = rex.Matches("-42 19.99 0.001 100USD .34 0.34 1,052.21");
		UNIT_ASSERT("Mateches count", matches->Count() == 2);

		StringPtr v(matches->ElementAt(0));
		UNIT_ASSERT("Match 1", v->Equals("052.21"));
		v = matches->ElementAt(1);
		UNIT_ASSERT("Match 2", v->Equals(".21"));
	}

	DEBUG_CLEAR_MEM_CHECK_POINTS();
	DEBUG_DUMP_MEM_LEAKS();
	UNIT_ASSERT_MEM_NOTED("Regex 1.2");
	Log::SWriteOkFail( "Regex test Matches" );
}

void _TestRegex()
{
	_TestRegexIsMatch();
	DEBUG_CLEAR_MEM_CHECK_POINTS();
	DEBUG_DUMP_MEM_LEAKS();

	_TestRegexMatch();
	DEBUG_CLEAR_MEM_CHECK_POINTS();
	DEBUG_DUMP_MEM_LEAKS();

	_TestRegexMatches();
	DEBUG_CLEAR_MEM_CHECK_POINTS();
	DEBUG_DUMP_MEM_LEAKS();
}


#endif
