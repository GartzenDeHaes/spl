#ifndef _xpathoppredicate_h
#define _xpathoppredicate_h

#include <spl/Debug.h>
#include <spl/RefCountPtrCast.h>
#include <spl/Variant.h>
#include <spl/xml/xpath/private/XPathOperator.h>

namespace spl
{
class XPathOpPredicate;
typedef RefCountPtrCast<XPathOpPredicate, XPathOperator, XPathOperatorPtr> XPathOpPredicatePtr;

/** @brief Selects the root node.
 */
class XPathOpPredicate : public XPathOperator
{
public:
	typedef enum _Operator
	{
		OP_ERROR = 0,
		OP_EQ = 19,
		OP_NEQ = 20,
		OP_LT = 21,
		OP_LTEQ = 22,
		OP_GT = 23,
		OP_GTEQ = 24
	} Operator;

private:
	Vector<XPathOperatorPtr> m_ops;
	Operator m_binop;
	VariantPtr m_arg;
	
public:	
	XPathOpPredicate();
	XPathOpPredicate(const XPathOpPredicate& op);
	virtual ~XPathOpPredicate();
	
	virtual XPathOpPredicate& operator =(const XPathOpPredicate& oproot);
	
	virtual bool IsMatch(XmlNode& context);
	virtual XmlNodeListPtr NextContext(XmlNodePtr context);
	
	inline Vector<XPathOperatorPtr>& Operators() { return m_ops; }
	inline Operator& BinOp() { return m_binop; }
	inline VariantPtr Arg() { return m_arg; }
	
#if defined(DEBUG) || defined(_DEBUG)
	void CheckMem() const;
	void ValidateMem() const;
#endif
};
}

#endif
