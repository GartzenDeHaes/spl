#include <spl/xml/xpath/private/XPathOpChildern.h>

using namespace spl;

XPathOpChildern::XPathOpChildern(const String& name)
: m_name(name)
{
}

XPathOpChildern::XPathOpChildern()
: m_name()
{
}

XPathOpChildern::XPathOpChildern(const XPathOpChildern& op)
: XPathOperator(op), m_name(op.m_name)
{
}

XPathOpChildern::~XPathOpChildern()
{
}

XPathOpChildern& XPathOpChildern::operator =(const XPathOpChildern& oproot)
{
	m_name = oproot.m_name;
	return *this;
}

bool XPathOpChildern::IsMatch(XmlNode& context)
{
	return context.HasChildNodes();
}

XmlNodeListPtr XPathOpChildern::NextContext(XmlNodePtr context)
{
	ASSERT(IsMatch(context));
	XmlNodeListPtr childs(context->ChildNodes());

	if (m_name.Length() == 0)
	{
		return childs;
	}

	XmlNodeListPtr nodes = XmlNodeListPtr(new XmlNodeList());

	for ( int x = 0; x < childs->Count(); x++ )
	{
		if (childs->Item(x)->Name() == m_name)
		{
			nodes->Add(childs->Item(x));
		}
	}

	return nodes;
}

#if defined(DEBUG) || defined(_DEBUG)
void XPathOpChildern::CheckMem() const
{
	XPathOperator::CheckMem();
}

void XPathOpChildern::ValidateMem() const
{
	XPathOperator::ValidateMem();
}
#endif
