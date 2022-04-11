/* Ported to SPL, original attribution below. */
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
#if !defined(STRINGTOKENIZER_HEADER_GUARD_1357924680)
#define STRINGTOKENIZER_HEADER_GUARD_1357924680

#include <spl/String.h>
#include <spl/RefCountPtr.h>
#include <spl/WeakReference.h>

namespace spl
{
/** 
 * @defgroup text Text
 * @{
 */

class StringTokenizer;
typedef RefCountPtr<StringTokenizer> StringTokenizerPtr;
typedef WeakReference<StringTokenizer, StringTokenizerPtr> StringTokenizerRef;

/** @brief Port of GNU Classpath StringTokenizer.
 *
 */
class StringTokenizer : public IMemoryValidate
{
private:
	static const char s_defaultTokens[];

    // Data members...
    String m_string;

	String m_tokens;

	bool m_returnTokens;

	int m_currentIndex;

public:

	/**
	 * Constructs a tokenizer for the target string.  No copies of any input
	 * strings are made.
	 *
	 * @param theString     string to tokenize
	 * @param theTokens     string of delimiters used to parse target, default
	 *                      is " \t\n\r"
	 * @param fReturnTokens if true, delimiter characters are also returned
	 *                      as tokens, default is false
	 */
	StringTokenizer(const String&	theString,
					const String&	theTokens = " \t\n\r",
					bool fReturnTokens = false);

	StringTokenizer(const StringTokenizer&);

    StringTokenizer& operator =(const StringTokenizer&);

	virtual ~StringTokenizer();

	/**
	 * Determine if there are tokens remaining
	 * 
	 * @return true if there are more tokens
	 */
	bool HasMoreTokens() const;

	/**
	 * Retrieve the next token to be parsed; behavior is undefined if there are
	 * no more tokens
	 * 
	 * @param theToken next token string
	 */
	void NextToken(String&	theToken);

	/**
	 * Count the number of tokens yet to be parsed
	 * 
	 * @return number of remaining tokens
	 */
	int	CountTokens() const;

	inline void Reset()
	{
		m_currentIndex = 0;
	}

#if defined(DEBUG) || defined(_DEBUG)
	void CheckMem() const;
	void ValidateMem() const;
#endif

protected:

	int FindNextDelimiterIndex(int theStartIndex) const;

private:

    // These are not implemented...
	inline bool	operator ==(const StringTokenizer&) const { return false; }
};

/** @} */
}
#endif	// STRINGTOKENIZER_HEADER_GUARD_1357924680
