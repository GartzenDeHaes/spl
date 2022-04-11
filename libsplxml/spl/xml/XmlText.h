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
#ifndef _xmltext_h
#define _xmltext_h

#include <spl/Debug.h>
#include <spl/text/StringBuffer.h>
#include <spl/xml/XmlNode.h>
#include <spl/xml/XmlParsingData.h>

namespace spl
{
	extern void _XmlEncodeString( const String& str, StringBuffer& outString );

	/** 
	 * @defgroup xml XML
	 * @{
	 */

	/** @brief Text xml data node.
	 *
	 */
	class XmlText : public XmlNode
	{
	protected:
		friend class XmlDocument;
		friend class XmlElement;
		friend class TinyXmlParser;
		friend class XmlNode;

		bool m_cdata;
		
		///  [internal use] Creates a new Element and returns it.
		virtual XmlNodePtr Clone() const;
		void CopyTo( XmlText& target ) const;

		virtual const char* _Parse( const char* p, XmlParsingData* data, XmlEncoding encoding );

	public:
		/** Constructor for text element. By default, it is treated as 
			normal, encoded text. If you want it be output as a CDATA text
			element, set the parameter _cdata to 'true'
		*/
		XmlText (const String& initValue );

		XmlText( const XmlText& copy );
		virtual ~XmlText();
		
		inline void operator =( const XmlText& base )
		{ 
			base.CopyTo( *this ); 
		}

		virtual String Name() const;
				
		/// Queries whether this represents text using a CDATA section.
		inline bool CDATA() const				
		{ 
			return m_cdata; 
		}
		
		/// Turns on or off a CDATA representation of text.
		inline void SetCDATA( bool _cdata )
		{ 
			m_cdata = _cdata; 
		}

		virtual XmlTextPtr ToText() const ; ///< Cast to a more defined type. Will return null not of the requested type.
		
		bool IsBlank() const;	// returns true if all white space and new lines
		
		virtual StringPtr ToString() const;
		virtual void WriteTo(TextWriter& writer) const;

		static XmlTextPtr CreateText(const String& val);

		inline static void XmlEncode(const String& str, StringBuffer& outString)
		{
			_XmlEncodeString(str, outString);
		}
	};

	REGISTER_TYPEOF( 544, XmlText );

	/** @} */
}
#endif
