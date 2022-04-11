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
#ifndef _xmlcomment_h
#define _xmlcomment_h

#include <spl/Debug.h>
#include <spl/xml/XmlNode.h>

namespace spl
{
/** 
 * @defgroup xml XML
 * @{
 */

/** @brief XML comment.
 *
 */
class XmlComment : public XmlNode
{
protected:
	friend class XmlNode;

	void CopyTo( XmlComment& target ) const;

	/// Construct a comment from text.
	XmlComment( const String& _value );
	
public:
	/// Constructs an empty comment.
	XmlComment();
	
	XmlComment( const XmlComment& );
	
	virtual ~XmlComment();
			
	void operator=( const XmlComment& base );

	virtual String Name() const;
			
	/// Returns a copy of this Comment.
	virtual XmlNodePtr Clone() const;
	
	virtual XmlCommentPtr  ToComment() const; ///< Cast to a more defined type. Will return null not of the requested type.

	virtual StringPtr ToString() const;
	virtual void WriteTo(TextWriter& writer) const;
	
	/*	Attribtue parsing starts: at the ! of the !--
						 returns: next char past '>'
	*/
	virtual const char* _Parse( const char* p, XmlParsingData* data, XmlEncoding encoding );
};

REGISTER_TYPEOF( 532, XmlComment );

/** @} */
}
#endif
