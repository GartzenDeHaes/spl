#ifndef _typeof_h
#define _typeof_h

// Already included from spl/types.h, but just to be safe
#include <spl/types.h>

//#ifndef HAVE_TYPEOF

// http://www.accu-usa.org/2000-05-Main.html
//
// Portable "typeof" operator and test program.
//
// Written by Bill Gibbons 2/18/2000
//
// This example uses function overloading and template specialization
// to implement a restricted form of the "typeof" operator.
//
// Each type for which "typeof" must work must be registered with
// the REGISTER_TYPEOF macro, which generates the required template
// specialization and overloaded function declaration.
//
// An ordinal 1..n is assigned to each type and used to pass type
// information by encoding the type as a number (in an array size)
// and using "sizeof" to extract the value as a constant.
//==================== The "typeof" machinery ====================

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

template<int N> struct typeof_class; // no def’n, only specializations
template<class T> struct WrapType { typedef T U; };

#define REGISTER_TYPEOF(N,T) \
    template<> struct typeof_class<N > { typedef WrapType<T >::U V; }; \
    char (*typeof_fct(const WrapType<T >::U &))[N]

#if defined(_MSC_VER)
#define REGISTER_TYPEOF2(N,T,T2) \
    template<> struct typeof_class<N > { typedef WrapType<T,##T2 >::U V; }; \
    char (*typeof_fct(const WrapType<T,##T2 >::U &))[N]
#else
#define REGISTER_TYPEOF2(N,T,T2) \
    template<> struct typeof_class<N > { typedef WrapType<T,T2 >::U V; }; \
    char (*typeof_fct(const WrapType<T,T2 >::U &))[N]
#endif

#define type_of(x) typeof_class<sizeof(*typeof_fct(x))>::V

//======== Registration of types to be used with "typeof" ========

REGISTER_TYPEOF( 1, char );
REGISTER_TYPEOF( 2, byte );
REGISTER_TYPEOF( 6, int16 );
REGISTER_TYPEOF( 7, uint16 );
REGISTER_TYPEOF( 8, long );
REGISTER_TYPEOF( 9, unsigned long );
REGISTER_TYPEOF( 10, int32 );
REGISTER_TYPEOF( 11, uint32 );
REGISTER_TYPEOF( 12, int64 );
#ifndef __TANDEM
REGISTER_TYPEOF( 13, uint64 );
#endif
REGISTER_TYPEOF( 14, float32 );
REGISTER_TYPEOF( 15, float64 );
REGISTER_TYPEOF( 17, IHashable );
REGISTER_TYPEOF( 18, IComparable );

//#else
//
//#define REGISTER_TYPEOF(a, b)
//#define REGISTER_TYPEOF2(a,b,c)
//
//#endif

#define USER_REGISTER_TYPEOF_START 1024

/** @brief foreach suppport for Array, List, and Vector. 
 *	To add foreach support for collections of your objects, register your class with the REGISTER_TYPEOF macro.
 *	@code
#include <spl/collection/Hashtable.h>
#include <spl/collection/Vector.h>

class MyClass
{
	// class stuff ...
};

REGISTER_TYPEOF(USER_REGISTER_TYPEOF_START+1,MyClass);
REGISTER_TYPEOF(USER_REGISTER_TYPEOF_START+2,Vector<MyClass>);
REGISTER_TYPEOF2(USER_REGISTER_TYPEOF_START+3,Hashtable<String,MyClass>);

@endcode
 *	@ref Arary
 *	@ref Hashtable
 *	@ref List
 *	@ref RedBlackTree
 *	@ref Vector
 *	@ref REGISTER_TYPEOF
 *	@ref REGISTER_TYPEOF2
 *@code
Vector<int> nums();
nums.Add(1);
nums.Add(2);
nums.Add(3);

foreach(i,nums)
{
	printf("%d\n", i.Current());
}
@endcode
 */
#define foreach(_it_,_collection_) \
	for(type_of(_collection_)::Iterator _it_ = _collection_.Begin();_it_.Next(); )

/** @} */
/** @} */
}
#endif
