/*
 *   Ported to SPL, original attribution below.
 */
 /* ---------------------------------------------------------------------------
   commonc++ - A C++ Common Class Library
   Copyright (C) 2005-2009  Mark A Lindner

   Ported from the commonc++ project.

   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Library General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public
   License along with this library; if not, write to the Free
   Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
   ---------------------------------------------------------------------------
*/

#ifndef __ccxx_Permissions_hxx
#define __ccxx_Permissions_hxx

#include <spl/types.h>
#include <spl/Debug.h>
#include <spl/String.h>

namespace spl
{
/** 
 * @defgroup file File System
 * @ingroup io
 * @{
 */

/** File permissions. Permissions consist of <i>read</i>, <i>write</i>,
* and <i>execute</i> access for each of <i>user</i> (owner), <i>group</i>,
* and <i>others</i> (everyone), and are represented efficiently as a mask.
*
* @author Mark Lindner.
*/
class Permissions
{
private:

	uint32 _mask;

public:

	/** Construct a new Permissions object with the given mask.
	*
	* @param mask The permissions mask.
	*/
	Permissions(uint32 mask = 0) throw();

	/** Copy constructor. */

	Permissions(const Permissions& other) throw();

	/** Destructor. */

	~Permissions() throw();

	/** User read mask. */
	static const int USER_READ;

	/** User write mask. */
	static const int USER_WRITE;

	/** User read and write mask. */
	static const int USER_READ_WRITE;

	/** User execute mask. */
	static const int USER_EXECUTE;

	/** User (all permissions) mask. */
	static const int USER_ALL;

	/** Group read mask. */
	static const int GROUP_READ;

	/** Group write mask. */
	static const int GROUP_WRITE;

	/** Group read and write mask. */
	static const int GROUP_READ_WRITE;

	/** Group execute mask. */
	static const int GROUP_EXECUTE;

	/** Group (all permissions) mask. */
	static const int GROUP_ALL;

	/** Others read mask. */
	static const int OTHERS_READ;

	/** Others write mask. */
	static const int OTHERS_WRITE;

	/** Others read and write mask. */
	static const int OTHERS_READ_WRITE;

	/** Others execute mask. */
	static const int OTHERS_EXECUTE;

	/** Others (all permissions) mask. */
	static const int OTHERS_ALL;

	/** All read and write mask. */
	static const int ALL_READ_WRITE;

	/** Complete permissions mask. */
	static const int ALLBITS;

	// user

	/** Test for <i>user read</i> access. */
	inline bool CanUserRead() const throw()
	{ return((_mask & USER_READ) != 0); }

	/** Enable <i>user read</i> access. */
	inline void SetUserRead() throw()
	{ _mask |= USER_READ; }

	/** Disable <i>user read</i> access. */
	inline void ClearUserRead() throw()
	{ _mask &= ~USER_READ; }

	/** Test for <i>user write</i> access. */
	inline bool CanUserWrite() const throw()
	{ return((_mask & USER_WRITE) != 0); }

	/** Enable <i>user write</i> access. */
	inline void SetUserWrite() throw()
	{ _mask |= USER_WRITE; }

	/** Disable <i>user write</i> access. */
	inline void ClearUserWrite() throw()
	{ _mask &= ~USER_WRITE; }

	/** Test for <i>user execute</i> access. */
	inline bool CanUserExecute() const throw()
	{ return((_mask & USER_EXECUTE) != 0); }

	/** Enable <i>user execute</i> access. */
	inline void SetUserExecute() throw()
	{ _mask |= USER_EXECUTE; }

	/** Disable <i>user execute</i> access. */
	inline void ClearUserExecute() throw()
	{ _mask &= ~USER_EXECUTE; }

	// group

	/** Test for <i>group read</i> access. */
	inline bool CanGroupRead() const throw()
	{ return((_mask & GROUP_READ) != 0); }

	/** Enable <i>group read</i> access. */
	inline void SetGroupRead() throw()
	{ _mask |= GROUP_READ; }

	/** Disable <i>group read</i> access. */
	inline void ClearGroupRead() throw()
	{ _mask &= ~GROUP_READ; }

	/** Test for <i>group write</i> access. */
	inline bool CanGroupWrite() const throw()
	{ return((_mask & GROUP_WRITE) != 0); }

	/** Enable <i>group write</i> access. */
	inline void SetGroupWrite() throw()
	{ _mask |= GROUP_WRITE; }

	/** Disable <i>group write</i> access. */
	inline void ClearGroupWrite() throw()
	{ _mask &= ~GROUP_WRITE; }

	/** Test for <i>group execute</i> access. */
	inline bool CanGroupExecute() const throw()
	{ return((_mask & GROUP_EXECUTE) != 0); }

	/** Enable <i>group execute</i> access. */
	inline void SetGroupExecute() throw()
	{ _mask |= GROUP_EXECUTE; }

	/** Disable <i>group execute</i> access. */
	inline void ClearGroupExecute() throw()
	{ _mask &= ~GROUP_EXECUTE; }

	// others

	/** Test for <i>others read</i> access. */
	inline bool CanOthersRead() const throw()
	{ return((_mask & OTHERS_READ) != 0); }

	/** Enable <i>others read</i> access. */
	inline void SetOthersRead() throw()
	{ _mask |= OTHERS_READ; }

	/** Disable <i>others read</i> access. */
	inline void ClearOthersRead() throw()
	{ _mask &= ~OTHERS_READ; }

	/** Test for <i>others write</i> access. */
	inline bool CanOthersWrite() const throw()
	{ return((_mask & OTHERS_WRITE) != 0); }

	/** Enable <i>others write</i> access. */
	inline void SetOthersWrite() throw()
	{ _mask |= OTHERS_WRITE; }

	/** Disable <i>others write</i> access. */
	inline void ClearOthersWrite() throw()
	{ _mask &= ~OTHERS_WRITE; }

	/** Test for <i>others execute</i> access. */
	inline bool CanOthersExecute() const throw()
	{ return((_mask & OTHERS_EXECUTE) != 0); }

	/** Enable <i>others execute</i> access. */
	inline void SetOthersExecute() throw()
	{ _mask |= OTHERS_EXECUTE; }

	/** Disable <i>others execute</i> access. */
	inline void ClearOthersExecute() throw()
	{ _mask &= ~OTHERS_EXECUTE; }

	/** Disable all access. */
	inline void ClearAll() throw()
	{ _mask = 0; }

	// operators

	Permissions& operator=(const Permissions& other) throw();

	/** Get the permissions mask. */

	inline uint32 GetMask() const throw()
	{ return(_mask); }

	/** Cast operator. Returns the permissions mask. */

	inline operator uint32() throw()
	{ return(_mask); }

	/** Get a String representation of the permissions mask.
	*/

	StringPtr ToString() const;

	inline uint32 operator=(uint32 rhs) throw()
	{ return((_mask = rhs)); }

	inline uint32 operator|=(uint32 rhs) throw()
	{ return(_mask |= (rhs & ALLBITS)); }

	inline uint32 operator&=(uint32 rhs) throw()
	{ return(_mask &= (rhs & ALLBITS)); }

	/** Default permissions for files: read & write for user, read for
	* group.
	*/

	static const Permissions DefaultFilePerms;

	/** Default permissions for directories: read, write & execute for user,
	* read & execute for group.
	*/

	static const Permissions DefaultDirPerms;
};

inline uint32 operator|(const Permissions& perm, const uint32 rhs) throw()
{ return(perm.GetMask() | (rhs & Permissions::ALLBITS)); }

inline uint32 operator&(const Permissions& perm, const uint32 rhs) throw()
{ return(perm.GetMask() & (rhs & Permissions::ALLBITS)); }

/** @} */
}
#endif // __ccxx_Permissions_hxx

/* end of header file */


