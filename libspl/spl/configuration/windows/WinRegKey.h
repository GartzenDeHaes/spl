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
#ifndef _winregkey_h
#define _winregkey_h

#include <spl/types.h>
#include <spl/collection/List.h>
#include <spl/net/Socket.h>		// Ensures a full windows include chain
#include <spl/String.h>
#include <spl/Memory.h>

namespace spl
{
class WinRegKey : public IMemoryValidate
{
public:
	typedef enum _KeyHive
	{
		HiveUnknownError = 0,
		HiveClassesRoot = (int)HKEY_CLASSES_ROOT,
		HiveCurrentuUser = (int)HKEY_CURRENT_USER,
		HiveLocalMachine = (int)HKEY_LOCAL_MACHINE,
		HiveUsers = (int)HKEY_USERS
	} KeyHive;
	
private:
	KeyHive m_hive;
	HKEY m_hkey;
	String m_path;
	bool m_isOpen;
	
	void Open(bool isUpdatable);
	
public:
	WinRegKey();
	WinRegKey(const WinRegKey& wrk);
	WinRegKey(const String& pathAndKey, KeyHive hive = HiveLocalMachine);
	virtual ~WinRegKey();
	
	String KeyName() const;
	inline const String& KeyNameAndPath() const { return m_path; }
	inline const bool IsOpen() const { return m_isOpen; }
	inline const KeyHive Hive() const { return m_hive; }
	
	void OpenForRead();
	void OpenForReadWrite();
	void Close();
	
	void DeleteSubKey(const String& keyName);
	void DeleteSubTree(const String& keyName);
	void DeleteValue(const String& name);
	
	void Set(int i);
	void Set(const String& val);
	void SetSubKey(const String&key, const String& val);
	
	List<WinRegKey> ListSubKeys();
	List<String> ListSubValueNames();
	
	String ValueString(const String& name);
	int ValueInt(const String& name);
	
	static WinRegKey Create(KeyHive hive, const String& pathAndKey);
	static bool KeyExists(KeyHive hive, const String& pathAndKey);
	
#ifdef DEBUG
	void ValidateMem() const;
	void CheckMem() const;
#endif
};
}
#endif
