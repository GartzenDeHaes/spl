/* ---------------------------------------------------------------------------
   commonc++ - A C++ Common Class Library
   Copyright (C) 2005-2009  Mark A Lindner

   This file is part of commonc++.

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
#ifndef _winperm_h
#define _winperm_h

#if defined(WIN32) || defined(_WIN32) || defined(_WIN64)

#include <spl/cleanwindows.h>

/** 
 * @defgroup file File System
 * @ingroup io
 * @{
 */

/** @brief For internal use.
 */
class _WinPerms
{
public:

	_WinPerms()
	  : heap(NULL),
		pdesc(NULL),
		user(NULL),
		group(NULL),
		dacl(NULL)
	{
		heap = ::HeapCreate(0, 0, 0);
	}

	~_WinPerms()
	{
		if(pdesc != NULL)
			::LocalFree(pdesc);

		if(dacl != NULL)
			::LocalFree(dacl);

		if(heap != NULL)
			::HeapDestroy(heap);
	}

	HANDLE heap;
	PSECURITY_DESCRIPTOR pdesc;
	PSID user;
	PSID group;
	PACL dacl;
};

#endif

/** @} */

#endif
