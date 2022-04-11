#include <spl/Debug.h>
#include <spl/interp/JsParse.h>
#include <spl/io/log/Log.h>
#include <spl/interp/VarInterp.h>

#ifdef DEBUG

using namespace spl;

static void _TestJsParseNull()
{
	JsParse *jp = new JsParse();
	
	DEBUG_CLEAR_MEM_CHECK_POINTS();
	DEBUG_NOTE_MEM( jp );
	jp->CheckMem();
	DEBUG_DUMP_MEM_LEAKS();
	UNIT_ASSERT_MEM_NOTED("_TestJsParseNull 1");

	delete jp;
	DEBUG_CLEAR_MEM_CHECK_POINTS();
	UNIT_ASSERT_MEM_NOTED("_TestJsParseNull 2");
	Log::SWriteOkFail( "JsParse null" );
}

static void _TestJsParseVar()
{
	JsParse *jp = new JsParse(false);
	VariantPtr method(new Variant(VarInterp::CreateDefaultContext(jp->Parse("var x;\n"))));
	
	DEBUG_CLEAR_MEM_CHECK_POINTS();
	DEBUG_NOTE_MEM( jp );
	jp->CheckMem();
	method.CheckMem();
	DEBUG_DUMP_MEM_LEAKS();
	UNIT_ASSERT_MEM_NOTED("_TestJsParseVar 1");

	delete jp;
	method.Release();
	DEBUG_CLEAR_MEM_CHECK_POINTS();
	UNIT_ASSERT_MEM_NOTED("_TestJsParseVar 2");
	Log::SWriteOkFail( "JsParse var def" );
}

static VarInterpPtr _TestRunVarProg(const char *src)
{
	JsParse *jp = new JsParse(true);
	ProgramPtr prog(jp->Parse(src));
	
	//StringPtr dis(prog->ToString());
	//printf("\n\n%s\n%s\n", src, dis->GetChars());
	
	VariantPtr method(new Variant(VarInterp::CreateDefaultContext(prog)));
	Vector<VariantPtr> args;
	
	VarInterpPtr vi(new VarInterp(method, args));
	bool ret = vi->Execute(true);
	
	delete jp;
	
	return vi;
}

static void _TestJsParseAdd()
{
	VarInterpPtr vi = _TestRunVarProg("this.x = 1 + 1;\n");

	DEBUG_CLEAR_MEM_CHECK_POINTS();
	vi.CheckMem();
	DEBUG_DUMP_MEM_LEAKS();
	UNIT_ASSERT_MEM_NOTED("_TestJsParseAdd 1");

	{
		VariantPtr v = vi->FindProperty("x");
		StringPtr s = v->ToString();
		UNIT_ASSERT("type", 0 == spl::StrCmp(v->TypeNameInner().GetChars(), "int64", 10));
		UNIT_ASSERT("2", *s == "2");

		DEBUG_CLEAR_MEM_CHECK_POINTS();
		vi.CheckMem();
		v.CheckMem();
		s.CheckMem();
		DEBUG_DUMP_MEM_LEAKS();
		UNIT_ASSERT_MEM_NOTED("_TestJsParseAdd 3");
	}
	
	vi = _TestRunVarProg("var x = 3 - 1;\n");
	{
		VariantPtr v = vi->StackPeek();;
		StringPtr s = v->ToString();
		UNIT_ASSERT("2", *s == "2");
	}
	
	vi = _TestRunVarProg("var x = 3;\nx += 2;");
	{
		VariantPtr v = vi->StackPeek();;
		StringPtr s = v->ToString();
		UNIT_ASSERT("5", *s == "5");
	}
	
	vi.Release();
	DEBUG_CLEAR_MEM_CHECK_POINTS();	
	DEBUG_DUMP_MEM_LEAKS();
	UNIT_ASSERT_MEM_NOTED("_TestJsParseAdd 2");
	Log::SWriteOkFail( "JsParse add assign" );
}

static void _TestJsParseExpr()
{
	int x = x = 1 + 1 * 6 / (3 - 1);
	VarInterpPtr vi = _TestRunVarProg("this.x = 1 + 1 * 6 / (3 - 1);\n");

	{
		VariantPtr v = vi->FindProperty("x");
		StringPtr s = v->ToString();
		UNIT_ASSERT("type", 0 == spl::StrCmp(v->TypeNameInner().GetChars(), "int64", 10));
		UNIT_ASSERT("4", *s == *Int32::ToString(x));
	}
	
	vi.Release();

	DEBUG_CLEAR_MEM_CHECK_POINTS();
	DEBUG_DUMP_MEM_LEAKS();
	UNIT_ASSERT_MEM_NOTED("_TestJsParseExpr 2");
	Log::SWriteOkFail( "JsParse expr" );
}

static void _TestJsParseFunction()
{
	VarInterpPtr vi = _TestRunVarProg("function addone(a, b) {\n return a + b; \n}\n this.x = addone(2, 1);\n");

	{
		VariantPtr v = vi->FindProperty("x");
		StringPtr s = v->ToString();
		UNIT_ASSERT("type", 0 == spl::StrCmp(v->TypeNameInner().GetChars(), "int64", 10));
		UNIT_ASSERT("3", *s == "3");
	}
	
	vi.Release();

	DEBUG_CLEAR_MEM_CHECK_POINTS();
	DEBUG_DUMP_MEM_LEAKS();
	UNIT_ASSERT_MEM_NOTED("js function 2");
	Log::SWriteOkFail( "JsParse function" );
}

static void _TestJsParseIf()
{
	const char *src =
		"function max(a, b) {\n" \
		"	if (a > b) { return a; }\n" \
		"	else { return b; } \n" \
		"}\n" \
		"this.x = max(1, 2);\n" \
		"this.y = max(2, 1);\n";
	
	VarInterpPtr vi = _TestRunVarProg(src);
	{
		VariantPtr v = vi->FindProperty("x");
		StringPtr s = v->ToString();
		UNIT_ASSERT("type", 0 == spl::StrCmp(v->TypeNameInner().GetChars(), "int32", 10));
		UNIT_ASSERT("2", *s == "2");

		v = vi->FindProperty("y");
		s = v->ToString();
		UNIT_ASSERT("type", 0 == spl::StrCmp(v->TypeNameInner().GetChars(), "int32", 10));
		UNIT_ASSERT("2", *s == "2");
	}
	
	src = 
		"function cases(arg) {\n" \
		"	if (arg == 1) { return \"a\"; }\n" \
		"	else if (arg == 2) { return \"b\"; }\n" \
		"	else { return \"c\"; }\n" \
		"}\n" \
		"var x = cases(1);\n" \
		"var y = cases(2);\n" \
		"var z = cases(0);\n";
	
	vi = _TestRunVarProg(src);
	{
		VariantPtr v = vi->StackPeek();
		StringPtr s = v->ToString();
		UNIT_ASSERT("type", 0 == spl::StrCmp(v->TypeNameInner().GetChars(), "String", 10));
		UNIT_ASSERT("c", *s == "c");
		vi->PopStack();
		
		v = vi->StackPeek();
		s = v->ToString();
		UNIT_ASSERT("type", 0 == spl::StrCmp(v->TypeNameInner().GetChars(), "String", 10));
		UNIT_ASSERT("b", *s == "b");
		vi->PopStack();

		v = vi->StackPeek();
		s = v->ToString();
		UNIT_ASSERT("type", 0 == spl::StrCmp(v->TypeNameInner().GetChars(), "String", 10));
		UNIT_ASSERT("a", *s == "a");
		vi->PopStack();
	}
	
	vi.Release();

	DEBUG_CLEAR_MEM_CHECK_POINTS();
	DEBUG_DUMP_MEM_LEAKS();
	UNIT_ASSERT_MEM_NOTED("_TestJsParseFunction 2");
	Log::SWriteOkFail( "JsParse if" );
}

static void _TestJsParseWhile()
{
	const char *src =
		"var x = 0;\n" \
		"var y = 1;\n" \
		"while(x < 10) {" \
		"	x++; \n" \
		"}\n";
	
	VarInterpPtr vi = _TestRunVarProg(src);
	{
		UNIT_ASSERT("stk count", vi->StackCount() == 2);
		VariantPtr v = vi->StackPeek();
		StringPtr s = v->ToString();
		UNIT_ASSERT("type", 0 == spl::StrCmp(v->TypeNameInner().GetChars(), "int32", 10));
		UNIT_ASSERT("1", *s == "1");
		
		vi->PopStack();
		
		v = vi->StackPeek();
		s = v->ToString();
		UNIT_ASSERT("type", 0 == spl::StrCmp(v->TypeNameInner().GetChars(), "int64", 10));
		UNIT_ASSERT("10", *s == "10");
		
	}

	vi.Release();

	DEBUG_CLEAR_MEM_CHECK_POINTS();
	DEBUG_DUMP_MEM_LEAKS();
	UNIT_ASSERT_MEM_NOTED("_TestJsParseFunction 2");
	Log::SWriteOkFail( "JsParse while" );
}

static void _TestJsParseFor()
{
	const char *src =
		"for(var x = 0; x < 10; x++) {" \
		"	;\n" \
		"}\n";
	
	VarInterpPtr vi = _TestRunVarProg(src);
	{
		UNIT_ASSERT("stack should be one", vi->StackCount() == 1);
		VariantPtr v = vi->StackPeek();
		StringPtr s = v->ToString();
		UNIT_ASSERT("type", 0 == spl::StrCmp(v->TypeNameInner().GetChars(), "int64", 10));
		UNIT_ASSERT("10", *s == "10");
	}

	vi.Release();

	DEBUG_CLEAR_MEM_CHECK_POINTS();
	DEBUG_DUMP_MEM_LEAKS();
	UNIT_ASSERT_MEM_NOTED("_TestJsParseFunction 2");
	Log::SWriteOkFail( "JsParse for" );
}

static void _TestJsParseBreak()
{
	const char *src =
		"var x = 0;\n" \
		"while(x < 10) {" \
		"	if(x++ > 0) { break; }; \n" \
		"}\n" \
		"this.y = x + 1;\n";
	
	VarInterpPtr vi = _TestRunVarProg(src);
	{
		VariantPtr v = vi->StackPeek();
		StringPtr s = v->ToString();
		UNIT_ASSERT("type", 0 == spl::StrCmp(v->TypeNameInner().GetChars(), "int64", 10));
		UNIT_ASSERT("2", *s == "2");
		
		v = vi->FindProperty("y");
		s = v->ToString();
		UNIT_ASSERT("type", 0 == spl::StrCmp(v->TypeNameInner().GetChars(), "int64", 10));
		UNIT_ASSERT("3", *s == "3");
	}

	vi.Release();

	DEBUG_CLEAR_MEM_CHECK_POINTS();
	DEBUG_DUMP_MEM_LEAKS();
	UNIT_ASSERT_MEM_NOTED("_TestJsParseFunction 2");
	Log::SWriteOkFail( "JsParse break" );
}

static void _TestJsParseSwitch()
{
	const char *src =
		"var x = 0;\n" \
		"switch(5) {" \
		"	case 4: x = 4; break; \n" \
		"	case 5: x = 5; break; \n" \
		"	default: x = 1; break; \n" \
		"}\n" \
		"";
	
	VarInterpPtr vi = _TestRunVarProg(src);
	{
		VariantPtr v = vi->StackPeek();
		StringPtr s = v->ToString();
		UNIT_ASSERT("type", 0 == spl::StrCmp(v->TypeNameInner().GetChars(), "int32", 10));
		UNIT_ASSERT("5", *s == "5");		
	}

	vi.Release();
	
	src =
		"var x = 0;\n" \
		"switch(4) {" \
		"	case 4: x = 4; break; \n" \
		"	case 5: x = 5; break; \n" \
		"	default: x = 1; break; \n" \
		"}\n" \
		"";
	
	vi = _TestRunVarProg(src);
	{
		VariantPtr v = vi->StackPeek();
		StringPtr s = v->ToString();
		UNIT_ASSERT("type", 0 == spl::StrCmp(v->TypeNameInner().GetChars(), "int32", 10));
		UNIT_ASSERT("4", *s == "4");		
	}

	vi.Release();	

	src =
		"var x = 0;\n" \
		"switch(1) {" \
		"	case 4: x = 4; break; \n" \
		"	case 5: x = 5; break; \n" \
		"	default: x = 1; break; \n" \
		"}\n" \
		"";
	
	vi = _TestRunVarProg(src);
	{
		VariantPtr v = vi->StackPeek();
		StringPtr s = v->ToString();
		UNIT_ASSERT("type", 0 == spl::StrCmp(v->TypeNameInner().GetChars(), "int32", 10));
		UNIT_ASSERT("1", *s == "1");
	}

	vi.Release();
	DEBUG_CLEAR_MEM_CHECK_POINTS();
	DEBUG_DUMP_MEM_LEAKS();
	UNIT_ASSERT_MEM_NOTED("js switch 1");
	Log::SWriteOkFail( "JsParse switch" );
}

static void _TestJsParseNew()
{
	const char *src =
		"function MyObj(cs) {\n" \
		"	this.x = cs;\n" \
		"}\n" \
		"var obj = new MyObj(1);\n";
	
	VarInterpPtr vi = _TestRunVarProg(src);
	{
		VariantPtr obj = vi->StackPeek();
		VariantPtr prop = obj->ToObject()->GetProperty("x");
		UNIT_ASSERT("1", *prop->ToString() == "1");
	}

	vi.Release();

	src =
		"function MyObj(cs) {\n" \
		"	switch(cs) {" \
		"		case 1: this.x = 'a'; break;\n" \
		"		case 2: this.x = 'b'; break;\n" \
		"		case 3: this.x = 'c'; break;\n" \
		"		default: this.x = 'z'; break;\n" \
		"	}; \n" \
		"}\n" \
		"var obj = new MyObj(1);\n" \
		"this.q = obj.x;\n";
	
	vi = _TestRunVarProg(src);
	{
		VariantPtr obj = vi->StackPeek();
		VariantPtr prop = obj->ToObject()->GetProperty("x");
		UNIT_ASSERT("a", *prop->ToString() == "a");
		
		obj = vi->FindProperty("q");
		StringPtr s = obj->ToString();
		UNIT_ASSERT("type", 0 == spl::StrCmp(obj->TypeNameInner().GetChars(), "String", 10));
		UNIT_ASSERT("a", *s == "a");
	}

	vi.Release();

	DEBUG_CLEAR_MEM_CHECK_POINTS();
	DEBUG_DUMP_MEM_LEAKS();
	UNIT_ASSERT_MEM_NOTED("_TestJsParseFunction 2");
	Log::SWriteOkFail( "JsParse new" );
}

static void _TestJsParseArray()
{
	const char *src =
		"var x = new Array();\n" \
		"x[0] = 'a';\n" \
		"x[1] = 'b';\n" \
		"x[2] = 'c';\n" \
		"this.y = x[1];\n";
	
	VarInterpPtr vi = _TestRunVarProg(src);
	{
		VariantPtr v = vi->StackPeek();
		UNIT_ASSERT("type", 0 == spl::StrCmp(v->TypeNameInner().GetChars(), "Array", 10));
		UNIT_ASSERT("length", *(v->ToObject()->GetProperty("length")->ToString()) == "3");
		
		v = vi->FindProperty("y");
		StringPtr s = v->ToString();
		UNIT_ASSERT("type", 0 == spl::StrCmp(v->TypeNameInner().GetChars(), "String", 10));
		UNIT_ASSERT("b", *s == "b");
	}

	vi.Release();

	DEBUG_CLEAR_MEM_CHECK_POINTS();
	DEBUG_DUMP_MEM_LEAKS();
	UNIT_ASSERT_MEM_NOTED("_TestJsParseFunction 2");
	Log::SWriteOkFail( "JsParse Array" );
}

static void _TestJsParseProp()
{
	const char *src =
		"var x = new Object();\n" \
		"x['bob'] = 'a';\n" \
		"x['sally'] = 'b';\n" \
		"x['dud'] = 'c';\n" \
		"this.y = x['sally'];\n";
	
	VarInterpPtr vi = _TestRunVarProg(src);
	{
		VariantPtr v = vi->FindProperty("y");
		StringPtr s = v->ToString();
		UNIT_ASSERT("type", 0 == spl::StrCmp(v->TypeNameInner().GetChars(), "String", 10));
		UNIT_ASSERT("b", *s == "b");
	}

	vi.Release();

	DEBUG_CLEAR_MEM_CHECK_POINTS();
	DEBUG_DUMP_MEM_LEAKS();
	UNIT_ASSERT_MEM_NOTED("_TestJsParseFunction 2");
	Log::SWriteOkFail( "JsParse properties" );
}

static void _TestJsParseInherit()
{
	const char *src =
		"function MySuper() {\n" \
		"	this.x = 'hi';\n" \
		"}\n" \
		"function MySub () : MySuper {\n" \
		"	this.y = 'bye';\n" \
		"}\n" \
		"this.q = new MySub();\n" \
		"q.x = 'mom';\n" \
		"this.z = new MySub();\n";
	
	VarInterpPtr vi = _TestRunVarProg(src);
	{
		VariantPtr v = vi->FindProperty("q");
		StringPtr s = v->ToObject()->GetProperty("x")->ToString();
		UNIT_ASSERT("mom", *s == "mom");
		s = v->ToObject()->GetProperty("y")->ToString();
		UNIT_ASSERT("bye", *s == "bye");

		v = vi->FindProperty("z");
		s = v->ToObject()->GetProperty("x")->ToString();
		UNIT_ASSERT("hi", *s == "hi");
		s = v->ToObject()->GetProperty("y")->ToString();
		UNIT_ASSERT("bye", *s == "bye");
	}

	vi.Release();

	DEBUG_CLEAR_MEM_CHECK_POINTS();
	DEBUG_DUMP_MEM_LEAKS();
	UNIT_ASSERT_MEM_NOTED("_TestJsParseFunction 2");
	Log::SWriteOkFail( "JsParse inheritance" );
}

static void _TestJsParseConsole()
{
	const char *src =
		"//var x = 1.23;\n" \
		"//Console.writeln('x = ' + x);\n" \
		"Console.writeln('Console.writeln ' + 'OK');\n";
	
	VarInterpPtr vi = _TestRunVarProg(src);
	vi.Release();

	DEBUG_CLEAR_MEM_CHECK_POINTS();
	DEBUG_DUMP_MEM_LEAKS();
	UNIT_ASSERT_MEM_NOTED("_TestJsParseFunction 2");
	Log::SWriteOkFail( "JsParse properties" );
}

static void _TestJsParseStringFn()
{
	const char *src =
		"var x = 'hI';\n" \
		"this.upper = x.toUpperCase();\n" \
		"this.lower = x.toLowerCase();\n" \
		"\n";
	
	VarInterpPtr vi = _TestRunVarProg(src);
	{
		VariantPtr v = vi->FindProperty("upper");
		StringPtr s = v->ToObject()->ToString();
		UNIT_ASSERT("HI", *s == "HI");
		v = vi->FindProperty("lower");
		s = v->ToObject()->ToString();
		UNIT_ASSERT("hi", *s == "hi");
	}
	
	vi.Release();

	DEBUG_CLEAR_MEM_CHECK_POINTS();
	DEBUG_DUMP_MEM_LEAKS();
	UNIT_ASSERT_MEM_NOTED("_TestJsParseFunction 2");
	Log::SWriteOkFail( "JsParse String functions" );
}

static void _TestJsParseFib()
{
	const char *src =
		"function doFib(fib)\n" \
		"{\n" \
		"	if (fib < 2) {\n" \
		"		return 1;\n" \
		"	}\n" \
		"	return doFib(fib-1) + doFib(fib-2);\n" \
		"}\n";
	
	VarInterpPtr vi = _TestRunVarProg(String(src) + "this.x = doFib(2);\n");
	{
		VariantPtr v = vi->FindProperty("x");
		StringPtr s = v->ToString();
		UNIT_ASSERT("fib(2)", *s == "2");
	}
	
	vi = _TestRunVarProg(String(src) + "this.x = doFib(5);\n");
	{
		VariantPtr v = vi->FindProperty("x");
		StringPtr s = v->ToString();
		UNIT_ASSERT("fib(5)", *s == "8");
	}

	vi.Release();

	DEBUG_CLEAR_MEM_CHECK_POINTS();
	DEBUG_DUMP_MEM_LEAKS();
	UNIT_ASSERT_MEM_NOTED("FIB");
	Log::SWriteOkFail( "JsParse FIB" );
}

static void _TestJsParseMultiArray()
{
	const char *src =
		"function bob() {\n" \
		"	function lenxx() { return ar.length; }\n" \
		"	function lenof(z, zpos) { return z[zpos].length; }\n" \
		"	var x = new Array(1);\n" \
		"	x[0] = new Array(1);\n" \
		"	this.len1 = lenof(x,0);\n" \
		"	this.len2 = x[0].length;\n" \
		"	this.ar = x[0];\n" \
		"	this.bab = lenxx();\n" \
		"}\nthis.thebob = new bob();\n" \
		"var aa = new Array(2); aa[0] = 'a'; aa[1] = 'b';\n" \
		"this.arr = new Array(2);\n" \
		"for (var z = 0; z < aa.length; ++z) { this.arr[z] = aa[z]; }";
	
	VarInterpPtr vi = _TestRunVarProg(src);
	{
		VariantPtr v = vi->FindProperty("thebob")->ToObject()->GetProperty("len1");
		StringPtr s = v->ToString();
		UNIT_ASSERT("1", *s == "1");
		
		v = vi->FindProperty("thebob")->ToObject()->GetProperty("len2");
		s = v->ToString();
		UNIT_ASSERT("1", *s == "1");
		
		v = vi->FindProperty("thebob")->ToObject()->GetProperty("ar");
		UNIT_ASSERT("type", 0 == spl::StrCmp(v->TypeNameInner().GetChars(), "Array", 10));

		v = vi->FindProperty("thebob")->ToObject()->GetProperty("bab");
		s = v->ToString();
		UNIT_ASSERT("1", *s == "1");
		
		v = vi->FindProperty("arr");
		s = v->ToObject()->GetProperty("0")->ToString();
		UNIT_ASSERT("[0]", *s == "a");
		s = v->ToObject()->GetProperty("1")->ToString();
		UNIT_ASSERT("[1]", *s == "b");
	}
	
	vi.Release();

	DEBUG_CLEAR_MEM_CHECK_POINTS();
	DEBUG_DUMP_MEM_LEAKS();
	UNIT_ASSERT_MEM_NOTED("_TestJsParseFunction 2");
	Log::SWriteOkFail( "JsParse multi dim arrays" );
}

static void _TestJsParseMath()
{
	const char *src =
		"var dSum = 0.5;\n" \
		"this.exp = 1.0/(1+Math.exp(-1.0*dSum));\n" \
		"\n";
	
	VarInterpPtr vi = _TestRunVarProg(src);
	{
		VariantPtr v = vi->FindProperty("exp");
		StringPtr s(v->ToString());
		StringPtr exp(Double::ToString(1.0/(1+Math::Exp(-1.0*0.5))));
		UNIT_ASSERT("exp", *s == *exp);
	}
	
	vi.Release();

	DEBUG_CLEAR_MEM_CHECK_POINTS();
	DEBUG_DUMP_MEM_LEAKS();
	UNIT_ASSERT_MEM_NOTED("_TestJsParseFunction 2");
	Log::SWriteOkFail( "JsParse Math" );
}

//static void _TestJsParseNN()
//{
//	const char *src =
//		"class NeuralNetwork(var insize, var h1size, var h2size, var outsize)\n" \
//		"{\n" \
//		"	\n" \
//		"	this.inSize = insize; this.h1Size = h1size; this.h2Size = h2size; this.outSize = outsize;\n" \
//		"	\n" \
//		"	this.dpInput = new Array(inSize);  // nodes\n" \
//		"	this.dpLayer1 = new Array(h1Size);\n" \
//		"	this.dpLayer2 = new Array(h2Size);\n" \
//		"	this.dpOutput = new Array(outSize);\n" \
//		"	\n" \
//		"	this.dpErrors1 = new Array(h1Size);     // error for each node\n" \
//		"	this.dpErrors2 = new Array(h2Size);\n" \
//		"	this.dpErrorsOut = new Array(outSize);\n" \
//		"	\n" \
//		"	this.dpWt1ToIn = new Array(dpInput.length * dpLayer1.length);	// weights from layer 1 to inputs\n" \
//		"	this.dpDelta1ToIn = new Array(dpInput.length * dpLayer1.length); // deltas from layer 1 to inputs\n" \
//		"	\n" \
//		"	this.dpWt2To1 = new Array(dpLayer1.length * dpLayer2.length);	// weights from layer 2 to layer 1\n" \
//		"	this.dpDelta2To1 = new Array(dpLayer1.length * dpLayer2.length);  // deltas from layer 2 to layer 1\n" \
//		"	\n" \
//		"	this.dpWtOutTo2 = new Array(dpLayer2.length * dpOutput.length);	// weights from output to layer 3\n" \
//		"	this.dpDeltaOutTo2 = new Array(dpLayer2.length * dpOutput.length); // deltas from output to layer 3\n" \
//		"	\n" \
//		"	this.dLearnRate = .2;\n" \
//		"	this.dMomentum = .05;\n" \
//		"	this.dWtRange = .2;\n" \
//		"	this.bUseAdaptiveLR = false;\n" \
//		"	this.bUseAdaptiveMom = false;\n" \
//		"	\n" \
//		"	var dPreviousError = 0.0;\n" \
//		"	\n" \
//		"	function getOuputSize() {\n" \
//		"		return dpOutput.length;\n" \
//		"	}\n" \
//		"	\n" \
//		"	function getInputSize() {\n" \
//		"		return dpInput.length;\n" \
//		"	}\n" \
//		"	function setInput(pos, d) {\n" \
//		"		if (pos < 0 || pos >= dpInput.length) \n" \
//		"		{\n" \
//		"			//throw 'Invalid position';\n" \
//		"			return;\n" \
//		"		}\n" \
//		"		dpInput[pos] = d;\n" \
//		"	}\n" \
//		"	function getOutput(pos) {\n" \
//		"		return dpOutput[pos];\n" \
//		"	}\n" \
//		"	function setArray(a, v) {\n" \
//		"		for (var x = 0; x < a.length; x++) {\n" \
//		"			a[x] = v;\n" \
//		"		}\n" \
//		"	}\n" \
//		"	function setInputLayer(in) {\n" \
//		"		if (in.length < dpInput.length) {\n" \
//		"			//throw 'Array size doesn't match number of input nodes';\n" \
//		"		}\n" \
//		"		for (var x = 0; x < in.length; x++) {\n" \
//		"			dpInput[x] = in[x];\n" \
//		"		}\n" \
//		"	}\n" \
//		"	function randomizeWeights()	{\n" \
//		"		// initialize the weights\n" \
//		"		for (var x = 0; x < dpWt1ToIn.length; x++) {\n" \
//		"			dpWt1ToIn[x] = (Math.random() * dWtRange * 2.0) - dWtRange;\n" \
//		"//Console.writeln('randomizeWeights 1 ' + dpWt1ToIn[x]);\n" \
//		"		}\n" \
//		"		for (x = 0; x < dpWt2To1.length; x++) {\n" \
//		"			dpWt2To1[x] = (Math.random() * dWtRange * 2.0) - dWtRange;\n" \
//		"//Console.writeln('randomizeWeights 2 ' + dpWt2To1[x]);\n" \
//		"		}\n" \
//		"//Console.writeln('randomizeWeights 3');\n" \
//		"		for (x = 0; x < dpWtOutTo2.length; x++) {\n" \
//		"			dpWtOutTo2[x] = (Math.random() * dWtRange * 2.0) - dWtRange;\n" \
//		"//Console.writeln('dpWtOutTo2[x]=' + dpWtOutTo2[x]);\n" \
//		"		}\n" \
//		"	}\n" \
//		"	\n" \
//		"	setArray(dpLayer1, 0.0);\n" \
//		"	setArray(dpLayer2, 0.0);\n" \
//		"	setArray(dpOutput, 0.0);\n" \
//		"	randomizeWeights();\n" \
//		"	\n" \
//		"	function activate() {\n" \
//		"//Console.writeln('activateLayer(dpInput, dpLayer1, dpWt1ToIn);');\n" \
//		"		activateLayer(dpInput, dpLayer1, dpWt1ToIn);\n" \
//		"//Console.writeln('activateLayer(dpLayer1, dpLayer2, dpWt2To1);');\n" \
//		"		activateLayer(dpLayer1, dpLayer2, dpWt2To1);\n" \
//		"//Console.writeln('activateLayer(dpLayer2, dpOutput, dpWtOutTo2);');\n" \
//		"//Console.writeln('dpWtOutTo2[0]=' + dpWtOutTo2[0]);\n" \
//		"		activateLayer(dpLayer2, dpOutput, dpWtOutTo2);\n" \
//		"	}\n" \
//		"	function activateLayer(layer1, layer2, wt) {\n" \
//		"		var dSum = 0.0;\n" \
//		"		var iWtPos = 0;\n" \
//		"	    \n" \
//		"		// for each node in layer 1\n" \
//		"		for (var x = 0; x < layer2.length; x++) {\n" \
//		"			dSum = 0.0;\n" \
//		"			// calculate the ouput for this node	\n" \
//		"			for (var y = 0; y < layer1.length; y++) {\n" \
//		"				// sum the weights * outputs.  note that the weights are\n" \
//		"//Console.writeln('layer1[y] = ' + layer1[y]);\n" \
//		"//Console.writeln('iWtPos = ' + iWtPos);\n" \
//		"//Console.writeln('wt[iWtPos] = ' + wt[iWtPos]);\n" \
//		"				dSum += layer1[y] * wt[iWtPos++];\n" \
//		"//Console.writeln('dSum = ' + dSum);\n" \
//		"			}\n" \
//		"			layer2[x] = 1.0/(1+Math.exp(-1.0*dSum));\n" \
//		"//Console.writeln('dSum = ' + dSum);\n" \
//		"//Console.writeln('layer2[' + x + '/' + layer2.length + '] = ' + layer2[x]);\n" \
//		"		}\n" \
//		"	}\n" \
//		"	function train(in, out, dErrTarget) {\n" \
//		"		var x = 0;\n" \
//		"		var delay = 0;\n" \
//		"		var iWtPos = 0;\n" \
//		"		var dErrTotal = 999999.0;\n" \
//		"		var dErrRet = -1;\n" \
//		"\n" \
//		"		while (dErrTotal > dErrTarget) {\n" \
//		"			Console.writeln(dErrTotal);\n" \
//		"			// set the errors to 0\n" \
//		"			setArray(dpErrors1, 0.0);\n" \
//		"			setArray(dpErrors2, 0.0);\n" \
//		"	    \n" \
//		"			// set the input\n" \
//		"			setInputLayer(in);\n" \
//		"	    \n" \
//		"			// activate\n" \
//		"			activate();\n" \
//		"\n" \
//		"			iWtPos = 0;\n" \
//		"			dErrTotal = 0.0;\n" \
//		"	        \n" \
//		"			// calculate the final error\n" \
//		"			for (x = 0; x < dpOutput.length; x++) {\n" \
//		"				// calculate the absolute error for the output layer\n" \
//		"				dpErrorsOut[x] = out[x] - dpOutput[x];\n" \
//		"			\n" \
//		"				// save the squared error\n" \
//		"				dErrTotal += dpErrorsOut[x] * dpErrorsOut[x];\n" \
//		"			\n" \
//		"				// back prop the error, update the weights\n" \
//		"				for (var y = 0; y < dpLayer2.length; y++) {\n" \
//		"					// back prop the error\n" \
//		"					dpErrors2[y] += dpErrorsOut[x] * dpWtOutTo2[iWtPos];\n" \
//		"\n" \
//		"					// update the delta and weight\n" \
//		"					dpDeltaOutTo2[iWtPos] = dLearnRate * dpErrorsOut[x] * dpLayer2[y] + dMomentum * dpDeltaOutTo2[iWtPos];\n" \
//		"					dpWtOutTo2[iWtPos] += dpDeltaOutTo2[iWtPos];\n" \
//		"					//\n" \
//		"					// need to check for exploding weights here\n" \
//		"					//\n" \
//		"					if (dpWtOutTo2[iWtPos] > 100.0 || dpWtOutTo2[iWtPos] < -100.0) {\n" \
//		"						//throw 'Exploding weights';\n" \
//		"					}\n" \
//		"					if (dpWtOutTo2[iWtPos] == 0.0) {\n" \
//		"						//throw 'Weight underflow';\n" \
//		"					}\n" \
//		"					iWtPos++;\n" \
//		"				}\n" \
//		"			}\n" \
//		"			iWtPos = 0;\n" \
//		"			// hidden layer 2\n" \
//		"			for (x = 0; x < dpLayer2.length; x++) {\n" \
//		"				if (dpLayer2[x] > 0) {\n" \
//		"					dpErrors2[x] *= dpLayer2[x] * (1.0 - dpLayer2[x]);\n" \
//		"				}\n" \
//		"				else {\n" \
//		"					dpErrors2[x] *= -dpLayer2[x] * (1.0 + dpLayer2[x]);\n" \
//		"				}\n" \
//		"				// back prop the error, update the weights\n" \
//		"				for (var y = 0; y < dpLayer1.length; y++) {\n" \
//		"					dpErrors1[y] += dpErrors2[x] * dpWt2To1[iWtPos];\n" \
//		"					// update the delta and weight\n" \
//		"					dpDelta2To1[iWtPos] = dLearnRate * dpErrors2[x] * dpLayer1[y] + dMomentum * dpDelta2To1[iWtPos];\n" \
//		"					dpWt2To1[iWtPos] += dpDelta2To1[iWtPos];\n" \
//		"					//\n" \
//		"					// need to check for exploding weights here\n" \
//		"					//\n" \
//		"					if (dpWt2To1[iWtPos] > 100.0 || dpWt2To1[iWtPos] < -100.0) {\n" \
//		"						//throw 'Exploding weights';\n" \
//		"					}\n" \
//		"					if (dpWt2To1[iWtPos] == 0.0) {\n" \
//		"						//throw 'Weight underflow';\n" \
//		"					}\n" \
//		"					iWtPos++;\n" \
//		"				}\n" \
//		"			}\n" \
//		"			iWtPos = 0;\n" \
//		"			// hidden layer 1\n" \
//		"			for (x = 0; x < dpLayer1.length; x++) {\n" \
//		"				if (dpLayer1[x] > 0) {\n" \
//		"					dpErrors1[x] *= dpLayer1[x] * (1 - dpLayer1[x]);\n" \
//		"				}\n" \
//		"				else {\n" \
//		"					dpErrors1[x] *= -dpLayer1[x] * (1 + dpLayer1[x]);\n" \
//		"				}\n" \
//		"				// back prop the error, update the weights\n" \
//		"				for (var y = 0; y < dpInput.length; y++) {\n" \
//		"					// update the delta and weight\n" \
//		"					dpDelta1ToIn[iWtPos] = dLearnRate * dpErrors1[x] * dpInput[y] + dMomentum * dpDelta1ToIn[iWtPos];\n" \
//		"					dpWt1ToIn[iWtPos] += dpDelta1ToIn[iWtPos];\n" \
//		"					//\n" \
//		"					// need to check for exploding weights here\n" \
//		"					//\n" \
//		"					if (dpWt1ToIn[iWtPos] > 100.0 || dpWt1ToIn[iWtPos] < -100.0) {\n" \
//		"						//throw 'Exploding weights';\n" \
//		"					}\n" \
//		"					if (dpWt1ToIn[iWtPos] == 0.0) {\n" \
//		"						//throw 'Weight underflow';\n" \
//		"					}\n" \
//		"					iWtPos++;\n" \
//		"				}\n" \
//		"			}\n" \
//		"			Console.writeln(dErrTotal);\n" \
//		"			if (bUseAdaptiveLR) {\n" \
//		"				delay++;\n" \
//		"				if (dErrTotal < dPreviousError && delay > 10) {\n" \
//		"					dLearnRate += .00001;\n" \
//		"					delay = 0;\n" \
//		"				}\n" \
//		"				else if (dLearnRate > .02) {\n" \
//		"					dLearnRate -= .005 * dLearnRate;\n" \
//		"				}\n" \
//		"				dPreviousError = dErrTotal;\n" \
//		"			}\n" \
//		"			if (dErrRet == -1) {\n" \
//		"				dErrRet = dErrTotal;\n" \
//		"			}\n" \
//		"		} \n" \
//		"		return dErrRet;\n" \
//		"	}\n" \
//		"	function learnXor() {\n" \
//		"		var ins = new Array(4);\n" \
//		"		ins[0] = new Array(1.0, 0.0);\n" \
//		"		ins[1] = new Array(0.0, 0.0);\n" \
//		"		ins[2] = new Array(1.0, 1.0);\n" \
//		"		ins[3] = new Array(0.0, 1.0);\n" \
//		"		var outs = new Array(4);\n" \
//		"		outs[0] = new Array(1);\n" \
//		"		outs[0][0] = 1.0;\n" \
//		"		outs[1] = new Array(1);\n" \
//		"		outs[1][0] = 0.0;\n" \
//		"		outs[2] = new Array(1);\n" \
//		"		outs[2][0] = 1.0;\n" \
//		"		outs[3] = new Array(1);\n" \
//		"		outs[3][0] = 1.0;\n" \
//		"		\n" \
//		"		var maxError = 0.25;\n" \
//		"		var iteration = 0;\n" \
//		"		var maxIterations = 5000;\n" \
//		"		var iCurr = 0;\n" \
//		"		var dCurrentErr = 0.0;\n" \
//		"		\n" \
//		"		Console.writeln('> begining training');\n" \
//		"		while (++iteration < maxIterations) \n" \
//		"		{\n" \
//		"			//try \n" \
//		"			//{\n" \
//		"				dCurrentErr += train(ins[iCurr], outs[iCurr], maxError);\n" \
//		"				Console.writeln('dCurrentErr == ' + dCurrentErr);\n" \
//		"			//}\n" \
//		"			//catch (ex) \n" \
//		"			//{\n" \
//		"			//	Console.writeln(ex);\n" \
//		"			//	return;\n" \
//		"			//}\n" \
//		"			iCurr++;\n" \
//		"			if (iCurr >= ins.length) \n" \
//		"			{\n" \
//		"				iCurr = 0;\n" \
//		"				if (dCurrentErr < maxError) \n" \
//		"				{\n" \
//		"					break;\n" \
//		"				}\n" \
//		"				Console.writeln( '>> error is ' + dCurrentErr );\n" \
//		"				dCurrentErr = 0.0;\n" \
//		"			}\n" \
//		"		}\n" \
//		"		Console.writeln( '> training complete ' + dCurrentErr );\n" \
//		"	}\n" \
//		"}\n";
//	
//	VarInterpPtr vi = _TestRunVarProg(String(src) + "this.x = new NeuralNetwork(2, 5, 5, 1);\nthis.y = x.getInputSize();x.learnXor();\n");
//	{
//		VariantPtr v = vi->FindProperty("y");
//		StringPtr s = v->ToString();
//		UNIT_ASSERT("2", *s == "2");
//	}
//
//	DEBUG_CLEAR_MEM_CHECK_POINTS();
//	DEBUG_DUMP_MEM_LEAKS();
//	UNIT_ASSERT_MEM_NOTED("NN");
//	Log::SWriteOkFail( "JsParse neural network" );
//}

static void _TestJsParseNestedFn()
{
	const char *src =
		"function one() {\n" \
		"	function two() {\n" \
		"		return 2;\n" \
		"	}\n" \
		"	function getVal() {\n" \
		"		return val;\n" \
		"	}\n" \
		"	this.val = 'hi';\n" \
		"}\n" \
		"var y = new one();\n" \
		"this.z = y.two();\n" \
		"this.k = y.getVal();\n";
	
	VarInterpPtr vi = _TestRunVarProg(src);
	{
		VariantPtr v = vi->FindProperty("z");
		StringPtr s = v->ToString();
		UNIT_ASSERT("type", 0 == spl::StrCmp(v->TypeNameInner().GetChars(), "int32", 10));
		UNIT_ASSERT("2", *s == "2");
		
		v = vi->FindProperty("k");
		s = v->ToString();
		UNIT_ASSERT("hi", *s == "hi");
	}
	
	vi.Release();

	DEBUG_CLEAR_MEM_CHECK_POINTS();
	DEBUG_DUMP_MEM_LEAKS();
	UNIT_ASSERT_MEM_NOTED("_TestJsParseFunction 2");
	Log::SWriteOkFail( "JsParse nested functions" );
}

static void _TestJsParseRegExp()
{
	const char *src =
		"var meh = new RegExp('\\\\d{3}', 'g');\n" \
		"this.blah = meh.exec('123456', 'g');\n" \
		"var q = new RegExp('(.).(.er)', 'i');\n" \
		"this.r = q.exec('Internet');\n" \
		"var xx = new RegExp('((4\\\\.[0-3])|(2\\\\.[0-3]))');\nthis.p = xx.exec('Mozilla/4.0');\n" \
		"var str = 'Hello world!';\n" \
		"var x = new RegExp('Hello');\n" \
		"this.y = x.exec(str);\n" \
		"this.z = x.exec('bobs hair world');\n";
	
	VarInterpPtr vi = _TestRunVarProg(src);
	{
		VariantPtr v = vi->FindProperty("y");
		StringPtr s = v->ToString();
		UNIT_ASSERT("input", *v->ToObject()->GetProperty("input")->ToString() == "Hello world!");
		UNIT_ASSERT("type", 0 == spl::StrCmp(v->TypeNameInner().GetChars(), "Array", 10));
		UNIT_ASSERT("Hellow", *s == "Hello");
		s = v->ToObject()->GetProperty("length")->ToString();
		UNIT_ASSERT("1", *s == "1");

		v = vi->FindProperty("p");
		//s = v->ToObject()->GetProperty("length")->ToString();
		//UNIT_ASSERT("1", *s == "1");
		s = v->ToObject()->GetProperty("0")->ToString();
		UNIT_ASSERT("[0]", *s == "4.0");

		v = vi->FindProperty("z");
		s = v->ToString();
		UNIT_ASSERT("type", 0 == spl::StrCmp(v->TypeNameInner().GetChars(), "null", 10));
		UNIT_ASSERT("null", *s == "null");

		v = vi->FindProperty("blah");
		//s = v->ToObject()->GetProperty("length")->ToString();
		//UNIT_ASSERT("2", *s == "2");
		s = v->ToObject()->GetProperty("0")->ToString();
		UNIT_ASSERT("[0]", *s == "123");
		//s = v->ToObject()->GetProperty("1")->ToString();
		//UNIT_ASSERT("[1]", *s == "456");  //IE says this should be 123456

		v = vi->FindProperty("r");
		s = v->ToObject()->GetProperty("length")->ToString();
		UNIT_ASSERT("3", *s == "3");
		s = v->ToObject()->GetProperty("0")->ToString();
		UNIT_ASSERT("[0]", *s == "Inter");
		s = v->ToObject()->GetProperty("1")->ToString();
		UNIT_ASSERT("[1]", *s == "I");
		s = v->ToObject()->GetProperty("2")->ToString();
		UNIT_ASSERT("[2]", *s == "ter");
	}
	
	vi.Release();

	DEBUG_CLEAR_MEM_CHECK_POINTS();
	DEBUG_DUMP_MEM_LEAKS();
	UNIT_ASSERT_MEM_NOTED("_TestJsParseFunction 2");
	Log::SWriteOkFail( "JsParse RegExp" );
}

void _TestJsParse()
{
	_TestJsParseNull();
	DEBUG_CLEAR_MEM_CHECK_POINTS();
	DEBUG_DUMP_MEM_LEAKS();
	
	_TestJsParseVar();
	DEBUG_CLEAR_MEM_CHECK_POINTS();
	DEBUG_DUMP_MEM_LEAKS();
	
	_TestJsParseAdd();
	DEBUG_CLEAR_MEM_CHECK_POINTS();
	DEBUG_DUMP_MEM_LEAKS();
	
	_TestJsParseExpr();
	DEBUG_CLEAR_MEM_CHECK_POINTS();
	DEBUG_DUMP_MEM_LEAKS();
	
	_TestJsParseFunction();
	DEBUG_CLEAR_MEM_CHECK_POINTS();
	DEBUG_DUMP_MEM_LEAKS();

	_TestJsParseIf();
	DEBUG_CLEAR_MEM_CHECK_POINTS();
	DEBUG_DUMP_MEM_LEAKS();
	
	_TestJsParseWhile();
	DEBUG_CLEAR_MEM_CHECK_POINTS();
	DEBUG_DUMP_MEM_LEAKS();	

	_TestJsParseFor();
	DEBUG_CLEAR_MEM_CHECK_POINTS();
	DEBUG_DUMP_MEM_LEAKS();
	
	_TestJsParseBreak();
	DEBUG_CLEAR_MEM_CHECK_POINTS();
	DEBUG_DUMP_MEM_LEAKS();
	
	_TestJsParseSwitch();
	DEBUG_CLEAR_MEM_CHECK_POINTS();
	DEBUG_DUMP_MEM_LEAKS();
	
	_TestJsParseNew();
	DEBUG_CLEAR_MEM_CHECK_POINTS();
	DEBUG_DUMP_MEM_LEAKS();
	
	_TestJsParseArray();
	DEBUG_CLEAR_MEM_CHECK_POINTS();
	DEBUG_DUMP_MEM_LEAKS();

	_TestJsParseProp();
	DEBUG_CLEAR_MEM_CHECK_POINTS();
	DEBUG_DUMP_MEM_LEAKS();
	
	_TestJsParseInherit();
	DEBUG_CLEAR_MEM_CHECK_POINTS();
	DEBUG_DUMP_MEM_LEAKS();

	_TestJsParseConsole();
	DEBUG_CLEAR_MEM_CHECK_POINTS();
	DEBUG_DUMP_MEM_LEAKS();
	
	_TestJsParseStringFn();
	DEBUG_CLEAR_MEM_CHECK_POINTS();
	DEBUG_DUMP_MEM_LEAKS();

	_TestJsParseFib();
	DEBUG_CLEAR_MEM_CHECK_POINTS();
	DEBUG_DUMP_MEM_LEAKS();
	
	_TestJsParseMath();
	DEBUG_CLEAR_MEM_CHECK_POINTS();
	DEBUG_DUMP_MEM_LEAKS();
	
	_TestJsParseMultiArray();
	DEBUG_CLEAR_MEM_CHECK_POINTS();
	DEBUG_DUMP_MEM_LEAKS();
	
	_TestJsParseNestedFn();
	DEBUG_CLEAR_MEM_CHECK_POINTS();
	DEBUG_DUMP_MEM_LEAKS();
	
	_TestJsParseRegExp();
	DEBUG_CLEAR_MEM_CHECK_POINTS();
	DEBUG_DUMP_MEM_LEAKS();
	
	//_TestJsParseNN();
	//DEBUG_CLEAR_MEM_CHECK_POINTS();
	//DEBUG_DUMP_MEM_LEAKS();
}

#endif
