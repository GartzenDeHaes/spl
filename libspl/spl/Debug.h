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
#ifndef _debug_h
#define _debug_h

#include <stdlib.h>
#include <assert.h>

#if defined(_WIN32) || defined(WIN32) || defined(_WIN64) || defined(WIN64)
#ifndef _WINDOWS
#define _WINDOWS
#endif
#endif

#include <spl/types.h>

#if defined _DEBUG || defined DEBUG

#ifndef DEBUG
#define DEBUG
#endif

#ifndef _DEBUG
#define _DEBUG
#endif

/** 
 * @defgroup debugging Debugging
 * @ingroup core
 * @{
 */

#ifdef __cplusplus
void *operator new(size_t size);
void operator delete(void *vp);
void *operator new[](size_t size);
void operator delete[](void *vp);
#endif

#undef malloc
#undef free

#ifndef NULL
#define NULL ((void *)0)
#endif

void _unitAssert( const char *msg, const int cond, const char *filename, const int lineno );
void _unitTest( const char *msg, const int cond);

void *_debugMalloc( const int, const char *filename, const int lineno, const bool isarray );
void *_rpl_malloc( const int size, char *filename, const int lineno, const bool isarray );
void _debugFree( void * );
void _debugAssert( const int cond, const char* filename, const int lineno );
int _debugCheckPtr( const void *ptr );
int _debugCheckBlock( const void *ptr, const int size );
void _debugClearMemCheckPoints();
void _debugNoteMemBlock( const void *vp );
int _debugCheckMem();
int _debugAssertMemFree();
void _debugCheckpointHeap();
int _debugAssertCheckPoint();
void _debugTearDown( bool reallyfree );
void _debugDumpMemLeaks();
void _debugValidateHeap();
//void _debugLockHeap();
//void _debugUnlockHeap();
void _debugFreeHeap();
void _debugEnableHeapLocking();

#define malloc(size) _debugMalloc(size, __FILE__, __LINE__, false)
#define free(ptr) _debugFree(ptr)
void realfree(void *vp);

#undef ASSERT
#define ASSERT(cond) _debugAssert( cond, __FILE__, __LINE__ )
#define ASSERT_PTR(ptr) ASSERT(_debugCheckPtr( ptr ))
#define ASSERT_MEM(ptr, size) ASSERT(_debugCheckBlock( ptr, size ))
#define ASSERT_MEM_FREE() ASSERT(_debugAssertMemFree())

#define DEBUG_CLEAR_MEM_CHECK_POINTS() _debugClearMemCheckPoints()
#define DEBUG_NOTE_MEM_ALLOCATION(a) _debugNoteMemBlock( a )
#define DEBUG_NOTE_MEM(a) _debugNoteMemBlock( a )
#define DEBUG_ASSERT_MEM_NOTED() ASSERT(_debugCheckMem())

#define DEBUG_CHECK_POINT_HEAP() _debugCheckpointHeap()
#define DEBUG_ASSERT_CHECK_POINT() ASSERT(_debugAssertCheckPoint())
#define DEBUG_TEAR_DOWN(a) _debugTearDown(a)
#define DEBUG_DUMP_MEM_LEAKS() _debugDumpMemLeaks()
//#define DEBUG_LOCK_HEAP() _debugLockHeap()
//#define DEBUG_UNLOCK_HEAP() _debugUnlockHeap()
#define DEBUG_FREE_HEAP() _debugFreeHeap()
#define DEBUG_ENABLE_HEAP_LOCK() _debugEnableHeapLocking()

#ifdef DEBUG2
#define DEBUG_VALIDATE() _debugValidateHeap()
#define DEBUG_PTR2(ptr) ASSERT(_debugCheckPtr( ptr ))
#define ASSERT_MEM2(ptr, size) ASSERT(_debugCheckBlock( ptr, size ))
#else
#define DEBUG_VALIDATE()
#define DEBUG_PTR2(ptr)
#define ASSERT_MEM2(ptr, size)
#endif

#define UNIT_TEST(msg,cond) _unitTest(msg,cond)
#define UNIT_ASSERT(msg,cond) _unitAssert( msg, cond, __FILE__, __LINE__ )
#define UNIT_ASSERT_PTR(msg,ptr) UNIT_ASSERT(msg,_debugCheckPtr( ptr ))
#define UNIT_ASSERT_MEM(msg,ptr,size) UNIT_ASSERT(msg,_debugCheckBlock( ptr, size ))
#define UNIT_ASSERT_MEM_FREE(msg) UNIT_ASSERT(msg,_debugAssertMemFree())
#define UNIT_ASSERT_MEM_NOTED(msg) UNIT_ASSERT(msg,_debugCheckMem())
#define UNIT_ASSERT_CHECK_POINT(msg) UNIT_ASSERT(msg,_debugAssertCheckPoint())

void debugAssertCPP(const int cond, const char *file, const int line);

#undef ASSERT
#define ASSERT(cond) if(!(cond)) debugAssertCPP(false, __FILE__, __LINE__ )

#else

void *rpl_malloc( const int size );

#ifndef ASSERT
#define ASSERT(cond)
#endif
#define ASSERT_PTR(ptr) 
#define ASSERT_MEM(ptr, size) 
#define ASSERT_MEM_FREE()

#define DEBUG_CLEAR_MEM_CHECK_POINTS() 
#define DEBUG_NOTE_MEM_ALLOCATION(a)
#define DEBUG_NOTE_MEM(a)
#define DEBUG_ASSERT_MEM_NOTED() 

#define DEBUG_CHECK_POINT_HEAP()
#define DEBUG_ASSERT_CHECK_POINT()
#define DEBUG_TEAR_DOWN(a)
#define DEBUG_DUMP_MEM_LEAKS()

#define UNIT_TEST(cond,msg)
#define UNIT_ASSERT(msg,cond)
#define UNIT_ASSERT_PTR(msg,ptr) 
#define UNIT_ASSERT_MEM(msg,ptr,size) 
#define UNIT_ASSERT_MEM_FREE(msg) 
#define UNIT_ASSERT_MEM_NOTED(msg) 
#define UNIT_ASSERT_CHECK_POINT(msg)
#define DEBUG_ENABLE_HEAP_LOCK()

#define DEBUG_VALIDATE()
//#define DEBUG_LOCK_HEAP()
//#define DEBUG_UNLOCK_HEAP()
#define DEBUG_FREE_HEAP()

#define realfree(a) free(a)

#endif

/** Static class to replace the C API. */
class Debug
{
private:
	inline Debug() {}							//< Forbid construction
	inline Debug(const Debug& d) {}				//< Forbid construction
	inline Debug& operator =(const Debug& d) { return *this; }	//< Forbig copying

public:

	/** @brief Assert condition is true.  You may want to use the ASSERT macro instead, since it will report file and line number. */
	inline static void Assert(bool cond)
	{
		ASSERT(cond);
	}

	/** @brief Check that the pointer points to heap memory. 
	 *	@ref AssertMem
	 *	Usually results in a heap scan, so performance can be poor.
	 */
	inline static void AssertPtr(const void *vp)
	{
		ASSERT_PTR(vp);
	}

	/** @brief Check that the pointer points to the start of a memory block of size size and validate the memory block (check for over/under writes). 
	 *	@ref AssertPtr
	 */
	inline static void AssertMem(const void *vp, const int size)
	{
		ASSERT_MEM(vp, size);
	}

	/** @brief Destroy the debug heap. 
	 *	@param reallyFree If true, delete any memory blocks on the debug heap.  Set to false if your program has static memory allocations.
	 *	Do not allocate, delete, or call debug functions after this.
	 */
	inline static void TearDown(bool reallyFree)
	{
		DEBUG_TEAR_DOWN(reallyFree);
	}

	/** @brief Delete all memory allocations.  Should only be used to reset the environment in unit tests. */
	inline static void FreeDebugHeap()
	{
		DEBUG_FREE_HEAP();
	}

	/** @brief Enable heap locking with HeapLock() and HeapUnlock. 
	 *	This may help with some multi threaded scenerios, but deadlocks in memory and pointer checks are still possible.
	 */
	inline static void EnableHeapLocking()
	{
		DEBUG_ENABLE_HEAP_LOCK();
	}

	/** @brief Lock the debug heap mutex.
	 *	@ref Mutex
	 *	@ref EnableHeapLocking
	 *	@ref UnlockHeap
	 */
	//inline static void HeapLock()
	//{
	//	DEBUG_LOCK_HEAP();
	//}

	/** @brief Unlock the debug heap mutex.
	 *	@ref Mutex
	 *	@ref EnableHeapLocking
	 *	@ref LockHeap
	 */
	//inline static void UnlockHeap()
	//{
	//	DEBUG_UNLOCK_HEAP();
	//}

	/** @brief Clear flags set with NoteMem
	 *	@ref NoteMem
	 *	@ref PrintMemoryLeaks
	 *	@ref AssertMemNoted
	 */
	inline void ClearMemCheckPoints()
	{
		DEBUG_CLEAR_MEM_CHECK_POINTS();
	}

	/** @brief printf any memory blocks that have not been marked with NoteMem.
	 *	@ref NoteMem
	 *	@ref ClearMemCheckPoints
	 *	@ref AssertMemNoted
	 */
	inline static void PrintMemoryLeaks()
	{
		DEBUG_DUMP_MEM_LEAKS();
	}

	/** @brief 
	 *	@ref PrintMemoryLeaks
	 *	@ref ClearMemCheckPoints
	 *	@ref AssertMemNoted
	 */
	inline static void NoteMem(const void *vp)
	{
		DEBUG_NOTE_MEM(vp);
	}

	/** @brief Check if NoteMem was called for all allocations.
	 *	@ref NoteMem
	 */
	inline static void AssertMemNoted()
	{
		DEBUG_ASSERT_MEM_NOTED();
	}

	/** Assert that all mem is deallocated. */
	inline static void AssertMemFree()
	{
		ASSERT_MEM_FREE();
	}

	/** @brief Assertion for unit tests, prints messages to Log and does not do a hard assert. 
	 *	@ref Assert
	 */
	inline static void UnitAssert(const char *msg, bool cond)
	{
		UNIT_ASSERT(msg, cond);
	}

	/** @brief Check that the pointer points to heap memory, prints messages to Log and does not do a hard assert. 
	 *	@ref AssertPtr
	 *	Usually results in a heap scan, so performance can be poor.
	 */
	inline static void UnitAssertPtr(const char *msg, const void *vp)
	{
		UNIT_ASSERT_PTR(msg, vp);
	}

	/** @brief Check if NoteMem was called for all allocations. 
	 *	@ref AssertMem
	 */
	inline static void UnitAssertMem(const char *msg, const void *vp, const int size)
	{
		UNIT_ASSERT_MEM(msg, vp, size);
	}

	/** @brief Check that the pointer points to the start of a memory block of size size and validate the memory block (check for over/under writes). 
	 *	@ref AssertMemNoted
	 */
	inline static void UnitAssertMemNoted(const char *msg)
	{
		UNIT_ASSERT_MEM_FREE(msg);
	}

	/** @brief Assert that all mem is deallocated. 
	 *	@ref AssertMemFree
	 */
	inline static void UnitAssertMemFree(const char *msg)
	{
		UNIT_ASSERT_MEM_FREE(msg);
	}
};

/** @} */

#endif 

