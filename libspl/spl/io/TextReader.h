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
#ifndef _textreader_h
#define _textreader_h

#include <spl/types.h>
#include <spl/Debug.h>
#include <spl/collection/Array.h>
#include <spl/io/IStream.h>
#include <spl/Memory.h>
#include <spl/RefCountPtr.h>
#include <spl/String.h>
#include <spl/text/StringBuffer.h>

namespace spl
{
/** 
 * @defgroup stream Streams
 * @ingroup io
 * @{
 */

	class TextReader;
	typedef RefCountPtr<TextReader> TextReaderPtr;
	typedef WeakReference<TextReader, TextReaderPtr> TextReaderRef;

	class TextReader : public IMemoryValidate
	{
	private:
		// forbid copy constructor
		inline TextReader(const TextReader& pmp) {}
		inline void operator =(const TextReader& pmp) {}

	private:
		inline int _Read()
		{
			if ( m_peekch > -1 )
			{
				int ch = m_peekch;
				m_peekch = -1;
				return ch;
			}
			return m_strm->ReadByte();
		}

	protected:
		IStreamPtr m_strm;
		int m_peekch;

	public:
		TextReader(IStreamPtr strm);
		virtual ~TextReader();

		void Close();
		int Peek();
		int Read();
		void UnRead( int ch );
		
		int ReadBlock(Array<byte>& buf, int offset, int count);
		bool ReadLine(StringBuffer& sb);
		StringPtr ReadLine();
		bool ReadLine(Array<byte>& buf, int& lineLen);
		StringPtr ReadToEnd();

		inline bool CanRead() const { return m_strm->CanRead(); }
		inline int Length() const { return m_strm->Length(); }

	#ifdef DEBUG
		virtual void ValidateMem() const;
		virtual void CheckMem() const;
	#endif
	};

/** @} */
}
#endif
