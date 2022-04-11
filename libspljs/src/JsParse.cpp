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
#include <spl/interp/JsParse.h>

using namespace spl;

JsParse::JsParse(bool isDebugBuild)
: m_isDebugBuild(isDebugBuild), m_lex(), m_prog(new Program()), m_locals(), m_fnNameStk(3), m_lastLineNo(1)
{
}

JsParse::~JsParse()
{
}

#if defined(DEBUG)
void JsParse::CheckMem() const
{
	m_lex.CheckMem();
	m_prog.CheckMem();
	m_locals.CheckMem();
	m_fnNameStk.CheckMem();
}

void JsParse::ValidateMem() const
{
	m_lex.ValidateMem();
	m_prog.ValidateMem();
	m_locals.ValidateMem();
	m_fnNameStk.ValidateMem();
}
#endif

JsLex::Token JsParse::Match(JsLex::Token tok)
{
	JsLex::Token nt = m_lex.Match(tok);
	if (m_isDebugBuild)
	{
		if (m_lastLineNo != m_lex.LineNumber())
		{
			m_prog->AppendCode(JSOP_LINE, JSOPARG_IMM, m_lex.LineNumber());
			m_lastLineNo = m_lex.LineNumber();
		}
	}
	
	return nt;
}

ProgramPtr JsParse::Parse(const String& src)
{
	m_prog = ProgramPtr(new Program());
	m_lex.Init(src);
	m_lex.Next();
	
	m_locals.Add( RefCountPtr<Hashtable<String, int> >(new Hashtable<String, int>()) );

	Stmts();
	
	m_prog->ParseComplete();
	return m_prog;
}

/**
file		: stmts
			|
			;
*/

/**
stmts		: stmt SEMI stmts
			| function stmts
			|
			;	
*/
void JsParse::Stmts()
{
	while
	(
		JsLex::T_RBRACE != m_lex.CurrentToken() && 
		JsLex::T_CASE != m_lex.CurrentToken() && 
		JsLex::T_DEFAULT != m_lex.CurrentToken() &&
		JsLex::T_JEOF != m_lex.CurrentToken()
	)
	{
		if (JsLex::T_FUNCTION == m_lex.CurrentToken())
		{
			Function();
		}
		else
		{
			Stmt();
		}
	}
}

/**
stmt		: VAR ID morevar
			| IF moreif
			| WHILE morewhile
			| FOR morefor
			| SWITCH moreswitch
			| RETURN morereturn
			| TRY moretry
			| BREAK
			| expr
			;
*/
void JsParse::Stmt()
{
	switch(m_lex.CurrentToken())
	{
		case JsLex::T_VAR:
			Match(JsLex::T_VAR);
			MoreVar();
			Match(JsLex::T_SEMI);
			break;
		case JsLex::T_IF:
			Match(JsLex::T_IF);
			MoreIf();
			break;
		case JsLex::T_WHILE:
			Match(JsLex::T_WHILE);
			MoreWhile();
			break;
		case JsLex::T_FOR:
			Match(JsLex::T_FOR);
			MoreFor();
			break;
		case JsLex::T_SWITCH:
			Match(JsLex::T_SWITCH);
			MoreSwitch();
			break;
		case JsLex::T_RETURN:
			if (m_locals.Count() == 0)
			{
				throw new ParseException("return is not allowed outside of a function.");
			}
			Match(JsLex::T_RETURN);
			MoreReturn();
			m_prog->AppendCode(JSOP_RET);
			break;
		case JsLex::T_TRY:
			Match(JsLex::T_TRY);
			break;
		case JsLex::T_BREAK:
			Match(JsLex::T_BREAK);
			m_prog->AppendCode(JSOP_BREAK);
			break;
		default:
			Expr();
			Match(JsLex::T_SEMI);
			break;
	}
}

/**
morevar		: ID ASSIGN expr COMMA morevar
			| ID ASSIGN expr
			;
 */
void JsParse::MoreVar()
{
	//m_prog->AppendCode(JSOP_DEFPROP, JSOPARG_TAB, m_prog->AddToTable(m_lex.Lexum().ToString()));
	m_locals.PeekRef()->Set(*m_lex.Lexum().ToString(), m_locals.PeekRef()->Count());
	m_prog->AppendCode(JSOP_PUSH, JSOPARG_IMM, 0);
	Match(JsLex::T_ID);
	
	if (m_lex.CurrentToken() == JsLex::T_ASSIGN)
	{
		m_prog->AppendCode(JSOP_DUP);
		Match(JsLex::T_ASSIGN);
		Expr();
		m_prog->AppendCode(JSOP_ASSIGN);
	}
	if (m_lex.CurrentToken() == JsLex::T_COMMA)
	{
		Match(JsLex::T_COMMA);
		MoreVar();
	}
}

/**
moreif		: LPAR expr RPAR stmtblock moremoreif
			;
*/
void JsParse::MoreIf()
{
	Match(JsLex::T_LPAR);
	Expr();
	Match(JsLex::T_RPAR);	
	
	int nextTo = m_prog->AppendCode(JSOP_JMPZ, JSOPARG_IMM, 0);
	StmtBlock();
	int exitTo = m_prog->AppendCode(JSOP_JMP, JSOPARG_IMM, 0);
	
	m_prog->FixupCode(nextTo);
	MoreMoreIf();
	
	m_prog->FixupCode(exitTo);
}

/**
moremoreif	: ELSE moreif
			| ELSE stmtblock
			|
			;
*/
void JsParse::MoreMoreIf()
{
	if (m_lex.CurrentToken() == JsLex::T_ELSE)
	{
		Match(JsLex::T_ELSE);
		if (m_lex.CurrentToken() == JsLex::T_IF)
		{
			Match(JsLex::T_IF);
			MoreIf();
		}
		else
		{
			StmtBlock();
		}
	}
}

/**
morewhile	: LPAR expr RPAR stmtblock
			;
*/
void JsParse::MoreWhile()
{
	int top = m_prog->Position();
	
	Match(JsLex::T_LPAR);
	Expr();
	Match(JsLex::T_RPAR);
	
	int exitTo = m_prog->AppendCode(JSOP_JMPZ, JSOPARG_IMM, 0);
	
	int enterPos = StmtBlock();
	
	m_prog->AppendCode(JSOP_JMP, JSOPARG_IMM, (top - m_prog->Position()) - 1);
	
	m_prog->FixupCode(exitTo, m_prog->Position() - exitTo);

	// Set break target
	m_prog->FixupCode(enterPos);
}

/**
morefor		: LPAR expr SEMI expr SEMI expr RPAR stmtblock
			;
*/
void JsParse::MoreFor()
{
	Match(JsLex::T_LPAR);
	Stmt();

	int topPos = m_prog->Position();

	Expr();
	Match(JsLex::T_SEMI);
	int jmpExitPos = m_prog->AppendCode(JSOP_JMPZ, JSOPARG_IMM, 0);
	int jmpOverPostPos = m_prog->AppendCode(JSOP_JMP, JSOPARG_IMM, 0);
	
	Expr();
	Match(JsLex::T_RPAR);
	m_prog->AppendCode(JSOP_POP);
	m_prog->AppendCode(JSOP_JMP, JSOPARG_IMM, topPos - m_prog->Position() - 1);

	m_prog->FixupCode(jmpOverPostPos);

	int enterPos = StmtBlock();
	
	m_prog->AppendCode(JSOP_JMP, JSOPARG_IMM, jmpOverPostPos - m_prog->Position() - 1);
	m_prog->FixupCode(jmpExitPos);
	
	// Set break target
	m_prog->FixupCode(enterPos);
}

/**
morereturn	: expr
			|
			;
*/
void JsParse::MoreReturn()
{
	if (m_lex.CurrentToken() == JsLex::T_SEMI)
	{
		m_prog->AppendCode(JSOP_PUSH, JSOPARG_IMM, m_prog->AddToTable(VariantPtr(new Variant())));
	}
	else
	{
		Expr();
	}
	Match(JsLex::T_SEMI);
}

/**
moretry		: stmtblock CATCH LPAR ID RPAR stmtblock [FINALLY stmtblock]-
			;
*/
void JsParse::MoreTry()
{
	StmtBlock();
	Match(JsLex::T_CATCH);
	Match(JsLex::T_LPAR);
	Match(JsLex::T_ID);
	Match(JsLex::T_RPAR);
	StmtBlock();
	if (m_lex.CurrentToken() == JsLex::T_FINALLY)
	{
		Match(JsLex::T_FINALLY);
		StmtBlock();
	}
}

/**
moreswitch	: LPAR expr RPAR LBRACE switchblock RBRACE
			;
*/
void JsParse::MoreSwitch()
{
	int enterPos = m_prog->AppendCode(JSOP_ENTER);
	
	JsObject *disp = new JsObject();
	int dispTabPos = m_prog->AddToTable(VariantPtr(new Variant(RefCountPtr<IJsObject>(disp))));
	m_prog->AppendCode(JSOP_PUSH, JSOPARG_TAB, dispTabPos);
	
	Match(JsLex::T_LPAR);
	Expr();
	Match(JsLex::T_RPAR);
	Match(JsLex::T_LBRACE);
	
	m_prog->AppendCode(JSOP_SWITCH);
	m_prog->AppendCode(JSOP_BREAK);
	
	SwitchBlock(m_prog->Position() - 1, disp);
	Match(JsLex::T_RBRACE);
	
	m_prog->AppendCode(JSOP_LEAVE);
	m_prog->FixupCode(enterPos);
}

/**
switchblock	: CASE literal COLON stmts switchblock
			| DEFAULT COLON stmts switchblock
			|
			;
*/
void JsParse::SwitchBlock(int pc, JsObject *disp)
{
	while(true)
	{
		if (m_lex.CurrentToken() == JsLex::T_CASE)
		{
			Match(JsLex::T_CASE);
	
			switch(m_lex.CurrentToken())
			{
				case JsLex::T_INT:
				case JsLex::T_STRING:
				case JsLex::T_REAL:
					disp->SetProperty((m_lex.Lexum().ToString()), VariantPtr(new Variant(m_prog->Position() - pc)));
					break;
				case JsLex::T_JNULL:
					disp->SetProperty("0", VariantPtr(new Variant(m_prog->Position() - pc)));
					break;
				default:
					throw new ParseException("Expected LITERAL, found " + m_lex.Lexum().ToString() + " on line " + Int32::ToString(m_lex.LineNumber()));
			}
			Match(m_lex.CurrentToken());
			Match(JsLex::T_COLON);
			Stmts();
		}
		else if (m_lex.CurrentToken() == JsLex::T_DEFAULT)
		{
			Match(JsLex::T_DEFAULT);
			Match(JsLex::T_COLON);
			
			disp->SetProperty("_____default_____", VariantPtr(new Variant(m_prog->Position() - pc)));
			Stmts();
		}
		else
		{
			return;
		}
	}
}

/**
function	: FUNTION ID LPAR paramlist RPAR [COLON ID [LPAR arglist RPAR]-]- stmtblock
			;
*/
void JsParse::Function()
{
	Match(JsLex::T_FUNCTION);
	int fnNamePos = m_prog->AddToTable(m_lex.Lexum().ToString());
	m_prog->AppendCode(JSOP_PUSH, JSOPARG_TAB, fnNamePos);
	m_fnNameStk.Add(m_lex.Lexum().GetChars());
	Match(JsLex::T_ID);
	
	m_locals.Add( RefCountPtr<Hashtable<String, int> >(new Hashtable<String, int>()) );

	Match(JsLex::T_LPAR);
	ParamList();
	Match(JsLex::T_RPAR);
	
	m_prog->AppendCode(JSOP_PUSH, JSOPARG_IMM, m_locals.PeekRef()->Count());
	m_prog->AppendCode(JSOP_PUSH, JSOPARG_TAB, m_prog->AddToTable("function " + *m_prog->GetTable().ElementAtRef(fnNamePos)->ToString()));
	int deffuncPos = m_prog->AppendCode(JSOP_DEFFUNC, JSOPARG_IMM, 0);

	if (JsLex::T_COLON == m_lex.CurrentToken())
	{
		Match(JsLex::T_COLON);
		StringPtr super = m_lex.Lexum().ToString();
		Match(JsLex::T_ID);
		
		if (m_locals.Count() > 2)
		{
			throw new ParseException("Super class " + *super +  " is not allowed on inner functions.");
		}

		int superPos = m_prog->AddToTable("super");
		m_prog->AppendCode(JSOP_DEFPROP, JSOPARG_TAB, superPos);
		m_prog->AppendCode(JSOP_NEW, JSOPARG_TAB, m_prog->AddToTable(*super));
		m_prog->AppendCode(JSOP_ASSIGN);
		
		m_prog->AppendCode(JSOP_ENTER);
		m_prog->AppendCode(JSOP_FINDPROP, JSOPARG_TAB, superPos);
		if (JsLex::T_LPAR == m_lex.CurrentToken())
		{
			Match(JsLex::T_LPAR);
			ArgList();
			Match(JsLex::T_RPAR);
		}
		m_prog->AppendCode(JSOP_CALL);
	}

	StmtBlock();

	m_prog->AppendCode(JSOP_PUSH, JSOPARG_TAB, m_prog->AddToTable(VariantPtr(new Variant())));
	m_prog->AppendCode(JSOP_RET);
	m_prog->AppendCode(JSOP_ENDFN);
	m_prog->FixupCode(deffuncPos);
	
	if (m_locals.Count() > 2)
	{
		//int superPos = m_prog->AddToTable(JsObject::CONTAINER_PROPERKTY_NAME);
		
		m_prog->AppendCode(JSOP_THIS);//, JSOPARG_TAB, m_prog->AddToTable(m_fnNameStk.ElementAtRef(m_fnNameStk.Count()-2)));
		m_prog->AppendCode(JSOP_FINDPROP, JSOPARG_TAB, fnNamePos);
		m_prog->AppendCode(JSOP_SETCTX);	
	}
	
	m_locals.Pop();
	m_fnNameStk.Pop();
}

/**
paramlist	: ID COMMA paramlist
			| ID
			|
			;
*/
void JsParse::ParamList()
{
	while (!m_lex.IsEOF() && (m_lex.CurrentToken() == JsLex::T_ID || m_lex.CurrentToken() == JsLex::T_VAR))
	{
		ASSERT(m_locals.Count() > 0);
		
		if (m_lex.CurrentToken() == JsLex::T_VAR)
		{
			Match(JsLex::T_VAR);
		}
		
		StringPtr id(m_lex.Lexum().ToString());
		
		if (m_locals.PeekRef()->ContainsKey(*id))
		{
			throw new ParseException(*id + " aready defined.");
		}
		
		m_locals.PeekRef()->Set(*id, m_locals.PeekRef()->Count());
		Match(JsLex::T_ID);
		
		if (m_lex.CurrentToken() != JsLex::T_COMMA)
		{
			break;
		}
		Match(JsLex::T_COMMA);
	}
}

/**
stmtblock	: LBRACE stmts RBRACE
			;
*/
int JsParse::StmtBlock()
{
	Match(JsLex::T_LBRACE);
	int pos = m_prog->AppendCode(JSOP_ENTER);
	Stmts();
	Match(JsLex::T_RBRACE);
	m_prog->AppendCode(JSOP_LEAVE);
	
	return pos;
}

/**
expr		: logop morelogops
			|
			;
*/
void JsParse::Expr()
{
	if (m_lex.CurrentToken() == JsLex::T_SEMI || m_lex.CurrentToken() == JsLex::T_LPAR)
	{
		return;
	}
	
	LogOp();
	MoreLogOps();
}

/**
logop		: relop morerelops
			;
*/
void JsParse::LogOp()
{
	RelOp();
	MoreRelOps();
}

/**
morelogops	: ASSIGN logop morelogops
			| PIPE logop morelogops
			| AMPR logop morelogops
			| AND logop morelogops
			| OR logop morelogops
			| ISNEQ logop morelogops
			| ISEQUAL logop morelogops
			|
			;
*/
void JsParse::MoreLogOps()
{
	switch(m_lex.CurrentToken())
	{
		case JsLex::T_ASSIGN:
			Match(JsLex::T_ASSIGN);
			LogOp();
			m_prog->AppendCode(JSOP_ASSIGN);
			MoreLogOps();
			break;
		case JsLex::T_PIPE:
			Match(JsLex::T_PIPE);
			LogOp();
			m_prog->AppendCode(JSOP_BINOR);
			MoreLogOps();
			break;
		case JsLex::T_AMPR:
			Match(JsLex::T_AMPR);
			LogOp();
			m_prog->AppendCode(JSOP_BINAND);
			MoreLogOps();
			break;
		case JsLex::T_AND:
			Match(JsLex::T_AND);
			LogOp();
			m_prog->AppendCode(JSOP_AND);
			MoreLogOps();
			break;
		case JsLex::T_OR:
			Match(JsLex::T_OR);
			LogOp();
			m_prog->AppendCode(JSOP_OR);
			MoreLogOps();
			break;
		case JsLex::T_ISNEQ:
			Match(JsLex::T_ISNEQ);
			LogOp();
			m_prog->AppendCode(JSOP_NEQ);
			MoreLogOps();
			break;
		case JsLex::T_ISEQUAL:
			Match(JsLex::T_ISEQUAL);
			LogOp();
			m_prog->AppendCode(JSOP_EQ);
			MoreLogOps();
			break;
	}
}

/**
relop		: term moreterms
			;
*/
void JsParse::RelOp()
{
	Term();
	MoreTerms();
}

/**
morerelops	: LT relop morerelops
			| GT relop morerelops
			| LTEQ relop morerelops
			| GTEQ relop morerelops
			|
			;
*/
void JsParse::MoreRelOps()
{
	switch(m_lex.CurrentToken())
	{
		case JsLex::T_LT:
			Match(JsLex::T_LT);
			RelOp();
			m_prog->AppendCode(JSOP_LT);
			MoreRelOps();
			break;
		case JsLex::T_GT:
			Match(JsLex::T_GT);
			RelOp();
			m_prog->AppendCode(JSOP_GT);
			MoreRelOps();
			break;
		case JsLex::T_LTEQ:
			Match(JsLex::T_LTEQ);
			RelOp();
			m_prog->AppendCode(JSOP_LTEQ);
			MoreRelOps();
			break;
		case JsLex::T_GTEQ:
			Match(JsLex::T_GTEQ);
			RelOp();
			m_prog->AppendCode(JSOP_GTEQ);
			MoreRelOps();
			break;
	}
}

/**
term		: factor morefactors
			;
*/
void JsParse::Term()
{
	Factor();
	MoreFactors();
}

/**
moreterms	: PLUS term moreterms
			| MINUS term moreterms
			| PLUSEQ term moreterms
			| MINUSEQ term moreterms
			| TIMESEQ term moreterms
			| DIVEQ term moreterms
			| MODEQ term moreterms
			| XOREQ term moreterms
			|
			;
*/
void JsParse::MoreTerms()
{
	switch(m_lex.CurrentToken())
	{
		case JsLex::T_PLUS:
			Match(JsLex::T_PLUS);
			Term();
			MoreTerms();
			m_prog->AppendCode(JSOP_ADD);
			break;
		case JsLex::T_MINUS:
			Match(JsLex::T_MINUS);
			Term();
			MoreTerms();
			m_prog->AppendCode(JSOP_SUB);
			break;
		case JsLex::T_PLUSEQ:
			Match(JsLex::T_PLUSEQ);
			m_prog->AppendCode(JSOP_DUP);
			Term();
			MoreTerms();
			m_prog->AppendCode(JSOP_ADD);
			m_prog->AppendCode(JSOP_ASSIGN);
			break;
		case JsLex::T_MINUSEQ:
			Match(JsLex::T_MINUSEQ);
			m_prog->AppendCode(JSOP_DUP);
			Term();
			MoreTerms();
			m_prog->AppendCode(JSOP_SUB);
			m_prog->AppendCode(JSOP_ASSIGN);
			break;
		case JsLex::T_TIMESEQ:
			Match(JsLex::T_TIMESEQ);
			m_prog->AppendCode(JSOP_DUP);
			Term();
			MoreTerms();
			m_prog->AppendCode(JSOP_MULT);
			m_prog->AppendCode(JSOP_ASSIGN);
			break;
		case JsLex::T_DIVEQ:
			Match(JsLex::T_DIVEQ);
			m_prog->AppendCode(JSOP_DUP);
			Term();
			MoreTerms();
			m_prog->AppendCode(JSOP_DIV);
			m_prog->AppendCode(JSOP_ASSIGN);
			break;
		case JsLex::T_MODEQ:
			Match(JsLex::T_MODEQ);
			m_prog->AppendCode(JSOP_DUP);
			Term();
			MoreTerms();
			m_prog->AppendCode(JSOP_MOD);
			m_prog->AppendCode(JSOP_ASSIGN);
			break;
		case JsLex::T_XOREQ:
			Match(JsLex::T_XOREQ);
			m_prog->AppendCode(JSOP_DUP);
			Term();
			MoreTerms();
			m_prog->AppendCode(JSOP_XOR);
			m_prog->AppendCode(JSOP_ASSIGN);
			break;
	}	
}

/**
factor		: MINUS single
			| PLUS single
			| INC single
			| DEC single
			| BANG single
			| single INC
			| single DEC
			| single
			;
*/
void JsParse::Factor()
{
	switch(m_lex.CurrentToken())
	{
		case JsLex::T_MINUS:
			Match(JsLex::T_MINUS);
			Single();
			m_prog->AppendCode(JSOP_NEGATE);
			break;
		case JsLex::T_PLUS:
			Match(JsLex::T_PLUS);
			Single();
			break;
		case JsLex::T_INC:
			Match(JsLex::T_INC);
			Single();
			m_prog->AppendCode(JSOP_INC);
			break;
		case JsLex::T_DEC:
			Match(JsLex::T_DEC);
			Single();
			m_prog->AppendCode(JSOP_DEC);
			break;
		default:
			Single();
			if (m_lex.CurrentToken() == JsLex::T_INC)
			{
				m_prog->AppendCode(JSOP_DUP);	/** a, a, ... **/
				m_prog->AppendCode(JSOP_RVAL);	/** a', a, ... **/
				m_prog->AppendCode(JSOP_ROT);	/** a, a', ... **/
				m_prog->AppendCode(JSOP_INC);	/** ++a, a', ... **/
				m_prog->AppendCode(JSOP_POP);	/** a', ... */
				Match(JsLex::T_INC);
			}
			else if (m_lex.CurrentToken() == JsLex::T_DEC)
			{
				m_prog->AppendCode(JSOP_DUP);	/** a, a, ... **/
				m_prog->AppendCode(JSOP_RVAL);	/** a', a, ... **/
				m_prog->AppendCode(JSOP_ROT);	/** a, a', ... **/
				m_prog->AppendCode(JSOP_DEC);	/** --a, a', ... **/
				m_prog->AppendCode(JSOP_POP);	/** a', ... */
				Match(JsLex::T_DEC);
			}
			break;
	}
}

/**
morefactors	: STAR factor morefactors
			| SLASH factor morefactors
			| MOD factor morefactors
			|
			;
*/
void JsParse::MoreFactors()
{
	switch(m_lex.CurrentToken())
	{
		case JsLex::T_STAR:
			Match(JsLex::T_STAR);
			Factor();
			MoreFactors();
			m_prog->AppendCode(JSOP_MULT);
			break;
		case JsLex::T_SLASH:
			Match(JsLex::T_SLASH);
			Factor();
			MoreFactors();
			m_prog->AppendCode(JSOP_DIV);
			break;
		case JsLex::T_MOD:
			Match(JsLex::T_MOD);
			Factor();
			MoreFactors();
			m_prog->AppendCode(JSOP_MOD);
			break;
	}
}

/**
single		: ID moresingle
			| THIS moresingle
			| LPAR expr RPAR moresingles
			| NEW ID
			| literal
			;
*/
void JsParse::Single()
{
	if (m_lex.CurrentToken() == JsLex::T_ID)
	{
		StringPtr id(m_lex.Lexum().ToString());
		Match(JsLex::T_ID);
		
		if (m_lex.CurrentToken() == JsLex::T_LPAR)
		{
			m_prog->AppendCode(JSOP_ENTER);
			m_prog->AppendCode(JSOP_FINDPROP, JSOPARG_TAB, m_prog->AddToTable(*id));
		}
		else
		{
			if (m_locals.Count() > 0 && m_locals.PeekRef()->ContainsKey(*id))
			{
				m_prog->AppendCode(JSOP_PUSH, JSOPARG_STK, m_locals.PeekRef()->Get(*id));
			}
			else
			{
				m_prog->AppendCode(JSOP_FINDPROP, JSOPARG_TAB, m_prog->AddToTable(*id));
			}
		}
		MoreSingles();
	}
	else if (m_lex.CurrentToken() == JsLex::T_THIS)
	{
		m_prog->AppendCode(JSOP_THIS);
		Match(JsLex::T_THIS);
		MoreSingles();		
	}
	else if (m_lex.CurrentToken() == JsLex::T_NEW)
	{
		Match(JsLex::T_NEW);
		m_prog->AppendCode(JSOP_NEW, JSOPARG_TAB, m_prog->AddToTable(m_lex.Lexum().ToString()));
		m_prog->AppendCode(JSOP_ENTER);
		m_prog->AppendCode(JSOP_DUP);
		Match(JsLex::T_ID);
		MoreSingles();

		// POP unused return val
		m_prog->AppendCode(JSOP_POP);
	}
	else if (m_lex.CurrentToken() == JsLex::T_LPAR)
	{
		Match(JsLex::T_LPAR);
		Expr();
		Match(JsLex::T_RPAR);
		MoreSingles();
	}
	else
	{
		Literal();
	}
}

/**
moresingle	: DOT single
			| LBRACE expr RBRAC moresingles
			| ASSIGN expr
			| LPAR paramlist RPAR moresingles
			|
			;
*/
void JsParse::MoreSingles()
{
	bool needPop = false;
	
	switch(m_lex.CurrentToken())
	{
		case JsLex::T_DOT:
			Match(JsLex::T_DOT);

			{
				StringPtr id(m_lex.Lexum().ToString());
				Match(JsLex::T_ID);

				if (m_lex.CurrentToken() == JsLex::T_LPAR)
				{
					m_prog->AppendCode(JSOP_ENTER);
					m_prog->AppendCode(JSOP_DEREF, JSOPARG_TAB, m_prog->AddToTable(*id));
					m_prog->AppendCode(JSOP_THIS);
					m_prog->AppendCode(JSOP_ROT);
					needPop = true;
				}
				else
				{
					m_prog->AppendCode(JSOP_DEREF, JSOPARG_TAB, m_prog->AddToTable(*id));
				}
			}
			MoreSingles();
			if (needPop)
			{
				m_prog->AppendCode(JSOP_ROT);
				m_prog->AppendCode(JSOP_POP);
			}
			break;
		case JsLex::T_LBRAC:
			Match(JsLex::T_LBRAC);
			Expr();
			Match(JsLex::T_RBRAC);
			m_prog->AppendCode(JSOP_DEREF);
			MoreSingles();
			break;
		case JsLex::T_LPAR:
			Match(JsLex::T_LPAR);
			ArgList();
			Match(JsLex::T_RPAR);
			m_prog->AppendCode(JSOP_CALL);
			break;
	}	
}

/**
literal		: INT | STRING | CHAR | REAL | NULL
			;
*/
void JsParse::Literal()
{
	switch(m_lex.CurrentToken())
	{
		case JsLex::T_INT:
			m_prog->AppendCode(JSOP_PUSH, JSOPARG_TAB, m_prog->AddToTable(Int32::Parse(m_lex.Lexum().GetChars())));
			break;
		case JsLex::T_STRING:
			m_prog->AppendCode(JSOP_PUSH, JSOPARG_TAB, m_prog->AddToTable(m_lex.Lexum().GetChars()));
			break;
		case JsLex::T_REAL:
			m_prog->AppendCode(JSOP_PUSH, JSOPARG_TAB, m_prog->AddToTable(Double::Parse(m_lex.Lexum().GetChars())));
			break;
		case JsLex::T_JNULL:
			m_prog->AppendCode(JSOP_NULL);
			break;
		case JsLex::T_YYTRUE:
			m_prog->AppendCode(JSOP_PUSH, JSOPARG_IMM, 1);
			break;
		case JsLex::T_YYFALSE:
			m_prog->AppendCode(JSOP_PUSH, JSOPARG_IMM, 0);
			break;
		default:
			throw new ParseException("Expected LITERAL, found " + m_lex.Lexum().ToString() + " on line " + Int32::ToString(m_lex.LineNumber()));
	}
	Match(m_lex.CurrentToken());
}

/**
arglist		: literal [COMMA arglist]-
			| ID [COMMA arglist]-
			;
*/
void JsParse::ArgList()
{
	if (m_lex.CurrentToken() == JsLex::T_RPAR)
	{
		return;
	}

	Expr();
	
	if (m_lex.CurrentToken() == JsLex::T_COMMA)
	{
		Match(JsLex::T_COMMA);
		ArgList();
	}
}
