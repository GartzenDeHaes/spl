#ifndef _xmliterator_h
#define _xmliterator_h

#include <spl/IIterator.h>
#include <spl/xml/XmlDocument.h>
#include <spl/collection/Vector.h>

namespace spl
{
	class XmlIterator : public IIterator<XmlNodePtr>
	{
	private:
		XmlDocumentPtr m_doc;
		Vector<XmlNodePtr> m_stack;

	public:
		XmlIterator(XmlDocumentPtr doc);

		virtual bool Next();
		virtual bool Prev();
		virtual XmlNodePtr Current();
		virtual XmlNodePtr& CurrentRef();
	};
}

#endif
