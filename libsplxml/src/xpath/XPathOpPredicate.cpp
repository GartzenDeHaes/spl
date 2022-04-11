#include <spl/xml/xpath/private/XPathOpPredicate.h>
#include <spl/xml/xpath/XPath.h>

using namespace spl;

XPathOpPredicate::XPathOpPredicate()
: m_ops(), m_binop(OP_ERROR), m_arg(new Variant())
{
}

XPathOpPredicate::XPathOpPredicate(const XPathOpPredicate& op)
: XPathOperator(op), m_ops(op.m_ops), m_binop(OP_ERROR), m_arg(new Variant())
{
}

XPathOpPredicate::~XPathOpPredicate()
{
}

XPathOpPredicate& XPathOpPredicate::operator =(const XPathOpPredicate& oproot)
{
	m_ops = oproot.m_ops;
	m_binop = oproot.m_binop;
	m_arg = oproot.m_arg;
	return *this;
}

bool XPathOpPredicate::IsMatch(XmlNode& context)
{
	return true;
}

XmlNodeListPtr XPathOpPredicate::NextContext(XmlNodePtr context)
{
	RefCountPtr<Array<XPathOperatorPtr> > ops(m_ops.ToArray());
	XmlNodeListPtr innerCtx = XPath::SelectNodes(ops, context);

	XmlNodeListPtr matchNodes = XmlNodeListPtr(new XmlNodeList());

	for ( int x = 0; x < innerCtx->Count(); x++ )
	{
		XmlNodePtr node = innerCtx->Item(x);

		switch( m_binop )
		{
		case OP_EQ:
			if (Variant(*node->Value()) == *m_arg)
			{
				matchNodes->Add(context);
				return matchNodes;
			}
			break;
		case OP_NEQ:
			if (Variant(*node->Value()) != *m_arg)
			{
				matchNodes->Add(context);
				return matchNodes;
			}
			break;
		case OP_LT:
			if (Variant(*node->Value()) < *m_arg)
			{
				matchNodes->Add(context);
				return matchNodes;
			}
			break;
		case OP_LTEQ:
			if (Variant(*node->Value()) <= *m_arg)
			{
				matchNodes->Add(context);
				return matchNodes;
			}
			break;
		case OP_GT:
			if (Variant(*node->Value()) > *m_arg)
			{
				matchNodes->Add(context);
				return matchNodes;
			}
			break;
		case OP_GTEQ:
			if (Variant(*node->Value()) >= *m_arg)
			{
				matchNodes->Add(context);
				return matchNodes;
			}
			break;
		default:
			throw new SyntaxException("Internal predicate operator error");
		}
	}

	return matchNodes;
}

#if defined(DEBUG) || defined(_DEBUG)
void XPathOpPredicate::CheckMem() const
{
	XPathOperator::CheckMem();
	m_ops.CheckMem();
}

void XPathOpPredicate::ValidateMem() const
{
	XPathOperator::ValidateMem();
	m_ops.ValidateMem();
}
#endif
