/*
 *  Much of this code is extracted from the tinyxml project, attributed below.
 */
/*
www.sourceforge.net/projects/tinyxml
Original code (2.0 and earlier )copyright (c) 2000-2006 Lee Thomason (www.grinninglizard.com)

This software is provided 'as-is', without any express or implied
warranty. In no event will the authors be held liable for any
damages arising from the use of this software.

Permission is granted to anyone to use this software for any
purpose, including commercial applications, and to alter it and
redistribute it freely, subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must
not claim that you wrote the original software. If you use this
software in a product, an acknowledgment in the product documentation
would be appreciated but is not required.

2. Altered source versions must be plainly marked as such, and
must not be misrepresented as being the original software.

3. This notice may not be removed or altered from any source distribution.
*/

/** 
 * @defgroup xml XML
 * @{
 */

#ifndef _xmlnode_h
#define _xmlnode_h

#include <spl/Debug.h>
#include <spl/Memory.h>
#include <spl/collection/List.h>
#include <spl/RefCountPtr.h>
#include <spl/RefCountPtrCast.h>
#include <spl/String.h>
#include <spl/io/TextStream.h>
#include <spl/WeakReference.h>
#include <spl/xml/XmlParsingData.h>

namespace spl
{
class XmlNode;
class XmlDocument;
class XmlElement;
class XmlComment;
class XmlUnknown;
class XmlAttribute;
class XmlText;
class XmlDeclaration;
class XmlNodeList;

/** @brief Pointer to XmlNode.  @ref XmlNode */
typedef RefCountPtr<XmlNode> XmlNodePtr;

typedef RefCountPtrCast<XmlDocument, XmlNode, XmlNodePtr> XmlDocumentPtr;		//< Cast from XmlDocumentPtr to XmlNodePtr.
typedef RefCountPtrCast<XmlElement, XmlNode, XmlNodePtr> XmlElementPtr;			//< Cast from XmlElementPtr to XmlNodePtr.
typedef RefCountPtrCast<XmlComment, XmlNode, XmlNodePtr> XmlCommentPtr;			//< Cast from XmlCommentPtr to XmlNodePtr.
typedef RefCountPtrCast<XmlUnknown, XmlNode, XmlNodePtr> XmlUnknownPtr;			//< Cast from XmlUnknownPtr to XmlNodePtr.
typedef RefCountPtrCast<XmlAttribute, XmlNode, XmlNodePtr> XmlAttributePtr;		//< Cast from XmlAttributePtr to XmlNodePtr.
typedef RefCountPtrCast<XmlText, XmlNode, XmlNodePtr> XmlTextPtr;				//< Cast from XmlTextPtr to XmlNodePtr.
typedef RefCountPtrCast<XmlDeclaration, XmlNode, XmlNodePtr> XmlDeclarationPtr;	//< Cast from XmlDeclarationPtr to XmlNodePtr.

REGISTER_TYPEOF( 489, XmlDocumentPtr );
REGISTER_TYPEOF( 491, XmlElementPtr );
REGISTER_TYPEOF( 492, XmlCommentPtr );
REGISTER_TYPEOF( 494, XmlUnknownPtr );
REGISTER_TYPEOF( 496, XmlAttributePtr );
REGISTER_TYPEOF( 499, XmlTextPtr );
REGISTER_TYPEOF( 500, XmlDeclarationPtr );

typedef WeakReference<XmlNode, XmlNodePtr> XmlNodeRef;						//< Weak ref to XmlNode.
typedef WeakReference<XmlDocument, XmlDocumentPtr> XmlDocumentRef;			//< Weak ref to XmlDocument.
typedef WeakReference<XmlElement, XmlElementPtr> XmlElementRef;				//< Weak ref to XmlElement.
typedef WeakReference<XmlComment, XmlCommentPtr> XmlCommentRef;				//<	Weak ref to XmlComment.
typedef WeakReference<XmlUnknown, XmlUnknownPtr> XmlUnknownRef;				//< Weak ref to XmlUnknown.
typedef WeakReference<XmlAttribute, XmlAttributePtr> XmlAttributeRef;		//< Weak ref to XmlAttribute.
typedef WeakReference<XmlText, XmlTextPtr> XmlTextRef;						//< Weak ref to XmlText.
typedef WeakReference<XmlDeclaration, XmlDeclarationPtr> XmlDeclarationRef;	//< Weak ref to XmlDeclaration.

REGISTER_TYPEOF( 502, XmlDocumentRef );
REGISTER_TYPEOF( 504, XmlElementRef );
REGISTER_TYPEOF( 506, XmlCommentRef );
REGISTER_TYPEOF( 508, XmlUnknownRef );
REGISTER_TYPEOF( 510, XmlAttributeRef );
REGISTER_TYPEOF( 512, XmlTextRef );
REGISTER_TYPEOF( 514, XmlDeclarationRef );

/** @brief Base XML node type.
 *
 */
class XmlNode : public IMemoryValidate
{
private:
	// copy constructor would probably need to do a deep copy, which undesireable
	inline XmlNode(const XmlNode& pmp) {}
	inline void operator =(const XmlNode& pmp) {}

public:
	/** The types of XML nodes supported by TinyXml. (All the
		unsupported types are picked up by UNKNOWN.)
	*/
	enum XmlNodeType
	{
		DOCUMENT,
		ELEMENT,
		COMMENT,
		UNKNOWN,
		TEXT,
		DECLARATION,
		TYPECOUNT,
		ATTRIBUTE
	};

	enum
	{
		TIXML_NO_ERROR = 0,
		TIXML_ERROR,
		TIXML_ERROR_OPENING_FILE,
		TIXML_ERROR_OUT_OF_MEMORY,
		TIXML_ERROR_PARSING_ELEMENT,
		TIXML_ERROR_FAILED_TO_READ_ELEMENT_NAME,
		TIXML_ERROR_READING_ELEMENT_VALUE,
		TIXML_ERROR_READING_ATTRIBUTES,
		TIXML_ERROR_PARSING_EMPTY,
		TIXML_ERROR_READING_END_TAG,
		TIXML_ERROR_PARSING_UNKNOWN,
		TIXML_ERROR_PARSING_COMMENT,
		TIXML_ERROR_PARSING_DECLARATION,
		TIXML_ERROR_DOCUMENT_EMPTY,
		TIXML_ERROR_EMBEDDED_NULL,
		TIXML_ERROR_PARSING_CDATA,
		TIXML_ERROR_DOCUMENT_TOP_ONLY,

		TIXML_ERROR_STRING_COUNT
	};

protected:
	friend class XmlDocument;
	friend class TinyXmlParser;
	friend class XmlElement;
	
	XmlNode*		m_parent;
	XmlNodeType		m_type;

	XmlNodePtr		m_firstChild;
	XmlNodePtr		m_lastChild;

	StringBuffer	m_value;

	XmlNodePtr		m_prev;
	XmlNodePtr		m_next;
			
	TiXmlCursor m_location;

	XmlNodeRef m_self;

	XmlNode( XmlNodeType _type );

	// Copy to the allocated object. Shared functionality between Clone, Copy constructor,
	// and the assignment operator.
	void CopyTo( XmlNode& target ) const;

	// Figure out what is at *p, and parse it. Returns null if it is not an xml node.
	XmlNodePtr Identify( const char* start, XmlEncoding encoding );

	virtual const char* _Parse(	const char* p, XmlParsingData* data, XmlEncoding encoding /*= TIXML_ENCODING_UNKNOWN */ ) = 0;
							  
public:
	class Iterator : public IIterator<XmlNodePtr>
	{
	private:
		List<XmlNodePtr> m_stack;
		XmlNodePtr m_current;
		
	public:
		Iterator(XmlNodePtr root);	//< root is not iterated
		Iterator(const Iterator& iter);
		virtual ~Iterator();
		Iterator& operator =(const Iterator iter);
		
		virtual bool Next();
		virtual bool Prev();

		virtual XmlNodePtr Current();
		virtual XmlNodePtr& CurrentRef();
	};
	
	virtual ~XmlNode();
	
	/** The meaning of 'value' changes for the specific type of
		TiXmlNode.
		@verbatim
		Document:	filename of the xml file
		Element:	name of the element
		Comment:	the comment text
		Unknown:	the tag contents
		Text:		the text string
		@endverbatim

		The subclasses will wrap this function.
	*/
	inline const StringPtr Value() const 
	{ 
		return m_value.ToString();
	}

	virtual String Name() const = 0;
	
	/** Changes the value of the node. Defined as:
		@verbatim
		Document:	filename of the xml file
		Element:	name of the element
		Comment:	the comment text
		Unknown:	the tag contents
		Text:		the text string
		@endverbatim
	*/
	inline void SetValue(const String& _value) 
	{ 
		m_value = _value;
	}

	/// Delete all the children of this node. Does not affect 'this'.
	void Clear();

	/// One step up the DOM.
	inline XmlNodePtr ParentNode() const
	{ 
		return NULL == m_parent ? XmlNodePtr() : m_parent->m_self; 
	}
	
	virtual RefCountPtr<XmlNodeList> ChildNodes() const;

	/// The first child of this node. Will be null if there are no children.
	inline XmlNodePtr FirstChild() const
	{
		return m_firstChild; 
	}
	
	/// The first child of this node with the matching 'value'. Will be null if none found.
	XmlNodePtr FirstChild( const String& value ) const;
	
	/// The last child of this node. Will be null if there are no children.
	inline XmlNodePtr LastChild() const
	{ 
		return m_lastChild; 
	}
		
	/// The last child of this node matching 'value'. Will be null if there are no children.
	XmlNodePtr LastChild( const String& value ) const;

	XmlNodePtr Child(int index) const;

	/** Add a new node related to this. Adds a child past the LastChild.

		NOTE: the node to be added is passed by pointer, and will be
		henceforth owned (and deleted) by tinyXml. This method is efficient
		and avoids an extra copy, but should be used with care as it
		uses a different memory model than the other insert functions.

		@sa InsertEndChild
	*/
	/** Add a new node related to this. Adds a child past the LastChild.
		Returns a pointer to the new object or NULL if an error occured.
	*/
	void AppendChild( XmlNodePtr addThis );

	/** Add a new node related to this. Adds a child before the specified child.
		Returns a pointer to the new object or NULL if an error occured.
	*/
	void InsertBefore( XmlNodePtr beforeThis, XmlNodePtr addThis );

	/** Add a new node related to this. Adds a child after the specified child.
		Returns a pointer to the new object or NULL if an error occured.
	*/
	void InsertAfter( XmlNodePtr afterThis, XmlNodePtr addThis );

	/** Replace a child of this node.
		Returns a pointer to the new object or NULL if an error occured.
	*/
	void ReplaceChild( XmlNodePtr replaceThis, XmlNodePtr withThis );

	/// Delete a child of this node.
	bool RemoveChild( XmlNodePtr removeThis );

	virtual RefCountPtr<XmlNodeList> Decendants();
	
	/// Navigate to a sibling node.
	inline XmlNodePtr PreviousSibling()		
	{ 
		return m_prev; 
	}
	
	/// Navigate to a sibling node.
	XmlNodePtr PreviousSibling( const String& );

	/// Navigate to a sibling node.
	inline XmlNodePtr NextSibling()				
	{ 
		return m_next; 
	}
	
	/// Navigate to a sibling node with the given 'value'.
	XmlNodePtr NextSibling( const String& );

	/** Convenience function to get through elements.
		Calls NextSibling and ToElement. Will skip all non-Element
		nodes. Returns 0 if there is not another element.
	*/
	XmlElementPtr NextSiblingElement();

	/** Convenience function to get through elements.
		Calls NextSibling and ToElement. Will skip all non-Element
		nodes. Returns 0 if there is not another element.
	*/
	XmlElementPtr NextSiblingElement( const String& );

	/// Convenience function to get through elements.
	XmlElementPtr FirstChildElement();

	/// Convenience function to get through elements.
	XmlElementPtr FirstChildElement( const String& _value );

	/** Query the type (as an enumerated value, above) of this node.
		The possible types are: DOCUMENT, ELEMENT, COMMENT,
								UNKNOWN, TEXT, and DECLARATION.
	*/
	inline XmlNodeType NodeType() const	
	{ 
		return m_type; 
	}

	/** Return a pointer to the Document this node lives in.
		Returns null if not in a document.
	*/
	XmlDocumentPtr GetDocument();

	/// @brief Returns true if this node has no children.
	inline bool HasChildNodes() const
	{ 
		return m_firstChild.IsNotNull(); 
	}
	
	/// @brief Performs a deep copy.
	virtual XmlNodePtr Clone() const = 0;

	virtual StringPtr InnerText() const;

	/// @brief Select all of the nodes matching the expression.
	RefCountPtr<XmlNodeList> SelectNodes(const String& xpath);

	/// @brief Select the first node matching the expression.  This may not be the first matching node in tree order.
	XmlNodePtr SelectSingleNode(const String& xpath);

	/** Return the position, in the original source file, of this node or attribute.
		The row and column are 1-based. (That is the first row and first column is
		1,1). If the returns values are 0 or less, then the parser does not have
		a row and column value.

		Generally, the row and column value will be set when the TiXmlDocument::Load(),
		TiXmlDocument::LoadFile(), or any TiXmlNode::Parse() is called. It will NOT be set
		when the DOM was created from operator>>.

		The values reflect the initial load. Once the DOM is modified programmatically
		(by adding or changing nodes and attributes) the new values will NOT update to
		reflect changes in the document.

		There is a minor performance cost to computing the row and column. Computation
		can be disabled if TiXmlDocument::SetTabSize() is called with 0 as the value.

		@sa TiXmlDocument::SetTabSize()
	*/
	inline int Row() const			{ return m_location.row + 1; }
	inline int Column() const		{ return m_location.col + 1; }	///< See Row()

	virtual XmlDocumentPtr    ToDocument()    const; ///< Cast to a more defined type. Will return null if not of the requested type.
	virtual XmlElementPtr     ToElement()     const; ///< Cast to a more defined type. Will return null if not of the requested type.
	virtual XmlCommentPtr     ToComment()     const; ///< Cast to a more defined type. Will return null if not of the requested type.
	virtual XmlUnknownPtr     ToUnknown()     const; ///< Cast to a more defined type. Will return null if not of the requested type.
	virtual XmlTextPtr        ToText()        const; ///< Cast to a more defined type. Will return null if not of the requested type.
	virtual XmlDeclarationPtr ToDeclaration() const; ///< Cast to a more defined type. Will return null if not of the requested type.

	inline bool IsDocument()    const { return m_type == DOCUMENT; }
	inline bool IsElement()     const { return m_type == ELEMENT; }
	inline bool IsComment()     const { return m_type == COMMENT; }
	inline bool IsUnknown()     const { return m_type == UNKNOWN; }
	inline bool IsText()        const { return m_type == TEXT; }
	inline bool IsDeclaration() const { return m_type == DECLARATION; }
	inline bool IsAttribute() const { return m_type == ATTRIBUTE; }

	inline StringPtr OuterXml() const { return ToString(); }
	virtual StringPtr InnerXml() const;

	virtual StringPtr ToString() const = 0;
	virtual void WriteTo(TextWriter& writer) const = 0;
	
	inline Iterator Begin()
	{
		return Iterator(m_self);
	}

#ifdef DEBUG
	virtual void ValidateMem() const;
	virtual void CheckMem() const;
#endif
};

REGISTER_TYPEOF( 520, XmlNode );

extern const char* _xmlErrorStrings[ XmlNode::TIXML_ERROR_STRING_COUNT ];	

/** @} */
}
#endif
