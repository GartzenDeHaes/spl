/* Ported to SPL, original attribution below. */
/* ---------------------------------------------------------------------------
   commonc++ - A C++ Common Class Library
   Copyright (C) 2005-2009  Mark A Lindner

   This file was ported from commonc++.

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
#include <spl/Environment.h>
#include <spl/io/Directory.h>
#include <spl/threading/Semaphore.h>
#include <spl/io/WinPerm.h>

using namespace spl;

#ifndef _TANDEM

#ifdef HAVE_SEMAPHORE_H
#include <semaphore.h>
#endif

#ifdef _WINDOWS
extern bool EncodePermissions(const Permissions& perm, _WinPerms &wperm);
#else
extern void EncodePermissions(const Permissions& perm, mode_t& mode);
#endif

//#ifndef _WINDOWS
//static bool VerifyIPCName(const String& name)
//{
//  return((name.Length() <= 14) && (name.IndexOf(Directory::SeperatorChar(), 0) < 0));
//}
//#endif

Semaphore::Semaphore
(
	uint32 value			/* = 1 */,
    const Permissions& perm	/* = Permissions::USER_READ_WRITE */
)
{
	if (value < 1)
	{
		throw new InvalidArgumentException("Invalid value");
	}

#ifdef _WINDOWS

	SECURITY_ATTRIBUTES sa;
	_WinPerms wperm;

	EncodePermissions(perm, wperm);

	sa.nLength = sizeof(SECURITY_ATTRIBUTES);
	sa.lpSecurityDescriptor = wperm.pdesc;
	sa.bInheritHandle = FALSE;

	_sem = ::CreateSemaphore(&sa, 0, value, NULL);
	if(_sem == NULL)
	{
		throw new Exception("Can't create semaphore " + Environment::LastErrorMessage());
	}

	// make all instances immediately available
	::ReleaseSemaphore(_sem, value, NULL);

#else

	//if(! VerifyIPCName(name))
	//{
	//	throw new InvalidArgumentException("Invalid IPC name");
	//}

	if (0 != sem_init(&_sem, 0, value))
	{
		throw new Exception("Can't init semaphore " + Environment::LastErrorMessage());
	}

#endif
}

Semaphore::~Semaphore()
{
#ifdef _WINDOWS

  if(_sem != NULL)
    ::CloseHandle(_sem);

#else

    sem_destroy(&_sem);

#endif
}

void Semaphore::Lock()
{
#ifdef _WINDOWS

  if(::WaitForSingleObjectEx(_sem, INFINITE, TRUE) != WAIT_OBJECT_0)
  {
	  throw new StateException("Error locking semaphore");
  }

#else

  if(::sem_wait(&_sem) != 0)
  {
	  throw new StateException("Error locking semaphore");
  }

#endif
}

bool Semaphore::TryWait()
{
#ifdef _WINDOWS

	return(::WaitForSingleObjectEx(_sem, 0, TRUE) == WAIT_OBJECT_0);

#else

	return(::sem_trywait(&_sem) == 0);

#endif
}

void Semaphore::Unlock()
{
#ifdef _WINDOWS

  if(::ReleaseSemaphore(_sem, 1, NULL) != TRUE)
  {
	  throw new StateException("Error unlocking semaphore");
  }

#else

  if(::sem_post(&_sem) != 0)
  {
	  throw new StateException("Error unlocking semaphore")	 ; 
  }

#endif
}

#if defined(DEBUG) || defined(_DEBUG)
void Semaphore::CheckMem() const 
{
}

void Semaphore::ValidateMem() const 
{
}
#endif

#endif
