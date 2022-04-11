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
/** @file spl/types.h
 *  @brief This is the main header file for the library, but it is included from spl/debug.h.
 *
 */
#ifndef _types_h
#define _types_h

#if defined(_WIN32) || defined(_WIN64)
//#include <spl/cleanwindows.h>
#ifndef _WINDOWS
#define _WINDOWS 1
#endif
#include <spl/configwin32.h>
#else
#include <spl/autoconf/config.h>
#endif

#define HAVE_CONFIG_H 1

#if HAVE_TYPES_H
#include <types.h>
#endif
#if HAVE_SYS_TYPES_H
#include <sys/types.h>
#endif
#if HAVE_STRING_H
#include <string.h>
#endif

/**
 *  @mainpage spl Standard Portable Library
 *
 *  @section intro Introduction
 *		The Standard Portable Library (SPL) is a smart pointer based cross platform
 *		application framework.  SPL is intended to be used instead of STL and has an API
 *		that should be familiar to dot net and java programmers.  It includes basic types
 *		such as Date and DateTime, runtime pointer debugging, collection templates, file IO,
 *		networking, database access, threading, and XML parsing.
 *		<a href='http://sourceforge.net/projects/spl'>Sourceforge site</a>
 *
 *	@section anoy Things You Might Not Like About SPL
 *		<ul>
 *		<li>SPL should be considered as beta quality, i.e. feature complete but not necessarily ready for production.
 *		At this point, the only missing planned features are XPath, XmlNode.SelectSingle(), XmlNode.SelectNodes(), and fixed width file handling.</li>
 *		<li>Pascal/C# naming conventions are used, so member functions start with an upper case letter.</li>
 *		<li>Prefix type notation, such as lpcstrzMyVar, is not used.</li>
 *		<li>Unit test code coverage is only about 30% or 40%.</li>
 *		<li>Smart pointers of templated types don't have typedef'ed short names.  For example
 *		the String smart pointer has
 *		a typedef of StringPtr.  However, a List of Strings must be declared using
 *		RefCountPtr&lt;List&lt;String&gt;&nbsp;&gt;</li>
 *		<li>DEBUG builds of applications with numerous memory allocations can run slowly due to
 *		extensive runtime pointer validation.</li>
 *		</ul>
 *
 *	@section setup Application Organization
 *		<table border='0'><tr><td><p>There are a few steps to setup and teardown the library.  The
 *			skeleton below is for command line applications.</p>&nbsp;<br /><p>
@code
#include <spl/Debug.h>
#include <spl/configuration/CommandLine.h>
#include <spl/io/log/Log.h>
#include <spl/UnitTest.h>
#include <spl/net/Socket.h>

using namespace spl;

int main(int argc, char **argv)
{
	try
	{
		CommandLine args(argc, (const char **)argv);

		//
		// Do stuff.
		//

		ASSERT_MEM_FREE();
		Socket::ShutdownService();
		DEBUG_TEAR_DOWN(true);

		return (Log::SMessageCount() > 0) ? 20 : 0;
	}
	catch ( Exception *ex )
	{
		Log::SWrite( ex );
		delete ex;
	}
	catch ( OutOfMemoryException mex )
	{
		Log::SWrite( mex );
	}
	return 20;
}
@endcode
		</p>
 *		</td></tr></table>
 *
 *	@section windoze Windows Development
 *		If you're doing windozes development, be sure to include stdafx, windows top level
 *		headers, and &#35;import's prior to including SPL headers.  SPL uses an abrievated set
 *		of windows headers to get networking to work.  If you susequently include &lt;windows.h&gt;,
 *		you'll get a lot of errors.
 *
 *	@section smptr Smart Pointers
 *		<p>There are three templates used to implement smart pointers: RefCountPtr, RefCountPtrCast,
 *		and WeakReference.  RefCountPointer overloads "->", so variables declared with it can
 *		be used just like orginary pointers.  The raw pointer is also available if needed.</p><p>
 *@code
RefCountPtr<String> pstr(new String("Hi"));

// Call String member function.
int len = pstr->Length();

// Use the raw String pointer.
len = pstr.Get()->Length();
@endcode
 *		</p><p>When you need to cast from pointer type to another, define a RefCountPtrCast.
 *		For example, let's say that you have a class SuperString that inherits from String.
 *		Putting the following definitions in SuperString's header will allow the compliler
 *		to automaticly convert the pointers.</p><p>
 *@code
#include <spl/String.h>

class SuperString : public spl::String
{
};

typedef spl::RefCountPtrCast<SuperString, spl::String, spl::StringPtr> SuperStringPtr;
@endcode
 *		</p><p>To avoid circular references that would prevent the pointer from being
 *		released, use WeakReference.  Let's say the A creates an instance of B and B
 *		holds a back pointer to A.  A should use RefCountPtr&lt;B&gt; and B should use
 *		WeakReference&lt;A&gt;  You'll also need to use an interface to break the compiler
 *		dependancy loop.</p>
 *	&nbsp;<br />
 *	<p><a href="http://sourceforge.net"><img
 *	src="http://sflogo.sourceforge.net/sflogo.php?group_id=186770&amp;type=1"
 *	width="88" height="31" border="0" alt="SourceForge.net Logo" /></a></p>
 *
 *	@section toc Packages
 *		<table cellpadding='0' cellspacing='0' border='0'><tr><td width='20px'>&nbsp;</td><td>
 *		<table border='1'>
 *		<tr><td>@ref core</td><td>
 *			<table border='0'>
 *			<tr><td>@ref smrtptr</td></tr>
 *			<tr><td>@ref types</td></tr>
 *			<tr><td>@ref debugging</td></tr>
 *			<tr><td>@ref env</td></tr>
 *			<tr><td>@ref logging</td></tr>
 *			</table>
 *		</td></tr>
 *		<tr><td>@ref collection</td><td>&nbsp;
 *		</td></tr>
 *		<tr><td>@ref config</td><td>&nbsp;
 *		</td></tr>
 *		<tr><td>@ref database</td><td>
 *			<table border='0'>
 *			<tr><td>@ref datamodel</td></tr>
 *			<tr><td>@ref sqlite</td></tr>
 *			<tr><td>@ref mysql</td></tr>
 *			</table>
 *		</td></tr>
 *		<tr><td>@ref io</td><td>
 *			<table border='0'>
 *			<tr><td>@ref stream</td></tr>
 *			<tr><td>@ref file</td></tr>
 *			</table>
 *		</td></tr>
 *		<tr><td>@ref math</td><td>&nbsp;
 *		</td></tr>
 *		<tr><td>@ref network</td><td>
 *			<table border='0'>
 *			<tr><td>@ref packet</td></tr>
 *			<tr><td>@ref socket</td></tr>
 *			</table>
 *		</td></tr>
 *		<tr><td>@ref threading</td><td>
 *			<table border='0'>
 *			<tr><td>@ref threads</td></tr>
 *			<tr><td>@ref sync</td></tr>
 *			</table>
 *		</td></tr>
 *		<tr><td>@ref text</td><td>&nbsp;
 *		</td></tr>
 *		<tr><td>@ref web</td><td>&nbsp;
 *		</td></tr>
 *		<tr><td>@ref xml</td><td>&nbsp;
 *		</td></tr>
 *		</table>
 *		</td></tr></table>
 *
 *  @section authors  SPL includes source code from the sources below (see the source file headers for
 *		specific attributions and modification notices).
 *		<ul>
 *		<li>John Garrison</li>
 *		<li><a href='http://www.microsoft.com/mspress/books/1024.aspx'>Writing Solid Code</a></li>
 *		<li><a href='http://www.bouncycastle.org/'>bouncycastle.org</a></li>
 *		<li><a href='http://sourceforge.net/projects/cplusplus/'>commonc++</a></li>
 *		<li><a href='http://www.gnu.org/software/classpath'>GNU Classpath</a></li>
 *		<li><a href='http://www.pcre.org'>PCRE</a></li>
 *		<li><a href='http://www.sqlite.org'>SQLITE</a></li>
 *		<li><a href='http://sourceforge.net/projects/tinyxml'>TinyXml</a></li>
 *		<li>Brian Parker. "Poor Man's typeof() Implementation," Comp.std.c++, 10 November 1997</li>
 *		</ul>
 *
 */

namespace spl
{
/**
 * @defgroup core Core
 * @{
 */

/**
 * @defgroup types Types
 * @ingroup core
 * @{
 */

#if defined(_WIN32) || defined(WIN32) || defined(_WIN64) || defined(WIN64) || defined(_MSC_VER)
#ifndef _WIN32	//< TODO should be able to remove this
#define _WIN32
#endif
#ifndef WIN32	//< TODO should be able to remove this
#define WIN32
#endif
#ifndef _WINDOWS
#define _WINDOWS
#endif
#if !defined(LITTLE_ENDIAN) && !defined(BIG_ENDIAN)
#define LITTLE_ENDIAN 1
#endif
	typedef signed __int64 int64;		//< A signed 64-bit integer.
	typedef unsigned __int64 uint64;	//< An unsigned 64-bit integer.  Note: this is signed on platforms that don't support unsigned 64 bits.
	typedef signed int int32;			//< A signed 32-bit integer.
	typedef signed short int16;			//< A signed 16-bit integer.
	typedef __int8 int8;				//< A signed 8-bit integer.
	typedef unsigned char byte;			//< 8 unsigned bits.

	typedef unsigned int uint32;		//< Unsigned 32-bit integer.
	typedef unsigned short uint16;		//< Unsigned 16-bit integer.
	typedef unsigned __int8 uint8;		//< Unsigned 8-bit integer.

	typedef float float32;				//< float, should be 32-bits.
	typedef double float64;				//< double, should be 64-bits.

#elif __GNUC__
#if !defined(LITTLE_ENDIAN) && !defined(BIG_ENDIAN)
#define LITTLE_ENDIAN 1
#endif
	typedef signed long long int64;
	typedef unsigned long long uint64;
	typedef signed int int32;
	typedef signed short int16;
	typedef char int8;
	typedef unsigned char byte;

	typedef unsigned int uint32;
	typedef unsigned short uint16;
	typedef unsigned char uint8;

	typedef float float32;
	typedef double float64;

	char *itoa( int i, char *buf, const int buflen );

#elif defined(__TANDEM)
#define _TANDEM		/* I've mixed __ and _ in the code */

#include <sys/types.h>
#include <wchar.h>
#define _tal
#define _extensible
#if !defined(LITTLE_ENDIAN) && !defined(BIG_ENDIAN)
#define BIG_ENDIAN 1
#endif
	typedef signed long long int64;
	typedef unsigned long long uint64;
	/*typedef signed long long uint64;*/
	typedef signed int int32;
	typedef signed short int16;
	typedef char int8;
	typedef unsigned char byte;

	typedef unsigned int uint32;
	typedef unsigned short uint16;
	typedef unsigned char uint8;

	typedef float float32;
	typedef double float64;

	char *itoa( int i, char *buf, const int buflen );
	/*int iswspace( char ch );*/

#ifndef _tcp_TYPES
#define _tcp_TYPES
	typedef unsigned char  u_char;
	typedef unsigned short u_short;
	typedef unsigned int   u_int;
	typedef unsigned long  u_long;
	typedef unsigned short ushort;
#endif /* _tcp_TYPES */

#else

	#error Unsupported Target

#endif

#ifndef __cplusplus
#define false 0
#define true 1
typedef char bool;
#endif

#define	IL_MIN_INT32				((int32)0x80000000)
#define	IL_MAX_INT32				((int32)0x7FFFFFFF)
#define	IL_MAX_UINT32				((uint32)0xFFFFFFFFL)
#define	IL_MIN_INT64				((int64)0x8000000000000000LL)
#define	IL_MAX_INT64				((int64)0x7FFFFFFFFFFFFFFFLL)
#define	IL_MAX_UINT64				((int64)0x7FFFFFFFFFFFFFFFLL)

#define	READ_BYTE(buf, offset)	((int32)(((int8 *)(buf))[(offset)]))
#define	READ_BYTE_SHIFT(buf,offset,shift) (((uint32)(((uint8 *)(buf))[(offset)])) << (shift))

/** These are used for templates that don't know the types of their args */
inline void TypeValidate( int8 i ) {}
inline void TypeValidate( int16 i ) {}
inline void TypeValidate( int32 i ) {}
inline void TypeValidate( int64 i ) {}
inline void TypeValidate( float32 i ) {}
inline void TypeValidate( float64 i ) {}
inline void TypeValidate( byte i ) {}
inline void TypeValidate( uint16 i ) {}
inline void TypeValidate( uint32 i ) {}
#ifndef __TANDEM
inline void TypeValidate( uint64 i ) {}
#endif

inline void TypeCheckMem( int8 i ) {}
inline void TypeCheckMem( int16 i ) {}
inline void TypeCheckMem( int32 i ) {}
inline void TypeCheckMem( int64 i ) {}
inline void TypeCheckMem( float32 i ) {}
inline void TypeCheckMem( float64 i ) {}
inline void TypeCheckMem( byte i ) {}
inline void TypeCheckMem( uint16 i ) {}
inline void TypeCheckMem( uint32 i ) {}
#ifndef __TANDEM
inline void TypeCheckMem( uint64 i ) {}
#endif

/// @brief Lets Math return hash codes in an implementation independant way (mostly for Hashtable keys).
class IHashable
{
public:
	inline IHashable() {}
	virtual ~IHashable();
	virtual uint32 HashCode() const = 0;
};

/// @brief Lets Hashtable (through Math) use pointer keys.  Implementors of IComparable
/// assume that the argument is of their type.  I.e., arbitrary types cannot be
/// passed to Equals and Compare.
class IComparable : public IHashable
{
public:
	inline IComparable() {}
	virtual ~IComparable();
	virtual bool Equals( const IComparable& a ) const = 0;
	virtual int Compare( const IComparable& a ) const = 0;

	///@brief Class instances with the same majic number are of the same type (can be casted).
	/// Majic numbers above 0xFFFF are available for user applications.
	virtual int32 MajicNumber() const = 0;
	virtual uint32 HashCode() const = 0;
};

#define BYTE_MAJIC 0x0112
#define BOOL_MAJIC 0x0113
#define INT16_MAJIC 0x0114
#define FLOAT32_MAJIC 0x0115

/** @} */
/** @} */
}

#include <spl/typeof.h>

#endif
