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
#include <spl/Int32.h>
#include <spl/xml/xpath/private/XPathOpAttrib.h>
#include <spl/xml/xpath/private/XPathOpChildern.h>
#include <spl/xml/xpath/private/XPathOpChildTree.h>
#include <spl/xml/xpath/private/XPathOpError.h>
#include <spl/xml/xpath/private/XPathOpNamedNode.h>
#include <spl/xml/xpath/private/XPathOpRoot.h>
#include <spl/xml/xpath/private/XPathParser.h>
#include <spl/xml/xpath/private/XPathOpPredicate.h>

using namespace spl;

XPathParser::XPathParser()
: m_lex(), m_exprStack(), m_out(NULL)
{
}

XPathParser::XPathParser(const XPathParser& parser)
: m_lex(parser.m_lex), m_out(NULL), m_exprStack(parser.m_exprStack)
{
}

XPathParser::~XPathParser()
{
}

XPathParser& XPathParser::operator =(const XPathParser& parser)
{
	m_lex = parser.m_lex;
	m_exprStack = parser.m_exprStack;
	return *this;
}

Array<XPathOperatorPtr> XPathParser::Parse(const String& expr)
{
	Vector<XPathOperatorPtr> ops;
	m_out = &ops;
	
	m_lex.Tokenize(expr);
	Expr();
	if (m_lex.HasMoreTokens())
	{
		throw new XmlException("Parsing stopped at char position " + *Int32::ToString(m_lex.CurrentLexumInputCharPos()), 1, m_lex.CurrentLexumInputCharPos());
	}
	return Array<XPathOperatorPtr>(*ops.ToArray());
}

#if defined(DEBUG) || defined(_DEBUG)
void XPathParser::CheckMem() const
{
	m_lex.CheckMem();
	m_exprStack.CheckMem();
}

void XPathParser::ValidateMem() const
{
	m_lex.ValidateMem();
	m_exprStack.ValidateMem();
}
#endif

void XPathParser::NodeSpec()
{
	if (m_lex.CurrentToken() == XPathLex::XP_LITERAL)
	{
		String nodename = m_lex.CurrentLexum();

		if (m_lex.HasMoreTokens() && m_lex.GetRelativeToken(1) == XPathLex::XP_COLON)
		{
			// namespace
			m_lex.Match(XPathLex::XP_LITERAL);
			if (m_lex.Match(XPathLex::XP_COLON) != XPathLex::XP_LITERAL)
			{
				throw new XmlException("Expected node name after namespace " + nodename, 1, m_lex.CurrentLexumInputCharPos());
			}
			nodename = nodename + ":" + m_lex.CurrentLexum();
		}
		else if (m_lex.HasMoreTokens() && m_lex.GetRelativeToken(1) == XPathLex::XP_LPAR)
		{
			// function call
			String functionName = m_lex.CurrentLexum();

			m_lex.Match(XPathLex::XP_LITERAL);
			m_lex.Match(XPathLex::XP_LPAR);
			ArgList();
			m_lex.Match(XPathLex::XP_RPAR);
			return;
		}

		m_out->Add(XPathOpNamedNodePtr(new XPathOpNamedNode(nodename)));
		m_lex.Match(XPathLex::XP_LITERAL);
		return;
	}
	
	if (m_lex.CurrentToken() == XPathLex::XP_DOT)
	{
		// match current node
		m_lex.Match(XPathLex::XP_DOT);
		return;
	}

	if (m_lex.CurrentToken() == XPathLex::XP_STAR)
	{
		// everything, but no operation needed.
		m_lex.Match(XPathLex::XP_STAR);
		return;
	}

	if (m_lex.CurrentToken() == XPathLex::XP_DOTDOT)
	{
		// match parent node
		m_lex.Match(XPathLex::XP_DOTDOT);
		return;
	}

	if (m_lex.CurrentToken() == XPathLex::XP_AT)
	{
		m_lex.Match(XPathLex::XP_AT);
		m_out->Add(XPathOpAttribPtr(new XPathOpAttrib()));		
		return;
	}

	throw new XmlException("Syntax error, expected node specifier.", 1, m_lex.CurrentLexumInputCharPos());
}

void XPathParser::NodePath()
{
	if (m_lex.CurrentToken() == XPathLex::XP_SLASHSLASH)
	{
		// select all
		m_lex.Match(XPathLex::XP_SLASHSLASH);
		m_out->Add(XPathOpChildTreePtr(new XPathOpChildTree()));
	}
	else if (m_lex.CurrentToken() == XPathLex::XP_SLASH)
	{
		// select root
		m_lex.Match(XPathLex::XP_SLASH);
		m_out->Add(XPathOpRootPtr(new XPathOpRoot()));
		if (m_lex.HasMoreTokens())
		{
			// select root's childern
			m_out->Add(XPathOpChildernPtr(new XPathOpChildern()));
		}
	}
	else if (m_lex.CurrentToken() == XPathLex::XP_LITERAL)
	{
		// select root's childern
		m_out->Add(XPathOpChildernPtr(new XPathOpChildern()));
	}
	else if (m_lex.CurrentToken() == XPathLex::XP_AT)
	{
		m_lex.Match(XPathLex::XP_AT);
		m_out->Add(XPathOpAttribPtr(new XPathOpAttrib()));
	}
	MorePath();
}

void XPathParser::MorePath()
{
	if (!m_lex.HasMoreTokens())
	{
		return;
	}
	if (m_lex.CurrentToken() == XPathLex::XP_LITERAL && m_lex.GetRelativeToken(1) == XPathLex::XP_LPAR)
	{
		// function call
		String functionName = m_lex.CurrentLexum();
		m_lex.Match(XPathLex::XP_LITERAL);
		m_lex.Match(XPathLex::XP_LPAR);
		ArgList();
		m_lex.Match(XPathLex::XP_RPAR);
	}
	else
	{
		Axis();
		NodeSpec();
		Predicate();
	}
	MoreMorePath();
}

bool XPathParser::Axis()
{
	if (m_lex.CurrentToken() != XPathLex::XP_LITERAL)
	{
		return false;
	}
	if (m_lex.GetRelativeToken(1) != XPathLex::XP_COLONCOLON)
	{
		return false;
	}

	m_out->Add(XPathOpErrorPtr(new XPathOpError("axis operator not supported")));

	String axis = m_lex.CurrentLexum();
	m_lex.Match(XPathLex::XP_LITERAL);
	m_lex.Match(XPathLex::XP_COLONCOLON);
	
	return true;
}

bool XPathParser::Predicate()
{
	if (m_lex.CurrentToken() != XPathLex::XP_LBRAC)
	{
		return false;
	}
	m_lex.Match(XPathLex::XP_LBRAC);
	
	Vector<XPathOperatorPtr> *outbak = m_out;
	XPathOpPredicatePtr oppred(new XPathOpPredicate());
	m_out = &oppred->Operators();
	
	Expr();
	
	if (m_exprStack.Count() != 2)
	{
		outbak->Add(XPathOpErrorPtr(new XPathOpError("Unsupported predicated expression.")));
	}
	else
	{	
		*oppred->Arg() = m_exprStack.Peek();
		String op = m_exprStack.Tail();
		
		if (op.Equals('='))
		{
			oppred->BinOp() = XPathOpPredicate::OP_EQ;
		}
		else if (op.Equals('<'))
		{
			oppred->BinOp() = XPathOpPredicate::OP_LT;
		}
		else if (op.Equals('>'))
		{
			oppred->BinOp() = XPathOpPredicate::OP_GT;
		}
		else if (op.Equals("<="))
		{
			oppred->BinOp() = XPathOpPredicate::OP_LTEQ;
		}
		else if (op.Equals(">="))
		{
			oppred->BinOp() = XPathOpPredicate::OP_GTEQ;
		}
		else if (op.Equals("!="))
		{
			oppred->BinOp() = XPathOpPredicate::OP_NEQ;
		}
		else
		{
			throw new SyntaxException("Unsupported predicated operator " + op);		
		}
	}
	
	m_exprStack.Clear();
	m_out = outbak;
	m_out->Add(oppred);
	
	m_lex.Match(XPathLex::XP_RBRAC);

	return true;
}

bool XPathParser::MoreMorePath()
{
	if (m_lex.CurrentToken() == XPathLex::XP_SLASH)
	{
		m_lex.Match(XPathLex::XP_SLASH);
		m_out->Add(XPathOpChildernPtr(new XPathOpChildern()));
		
		if (! m_lex.HasMoreTokens())
		{
			throw new SyntaxException("xpath cannot end with a slash");
		}
	}
	else if (m_lex.CurrentToken() == XPathLex::XP_SLASHSLASH)
	{
		m_lex.Match(XPathLex::XP_SLASHSLASH);
		m_out->Add(XPathOpChildTreePtr(new XPathOpChildTree()));
		
		if (! m_lex.HasMoreTokens())
		{
			throw new SyntaxException("xpath cannot end with a slash slash");
		}
	}
	else if (m_lex.CurrentToken() == XPathLex::XP_AT)
	{
		m_lex.Match(XPathLex::XP_AT);
		m_out->Add(XPathOpAttribPtr(new XPathOpAttrib()));		
	}
	else if (m_lex.CurrentToken() == XPathLex::XP_STAR)
	{
		// everything, do nothing
		m_lex.Match(XPathLex::XP_STAR);
	}
	else
	{
		return false;
	}
	
	MorePath();

	return true;
}

bool XPathParser::Expr()
{
	if (!m_lex.HasMoreTokens())
	{
		return false;
	}

	if (m_lex.CurrentToken() == XPathLex::XP_LPAR)
	{
		return false;
	}

	LogOp();

	if (MoreLogOp())
	{
		MoreExpr();
	}

	return true;
}

bool XPathParser::MoreExpr()
{
	if (m_lex.CurrentToken() == XPathLex::XP_PIPE)
	{
		m_lex.Match(XPathLex::XP_PIPE);
		Expr();
		return true;
	}

	return false;
}

void XPathParser::LogOp()
{
	RelOp();
	MoreRelOps();
}

bool XPathParser::MoreLogOp()
{
	switch(m_lex.CurrentToken())
	{
	case XPathLex::XP_AND:
		m_lex.Match(XPathLex::XP_AND);
		break;
	case XPathLex::XP_OR:
		m_lex.Match(XPathLex::XP_OR);
		break;
	case XPathLex::XP_NEQ:
		m_exprStack.Add(m_lex.CurrentLexum());
		m_lex.Match(XPathLex::XP_NEQ);
		break;
	case XPathLex::XP_EQ:
		m_exprStack.Add(m_lex.CurrentLexum());
		m_lex.Match(XPathLex::XP_EQ);
		break;
	default:
		return false;
	}

	LogOp();
	MoreLogOp();

	return true;
}

bool XPathParser::MoreRelOps()
{
	switch(m_lex.CurrentToken())
	{
	case XPathLex::XP_LT:
		m_exprStack.Add(m_lex.CurrentLexum());
		m_lex.Match(XPathLex::XP_LT);
		break;
	case XPathLex::XP_GT:
		m_exprStack.Add(m_lex.CurrentLexum());
		m_lex.Match(XPathLex::XP_GT);
		break;
	case XPathLex::XP_LTEQ:
		m_exprStack.Add(m_lex.CurrentLexum());
		m_lex.Match(XPathLex::XP_LTEQ);
		break;
	case XPathLex::XP_GTEQ:
		m_exprStack.Add(m_lex.CurrentLexum());
		m_lex.Match(XPathLex::XP_GTEQ);
		break;
	default:
		return false;
	}

	RelOp();
	MoreRelOps();

	return true;
}

void XPathParser::RelOp()
{
	Term();
	MoreTerms();
}

void XPathParser::Term()
{
	Factor();
	MoreFactors();
}

bool XPathParser::MoreTerms()
{
	if (m_lex.CurrentToken() == XPathLex::XP_PLUS)
	{
		m_lex.Match(XPathLex::XP_PLUS);
	}
	else if (m_lex.CurrentToken() == XPathLex::XP_MIN)
	{
		m_lex.Match(XPathLex::XP_MIN);
	}
	else
	{
		return false;
	}

	Term();
	MoreTerms();
	return true;
}

void XPathParser::Factor()
{
	PathExpr();
	MoreFactors();
}

bool XPathParser::MoreFactors()
{
	if (!m_lex.HasMoreTokens())
	{
		return false;
	}

	switch(m_lex.CurrentToken())
	{
	case XPathLex::XP_STAR:
		m_lex.Match(XPathLex::XP_STAR);
		break;
	case XPathLex::XP_SLASH:
		m_lex.Match(XPathLex::XP_SLASH);
		break;
	case XPathLex::XP_MOD:
		m_lex.Match(XPathLex::XP_MOD);
		break;
	case XPathLex::XP_DIV:
		m_lex.Match(XPathLex::XP_DIV);
		break;
	default:
		return false;
	}

	Factor();
	MoreFactors();

	return true;
}

bool XPathParser::PathExpr()
{
	switch(m_lex.CurrentToken())
	{
	case XPathLex::XP_DOLLAR:
		m_lex.Match(XPathLex::XP_DOLLAR);
		m_lex.Match(XPathLex::XP_LITERAL);
		break;
	case XPathLex::XP_LPAR:
		m_lex.Match(XPathLex::XP_LPAR);
		Expr();
		m_lex.Match(XPathLex::XP_RPAR);
		break;
	case XPathLex::XP_INT:
		m_exprStack.Add(m_lex.CurrentLexum());
		m_lex.Match(XPathLex::XP_INT);
		break;
	case XPathLex::XP_FLOAT:
		m_exprStack.Add(m_lex.CurrentLexum());
		m_lex.Match(XPathLex::XP_FLOAT);
		break;
	case XPathLex::XP_STRING:
		m_exprStack.Add(m_lex.CurrentLexum());
		m_lex.Match(XPathLex::XP_STRING);
		break;
	case XPathLex::XP_SLASHSLASH:
	case XPathLex::XP_SLASH:
	case XPathLex::XP_LITERAL:
	case XPathLex::XP_LBRAC:
	case XPathLex::XP_DOT:
	case XPathLex::XP_DOTDOT:
	case XPathLex::XP_STAR:
	case XPathLex::XP_AT:
		NodePath();
		break;
	default:
		return false;
	}

	return true;
}

bool XPathParser::ArgList()
{
	if (m_lex.CurrentToken() == XPathLex::XP_RPAR)
	{
		return false;
	}

	Expr();
	MoreArgs();

	return true;
}

bool XPathParser::MoreArgs()
{
	if (m_lex.CurrentToken() == XPathLex::XP_COMMA)
	{
		return false;
	}

	while (m_lex.CurrentToken() == XPathLex::XP_COMMA)
	{
		m_lex.Match(XPathLex::XP_COMMA);
		Expr();
	}

	return true;
}
