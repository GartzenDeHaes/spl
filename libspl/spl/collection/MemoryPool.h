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
 /** @file DelimitedFile.cpp
 *  @brief Started as a port of http://www.codeproject.com/KB/cpp/MemoryPool.aspx, but that was too buggy.
 */
#ifndef _memorypool_h
#define _memorypool_h

#include <spl/types.h>
#include <spl/Debug.h>
#include <spl/collection/Array.h>
#include <spl/Memory.h>

namespace spl
{
/** 
 * @defgroup collection Collections
 * @{
 */

/** @brief Memory Chunk Struct
 *
 * This struct will hold (and manage) the actual allocated Memory.
 * Every MemoryChunk will point to a MemoryBlock, and another SMemoryChunk,
 * thus creating a linked-list of MemoryChunks.
 */
typedef struct MemoryBlock
{
	int32 m_blockId;		//!< Unique ID for the block placed in the allocated data
	byte* m_data;			//!< The actual Data
	int m_size;				//!< Size of the "Data"-Block
	int m_freeSize;			//!< Amount of memory freed.  If equal to size, m_pos will be reset to zero.
	int m_pos;				//!< actual used Size
	MemoryBlock *m_next;	//!< Pointer to the Next MemoryChunk in the List (may be NULL)
	int m_checkBits;
} MemoryBlock;

static const int DEFAULT_MEMORY_POOL_BLOCK_SIZE = 1024 ;	//!< Initial MemoryPool size (in Bytes)

/** @brief An application heap.
 *
 * This class is the actual implementation of the IMemoryBlock - Interface.
 * It is responsible for all MemoryRequests (GetMemory() / FreeMemory()) and
 * manages the allocation/deallocation of Memory from the Operating-System.
 */
class MemoryPool : public IMemoryValidate
{
private:
	// Copy constructor doesn't make sense for this class
	inline MemoryPool(const MemoryPool& mp) {}
	inline void operator =(const MemoryPool& mp) {}

    MemoryBlock m_head;  //!< Pointer to the first Chunk in the Linked-List of Memory Chunks

    int m_totalMemoryPoolSize ;  //!< Total Memory-Pool size in Bytes
    int m_usedMemoryPoolSize ;   //!< amount of used Memory in Bytes
    int m_freeMemoryPoolSize ;   //!< amount of free Memory in Bytes

    int m_memoryBlockSize ;     //!< amount of Memory which can be Managed by a single MemoryChunk.
    int m_memoryBlockCount ;  //!< Total amount of "SMemoryChunk"-Objects in the Memory-Pool.

	bool m_setMemoryData ;                      //!< Set to "true", if you want to set all (de)allocated Memory to a predefined Value (via "memset()"). Usefull for debugging.

	MemoryBlock *AllocateBlock(int size);

public:
    /*!
	Contructor
	\param [IN] blockSize The Size (in Bytes) each MemoryChunk can Manage. 
	            A low sMemoryChunkSize increases the MemoryPool runtime (bad), but decreases the Memory-overhead/fragmentation (good)
	\param [IN] bSetMemoryData Set to true, if you want to set all allocated/freed Memory to a specific
	            Value. Very usefull for debugging, but has a negativ impact on the runtime.
	*/
    MemoryPool(const int blockSize = DEFAULT_MEMORY_POOL_BLOCK_SIZE, bool bSetMemoryData = false);    
	virtual ~MemoryPool();

	/*!
	  Get "sMemorySize" Bytes from the Memory Pool.
	  \param [IN] sMemorySize       Sizes (in Bytes) of Memory.
	  \return                       Pointer to a Memory-Block of "sMemorySize" Bytes, or NULL if an error occured. 
	*/
    void *Malloc(int size) ;
    
	/*!
	  Free the allocated memory again....
	  \param [IN] ptrMemoryBlock    Pointer to a Block of Memory, which is to be freed (previoulsy allocated via "GetMemory()").
	  \param [IN] sMemorySize       Sizes (in Bytes) of Memory.
	*/
	void Free(void *ptr);

	/*!
	  Writes the contents of the MemoryPool to a File.
	  Note : This file can be quite large (several MB).
	  \param [IN] strFileName      FileName of the MemoryDump.
	  \return                      true on success, false otherwise 
	*/
    bool WriteMemoryDumpToFile(const char *fileName) const;

	/*!
	  Check, if a Pointer is in the Memory-Pool.
	  Note : This Checks only if a pointer is inside the Memory-Pool,
	         and <b>not</b> if the Memory contains meaningfull data.
	  \param [IN] ptrPointer       Pointer to a Memory-Block which is to be checked.
	  \return                      true, if the Pointer could be found in the Memory-Pool, false otherwise.
	*/
	bool IsValidPointer(const void *ptr) const;
 
#ifdef DEBUG
	virtual void ValidateMem () const;
	virtual void CheckMem () const;
#endif
};

REGISTER_TYPEOF( 400, MemoryPool );

/** @} */
}

void *operator new (size_t size, spl::MemoryPool& pool);
void operator delete (void *ptr, spl::MemoryPool& pool);

#endif
