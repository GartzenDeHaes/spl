/*
 * Copyright 1999-2004 The Apache Software Foundation.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
// Class header file.
#include <spl/text/StringTokenizer.h>

using namespace spl;

const char StringTokenizer::s_defaultTokens[] =
{
	' ',
	'\t',
	'\n',
	'\r',
	0,
};

StringTokenizer::StringTokenizer(
			const String&	theString,
			const String&	theTokens,
			bool fReturnTokens) :
	m_string(theString),
	m_tokens(theTokens),
	m_returnTokens(fReturnTokens),
	m_currentIndex(0)
{
	ASSERT(m_string.Length() != 0 && m_tokens.Length() != 0);
}

StringTokenizer::StringTokenizer(const StringTokenizer& st)
:		m_string(st.m_string),
		m_tokens(st.m_tokens),
		m_returnTokens(st.m_returnTokens),
		m_currentIndex(st.m_currentIndex)
{
}

StringTokenizer& StringTokenizer::operator =(const StringTokenizer& st)
{
	m_string = st.m_string;
	m_tokens = st.m_tokens;
	m_returnTokens = st.m_returnTokens;
	m_currentIndex = st.m_currentIndex;

	return *this;
}

StringTokenizer::~StringTokenizer()
{
}

bool
StringTokenizer::HasMoreTokens() const
{
	int theCount = 0;
	int	theCurrentIndex = m_currentIndex;

	if (theCurrentIndex < m_string.Length())
	{
		while(theCurrentIndex < m_string.Length())
		{
			const int theNextIndex = FindNextDelimiterIndex(theCurrentIndex);

			if (theNextIndex == theCurrentIndex)
			{
				theCurrentIndex = theNextIndex + 1;

				if (m_returnTokens == true)
				{
					return true;
				}
			}
			else
			{
				return true;
			}
		}
	}

	return false;
}

void
StringTokenizer::NextToken(String& theToken)
{
	ASSERT(m_currentIndex < m_string.Length());

	// Find the index of the next delimiter.
	int	theIndex = FindNextDelimiterIndex(m_currentIndex);

	if (theIndex == m_currentIndex)
	{
		m_currentIndex = theIndex + 1;

		if (m_returnTokens == true)
		{
			// The next delimiter is at the current index.  If we're
			// returning delimiters as tokens, then make that the
			// return value.  Otherwise, return an empty string.
			theToken = *m_string.Mid(theIndex, theIndex + 1);
		}
		else if (m_currentIndex < m_string.Length())
		{
			 NextToken( theToken );
		}
	}
	else
	{
		if (theIndex == m_currentIndex)
		{
			theIndex = FindNextDelimiterIndex(m_currentIndex + 1);
		}
		ASSERT(theIndex > m_currentIndex);

		theToken = *m_string.Mid(m_currentIndex, theIndex);

		m_currentIndex = theIndex;
	}
}

int
StringTokenizer::CountTokens() const
{
	int theCount = 0;
	int	theCurrentIndex = m_currentIndex;

	if (theCurrentIndex < m_string.Length())
	{
		while(theCurrentIndex < m_string.Length())
		{
			const int theNextIndex = FindNextDelimiterIndex(theCurrentIndex);

			if (theNextIndex == theCurrentIndex)
			{
				theCurrentIndex = theNextIndex + 1;

				if (m_returnTokens == true)
				{
					theCount++;
				}
			}
			else
			{
				theCount++;

				theCurrentIndex = theNextIndex;
			}
		}
	}

	return theCount;
}

int
StringTokenizer::FindNextDelimiterIndex(int	theStartIndex) const
{
	bool fTokenFound = false;
	int	theIndex = theStartIndex;

	while(theIndex < m_string.Length() && fTokenFound == false)
	{
		const char theCurrentChar = m_string[theIndex];

		if (m_tokens.IndexOf(theCurrentChar) > -1)
		{
			fTokenFound = true;
		}
		else
		{
			theIndex++;
		}
	}

	return theIndex;
}

#if defined(DEBUG) || defined(_DEBUG)
void StringTokenizer::CheckMem() const
{
	m_string.CheckMem();
	m_tokens.CheckMem();
}

void StringTokenizer::ValidateMem() const
{
	m_string.ValidateMem();
	m_tokens.ValidateMem();
}
#endif

