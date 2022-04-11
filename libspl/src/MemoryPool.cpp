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
#include <spl/Exception.h>
#include <spl/io/File.h>
#include <spl/math/Math.h>
#include <spl/collection/MemoryPool.h>

using namespace spl;

static const int FREEED_MEMORY_CONTENT        = 0xAA ; //!< Value for freed memory  
static const int NEW_ALLOCATED_MEMORY_CONTENT = 0xFF ; //!< Initial Value for new allocated memory 
static const int MP_MAJIC = 27013;

MemoryPool::MemoryPool(const int blockSize, bool setMemoryData)
: m_totalMemoryPoolSize(0),
  m_usedMemoryPoolSize(0),
  m_freeMemoryPoolSize(0),
  m_memoryBlockSize(blockSize),
  m_memoryBlockCount(0),
  m_setMemoryData(setMemoryData)
{
	m_head.m_data = NULL;
	m_head.m_size = 0;
	m_head.m_pos = 0;
	m_head.m_next = NULL;
	m_head.m_checkBits = MP_MAJIC;
}

MemoryPool::~MemoryPool()
{
	ASSERT(m_head.m_checkBits == MP_MAJIC);

	MemoryBlock *mb = m_head.m_next;
	while ( NULL != mb )
	{
		ASSERT(mb->m_checkBits == MP_MAJIC);
		ASSERT_MEM(mb, sizeof(MemoryBlock));
		ASSERT_MEM(mb->m_data, mb->m_size);
		
		MemoryBlock *next = mb->m_next;
		free(mb->m_data);
		delete mb;
		mb = next;
	}
	m_head.m_next = NULL;
}

void *MemoryPool::Malloc(int size)
{
	ASSERT(m_head.m_checkBits == MP_MAJIC);

	m_freeMemoryPoolSize -= size;
	m_usedMemoryPoolSize += size;

	// add space for the block ID and size
	size += 8;

	MemoryBlock *mb;

	if ( size > m_memoryBlockSize )
	{
		// For large allocations, just make a special block
		mb = AllocateBlock(size);
	}
	else
	{
		mb = m_head.m_next;
		while ( NULL != mb )
		{
			if ( mb->m_size - mb->m_pos > size )
			{
				break;
			}
			mb = mb->m_next;
		}
		if ( NULL == mb )
		{
			mb = AllocateBlock(m_memoryBlockSize);
		}
	}
	void *data = &mb->m_data[mb->m_pos];
	*((int32 *)data) = mb->m_blockId;
	data = &((byte *)data)[4];
	*((int32 *)data) = size;
	data = &((byte *)data)[4];

	mb->m_pos += size;
	ASSERT( mb->m_pos <= mb->m_size );
	return data;
}

void MemoryPool::Free(void *ptr)
{
	ASSERT(m_head.m_checkBits == MP_MAJIC);

	int blockId = *((int32 *)(((byte *)ptr) - 8));
	int size = *((int32 *)(((byte *)ptr) - 4));

	MemoryBlock *mb = m_head.m_next;
	while ( NULL != mb )
	{
		if ( mb->m_blockId == blockId )
		{
			ASSERT( size <= mb->m_size );
			mb->m_freeSize += size;
			ASSERT( mb->m_freeSize <= mb->m_size );
			if ( mb->m_freeSize == mb->m_size )
			{
				mb->m_freeSize = 0;
				mb->m_pos = 0;
			}
			return;
		}
		mb = mb->m_next;
	}
	throw new Exception("MemoryPool::Free: pointer not found");
}

bool MemoryPool::WriteMemoryDumpToFile(const char *fileName) const
{
	spl::IStreamPtr writer = File::OpenWrite(fileName);
	
	MemoryBlock *mb = m_head.m_next;
	while ( NULL != mb )
	{
		Array<byte> data(mb->m_data, mb->m_size);
		writer->Write(data);
		mb = mb->m_next;
	}
	writer->Close();

	return true;
}

bool MemoryPool::IsValidPointer(const void *ptr) const
{
	ASSERT(m_head.m_checkBits == MP_MAJIC);

	int blockId = *((int32 *)(((byte *)ptr) - 8));
	int size = *((int32 *)(((byte *)ptr) - 4));

	MemoryBlock *mb = m_head.m_next;
	while ( NULL != mb )
	{
		if ( mb->m_blockId == blockId )
		{
			ASSERT( size <= mb->m_size );
			return true;;
		}
		mb = mb->m_next;
	}
	return false;
}

MemoryBlock *MemoryPool::AllocateBlock(int size)
{
	MemoryBlock *mb = (MemoryBlock *)malloc(sizeof(MemoryBlock));
	if ( NULL == mb )
	{
		throw OutOfMemoryException();
	}
	memset(mb, 0, sizeof(MemoryBlock));
	mb->m_checkBits = MP_MAJIC;
	if (NULL == (mb->m_data = (byte *)malloc(size)))
	{
		throw OutOfMemoryException();
	}

	if ( m_setMemoryData )
	{
		memset(mb->m_data, NEW_ALLOCATED_MEMORY_CONTENT, size);
	}

	mb->m_blockId = m_memoryBlockCount++;
	mb->m_size = size;
	mb->m_next = m_head.m_next;
	m_head.m_next = mb;

	m_totalMemoryPoolSize += size;
	m_freeMemoryPoolSize += size;

	return mb;
}

#ifdef DEBUG
void MemoryPool::ValidateMem () const
{
	ASSERT(m_head.m_checkBits == MP_MAJIC);

	MemoryBlock *mb = m_head.m_next;
	while ( NULL != mb )
	{
		ASSERT(mb->m_checkBits == MP_MAJIC);
		ASSERT_MEM(mb, sizeof(MemoryBlock));
		ASSERT_MEM(mb->m_data, mb->m_size);
		mb = mb->m_next;
	}
}

void MemoryPool::CheckMem () const
{
	ASSERT(m_head.m_checkBits == MP_MAJIC);

	MemoryBlock *mb = m_head.m_next;
	while ( NULL != mb )
	{
		ASSERT(mb->m_checkBits == MP_MAJIC);
		DEBUG_NOTE_MEM(mb);
		DEBUG_NOTE_MEM(mb->m_data);

		mb = mb->m_next;
	}
}
#endif

void *operator new (size_t size, MemoryPool& pool)
{
	return pool.Malloc((int)size);
}

void operator delete (void *ptr, MemoryPool& pool)
{
	pool.Free(ptr);
}
