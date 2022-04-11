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
#ifndef _textwriter_h
#define _textwriter_h

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

	class TextWriter;
	typedef RefCountPtr<TextWriter> TextWriterPtr;
	typedef WeakReference<TextWriter, TextWriterPtr> TextWriterRef;

	/** @brief Writes text to a stream, eg numerics are converted to strings for writing.
	 *
	 */
	class TextWriter : public IMemoryValidate
	{
	private:
		// forbid copy constructor
		inline TextWriter(const TextWriter& tw) {}
		inline void operator =(const TextWriter& tw) {}

	protected:
		IStreamPtr m_strm;

	public:
		TextWriter(IStreamPtr strm);
		virtual ~TextWriter();

		void Close();
		void Flush();
		void Write(byte b);
		void Write(int8 i);
		void Write(int16 i);
		void Write(int32 i);
		void Write(int64 i);
		void Write(uint16 i);
		void Write(uint32 i);
		void Write(float32 f);
		void Write(float64 f);
		void Write(Array<char>& buf);
		void Write(const String& str);
		void WriteFormat(const String fmt, ...);
		void WriteLine(const String& str);

	#ifdef DEBUG
		virtual void ValidateMem() const;
		virtual void CheckMem() const;
	#endif
	};

/** @} */
}
#endif
