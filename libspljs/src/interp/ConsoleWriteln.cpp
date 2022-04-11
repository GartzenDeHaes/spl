#include <spl/interp/jsfunctions/ConsoleWriteln.h>
#include <spl/Environment.h>

#include <stdio.h>
#ifdef HAVE_IO_H
#include <io.h>
#endif

#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif

using namespace spl;

ConsoleWriteln::ConsoleWriteln()
{
	m_prog->NoteArgument();
}

ConsoleWriteln::~ConsoleWriteln()
{
}

bool ConsoleWriteln::IsNative() const
{
	return true;
}

VariantPtr ConsoleWriteln::Call(JsMethod *isthis, Vector<VariantPtr>& args)
{
	if (isatty(fileno(stdin)))
	{
		StringPtr txt(args.ElementAtRef(0)->ToString());
		printf( "%s\n", txt->GetChars() );
	}
	return VariantPtr(new Variant());
}

ConsoleReadln::ConsoleReadln()
{
	m_prog->NoteArgument();
}

ConsoleReadln::~ConsoleReadln()
{
}

bool ConsoleReadln::IsNative() const
{
	return true;
}

VariantPtr ConsoleReadln::Call(JsMethod *isthis, Vector<VariantPtr>& args)
{
	if (isatty(fileno(stdin)))
	{
		char buf[128];
		fgets( buf, sizeof(buf), stdin );
		return VariantPtr(new Variant(buf));
	}
	return VariantPtr(new Variant());
}
