/* Ported to SPL, original attribution below. */
/* ---------------------------------------------------------------------------
   commonc++ - A C++ Common Class Library
   Copyright (C) 2005-2009  Mark A Lindner

   Ported from commonc++.

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
#include <spl/io/Permissions.h>
#include <spl/text/StringBuffer.h>
#include <spl/io/WinPerm.h>

#ifdef _WINDOWS

#include <Tchar.h>
#include <Lmcons.h>
#include <aclapi.h>
#include <accctrl.h>

#define S_IRUSR 0400
#define S_IWUSR 0200
#define S_IXUSR 0100
#define S_IRGRP 0040
#define S_IWGRP 0020
#define S_IXGRP 0010
#define S_IROTH 0004
#define S_IWOTH 0002
#define S_IXOTH 0001

#ifndef SECURITY_MAX_SID_SIZE
#define SECURITY_MAX_SID_SIZE 68
#endif

#else

#include <sys/stat.h>

#endif

using namespace spl;

const int Permissions::USER_READ = S_IRUSR,
  Permissions::USER_WRITE = S_IWUSR,
  Permissions::USER_READ_WRITE = (S_IRUSR | S_IWUSR),
  Permissions::USER_EXECUTE = S_IXUSR,
  Permissions::USER_ALL = (S_IRUSR | S_IWUSR | S_IXUSR),
  Permissions::GROUP_READ = S_IRGRP,
  Permissions::GROUP_WRITE = S_IWGRP,
  Permissions::GROUP_READ_WRITE = (S_IRGRP | S_IWGRP),
  Permissions::GROUP_EXECUTE = S_IXGRP,
  Permissions::GROUP_ALL = (S_IRGRP | S_IWGRP | S_IXGRP),
  Permissions::OTHERS_READ = S_IROTH,
  Permissions::OTHERS_WRITE = S_IWOTH,
  Permissions::OTHERS_READ_WRITE = (S_IROTH | S_IWOTH),
  Permissions::OTHERS_EXECUTE = S_IXOTH,
  Permissions::OTHERS_ALL = (S_IROTH | S_IWOTH | S_IXOTH),
  Permissions::ALL_READ_WRITE = (S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP
                                 | S_IROTH | S_IWOTH),
  Permissions::ALLBITS = (USER_ALL | GROUP_ALL | OTHERS_ALL);

/*
 */

const Permissions Permissions::DefaultFilePerms(Permissions::USER_ALL
                                                | Permissions::GROUP_READ);

const Permissions Permissions::DefaultDirPerms(Permissions::USER_ALL
                                               | Permissions::GROUP_READ
                                               | Permissions::GROUP_EXECUTE);

/*
 */

Permissions::Permissions(uint32 mask /* = 0 */) throw()
  : _mask(mask & ALLBITS)
{
}

/*
 */

Permissions::Permissions(const Permissions& other) throw()
  : _mask(other._mask)
{
}

/*
 */

Permissions::~Permissions() throw()
{
}

/*
 */

Permissions& Permissions::operator=(const Permissions& other) throw()
{
  _mask = other._mask;
  return(*this);
}

/*
 */

StringPtr Permissions::ToString() const
{
  StringBuffer s;
  s.Append("---------");

  for(int i = 2, x = 0; i >=0; --i)
  {
    unsigned int b = ((_mask >> (i * 3)) & 0x07);

    if(b & 4)
      s.SetCharAt(x, 'r');
    ++x;

    if(b & 2)
      s.SetCharAt(x, 'w');
    ++x;

    if(b & 1)
      s.SetCharAt(x, 'x');
    ++x;
  }

  return s.ToString();
}

#ifdef _WINDOWS

#define LUSIZE 1024
#define ACCT_NAME_SIZE LUSIZE
#define ACL_SIZE 1024
#define SID_SIZE LUSIZE  /* See support.h */
#define DOM_SIZE LUSIZE

// The commonc++ guy got this from http://fmgroup.polito.it/murciano/teachings/sp/CHAPTR15/InitUnFp.c
bool FindPrimaryGroup(LPTSTR name)
{
  /* Experimentation shows that the groups entered are as follows:
     0        -      None
     1        -      Everyone
     2        -      The first non-trivial group
     3...     -  Keep looking up to the count, which is part
     of the structure - see the documentation! */

	TCHAR RefDomain[DOM_SIZE];
	DWORD RefDomCnt = DOM_SIZE;
	SID_NAME_USE nameuse  = SidTypeGroup;
	HANDLE tHandle;
	TOKEN_GROUPS TokenG[512]; /* You need some space for this. */
	DWORD TISize, AcctSize = UNLEN + 1;

	if(! ::OpenProcessToken(::GetCurrentProcess(), TOKEN_ALL_ACCESS, &tHandle))
		return false;

	if(! ::GetTokenInformation(tHandle, TokenGroups, &TokenG, sizeof(TokenG), &TISize))
	{
		::CloseHandle(tHandle);
		return false;
	}

	if(! ::LookupAccountSid(NULL, TokenG[0].Groups[2].Sid, name, &AcctSize, RefDomain, &RefDomCnt, &nameuse))
	{
		::CloseHandle(tHandle);
		return false;
	}

	::CloseHandle(tHandle);
	return true;
}

// sets wperm from perm
bool EncodePermissions(const Permissions& perm, _WinPerms &wperm)
{
	if (NULL != wperm.pdesc)
	{
		::LocalFree(wperm.pdesc);
		wperm.pdesc = NULL;
	}

	static uint32 masks[9] = { Permissions::USER_READ,
							 Permissions::USER_WRITE,
							 Permissions::USER_EXECUTE,
							 Permissions::GROUP_READ,
							 Permissions::GROUP_WRITE,
							 Permissions::GROUP_EXECUTE,
							 Permissions::OTHERS_READ,
							 Permissions::OTHERS_WRITE,
							 Permissions::OTHERS_EXECUTE };

	static int types[3] = { (FILE_READ_DATA | READ_CONTROL
						   | FILE_READ_ATTRIBUTES | FILE_READ_EA),

						  (FILE_WRITE_DATA | FILE_APPEND_DATA
						   | FILE_WRITE_ATTRIBUTES | FILE_WRITE_EA
						   | DELETE | FILE_DELETE_CHILD
						   | WRITE_DAC | WRITE_OWNER),

						  FILE_EXECUTE };

	///BUG: Doesn't work?
	
	bool status = false;
	int i;

	wperm.pdesc = (PSECURITY_DESCRIPTOR)::LocalAlloc(0, sizeof(SECURITY_DESCRIPTOR));

	if(! ::InitializeSecurityDescriptor(wperm.pdesc, SECURITY_DESCRIPTOR_REVISION))
		return false;

	PACL pAcl = NULL;
	LPTSTR GrpNms[3] = { "", "", "Everyone" };
	PSID sids[3] = { NULL, NULL, NULL };
	SID_NAME_USE nameuse [] = { SidTypeUser, SidTypeGroup, SidTypeWellKnownGroup };
	TCHAR RefDomain[3][DOM_SIZE];
	TCHAR user[UNLEN + 1];
	DWORD RefDomSize[3] = { DOM_SIZE, DOM_SIZE, DOM_SIZE };
	DWORD sidSize[3] = { SECURITY_MAX_SID_SIZE, SECURITY_MAX_SID_SIZE,
						SECURITY_MAX_SID_SIZE };
	DWORD sz = UNLEN + 1;

	if(! ::GetUserName(user, &sz))
		return false;

	GrpNms[0] = user;

	// Get the user's primary group. 

	GrpNms[1] = (LPTSTR)::HeapAlloc(wperm.heap, 0, UNLEN + 1);
	if(GrpNms[1] == NULL)
		return false;

	GrpNms[1][0] = 0;
	if (!FindPrimaryGroup(GrpNms[1]))
	{
		return false;
	}

	for(i = 0; i < 3; i++)
	{
		sids[i] = ::HeapAlloc(wperm.heap, 0, SECURITY_MAX_SID_SIZE);
		if(sids[i] == NULL)
			return false;

		if(! ::LookupAccountName(NULL, GrpNms[i], sids[i], &sidSize[i],
								 RefDomain[i], &RefDomSize[i], &nameuse[i]))
		return false;
	}

	pAcl = (PACL)::HeapAlloc(wperm.heap, 0, ACL_SIZE);
	if(pAcl == NULL)
		return false;

	if(! ::InitializeAcl(pAcl, ACL_SIZE, ACL_REVISION))
		return false;

	// Add all the ACEs. Scan the permission bits, adding an allowed ACE when
	// the bit is set and a denied ACE when the bit is reset.
	for(i = 0; i < 9; i++)
	{
		bool f = 0 != (perm & masks[i]);
		bool ok;

		if(f)
			ok = 0 != ::AddAccessAllowedAce(pAcl, ACL_REVISION, types[i % 3], sids[i / 3]);
		else
			ok = 0 != ::AddAccessDeniedAce(pAcl, ACL_REVISION, types[i % 3], sids[i / 3]);

		if(! ok)
			return false;
	}

	// Add a final deny all to everyone ACE

	if(! ::AddAccessDeniedAce(pAcl, ACL_REVISION,
							STANDARD_RIGHTS_ALL | SPECIFIC_RIGHTS_ALL,
							sids[2]))
		return false;

	// The ACL is now complete. Associate it with the security descriptor.

	if(! ::SetSecurityDescriptorDacl(wperm.pdesc, TRUE, pAcl, FALSE))
		return false;

	if(! ::IsValidSecurityDescriptor(wperm.pdesc))
		return false;

	return true;
}

// sets perm from wperm
bool DecodePermissions(_WinPerms &wperm, Permissions &perm)
{
	PSID _worldSID = NULL;
	TRUSTEE ident = { NULL, NO_MULTIPLE_TRUSTEE, TRUSTEE_IS_SID };
	ACCESS_MASK mask;

	// user

	ident.TrusteeType = TRUSTEE_IS_USER;
	ident.ptstrName = (LPTSTR)wperm.user;

	if(::GetEffectiveRightsFromAcl(wperm.dacl, &ident, &mask) != ERROR_SUCCESS)
	{
		return false;
	}

	(mask & FILE_READ_DATA) ? perm.SetUserRead() : perm.ClearUserRead();
	(mask & FILE_WRITE_DATA) ? perm.SetUserWrite() : perm.ClearUserWrite();
	(mask & FILE_EXECUTE) ? perm.SetUserExecute() : perm.ClearUserExecute();

	// group

	ident.TrusteeType = TRUSTEE_IS_GROUP;
	ident.ptstrName = (LPTSTR)wperm.group;

	if(::GetEffectiveRightsFromAcl(wperm.dacl, &ident, &mask) != ERROR_SUCCESS)
	{
		return false;
	}

	(mask & FILE_READ_DATA) ? perm.SetGroupRead() : perm.ClearGroupRead();
	(mask & FILE_WRITE_DATA) ? perm.SetGroupWrite() : perm.ClearGroupWrite();
	(mask & FILE_EXECUTE) ? perm.SetGroupExecute() : perm.ClearGroupExecute();

	// world
	SID_IDENTIFIER_AUTHORITY sidAuth = SECURITY_WORLD_SID_AUTHORITY;
	::AllocateAndInitializeSid(&sidAuth, 1, SECURITY_WORLD_RID, 0, 0, 0, 0, 0, 0, 0, &_worldSID);

	ident.TrusteeType = TRUSTEE_IS_WELL_KNOWN_GROUP;
	ident.ptstrName = (LPTSTR)_worldSID;

	if(::GetEffectiveRightsFromAcl(wperm.dacl, &ident, &mask) != ERROR_SUCCESS)
	{
	    ::FreeSid(_worldSID);
		return false;
	}
    ::FreeSid(_worldSID);

	(mask & FILE_READ_DATA) ? perm.SetOthersRead() : perm.ClearOthersRead();
	(mask & FILE_WRITE_DATA) ? perm.SetOthersWrite() : perm.ClearOthersWrite();
	(mask & FILE_EXECUTE) ? perm.SetOthersExecute() : perm.ClearOthersExecute();

	return true;
}

#else

void EncodePermissions(const Permissions& perm, mode_t& mode)
{
	mode = (mode_t)perm.GetMask();
}


void DecodePermissions(const mode_t mode, Permissions& perm)
{
	perm = (uint32)(mode & 0777);
}

#endif

/* end of source file */
