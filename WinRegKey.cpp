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
#include <spl/configuration/windows/WinRegKey.h>

using namespace spl;

WinRegKey::WinRegKey()
: m_hive(HiveUnknownError), m_hkey(0), m_path(), m_isOpen(false)
{
}

WinRegKey::WinRegKey(const WinRegKey& wrk)
: m_hive(wrk.m_hive), m_hkey(wrk.m_hkey), m_path(wrk.m_path), m_isOpen(wrk.m_isOpen)
{
}

WinRegKey::WinRegKey(const String& pathAndKey, KeyHive hive)
: m_hive(hive), m_hkey(0), m_path(pathAndKey), m_isOpen(false)
{
}

WinRegKey::~WinRegKey()
{
	Close();
}

String WinRegKey::KeyName() const
{
	int pos;
	if ((pos = m_path.LastIndexOf("\\")) < 0)
	{
		return m_path;
	}
	return *m_path.Substring(pos + 1);
}

void WinRegKey::Open(bool forUpdate)
{
	if (m_isOpen)
	{
		return;
	}
	
	int flags = KEY_READ | KEY_ENUMERATE_SUB_KEYS;
	if (forUpdate)
	{
		flags |= KEY_WRITE | KEY_SET_VALUE | KEY_CREATE_SUB_KEY;
	}
	
	int ret = RegOpenKeyEx
	(
		(HKEY)m_hive, 
		m_path,
		0, 
		flags, 
		&m_hkey
	);
	
	if (ERROR_SUCCESS != ret)
	{
		throw new IOException("RegOpenKeyEx returned " + Int32::ToString(ret));
	}
	
	m_isOpen = true;
}

void WinRegKey::OpenForRead()
{
	Open(false);
}

void WinRegKey::OpenForReadWrite()
{
	Open(true);
}

void WinRegKey::Close()
{
	if (m_isOpen)
	{
		ASSERT(0 != m_hkey);
		RegCloseKey(m_hkey);
		m_hkey = 0;
		m_isOpen = false;
	}
}

void WinRegKey::DeleteSubKey(const String& keyName)
{
	bool needToClose = !m_isOpen;
	if (!m_isOpen)
	{
		Open(true);
	}
	
	int ret = RegDeleteKey(m_hkey, (LPCSTR)keyName.GetChars());
	
	if (needToClose)
	{
		Close();
	}
	
	if (ERROR_SUCCESS != ret)
	{
		throw new IOException("RegDeleteKey returned " + Int32::ToString(ret));
	}
}

void WinRegKey::DeleteValue(const String& name)
{
	bool needToClose = !m_isOpen;
	if (!m_isOpen)
	{
		Open(true);
	}
	
	int ret = RegDeleteValue(m_hkey, (LPCSTR)name.GetChars());
	
	if (needToClose)
	{
		Close();
	}
	
	if (ERROR_SUCCESS != ret)
	{
		throw new IOException("RegDeleteKey returned " + Int32::ToString(ret));
	}
}

void WinRegKey::DeleteSubTree(const String& keyName)
{
	bool needToClose = !m_isOpen;
	if (!m_isOpen)
	{
		Open(true);
	}
	
	int ret = S_OK;
	
#if _WIN32_WINNT > 0x0600 
	ret = RegDeleteTree(m_hkey, (LPCSTR)keyName.GetChars());
#else
	throw new NotSupportedException("Requires WIN6 or greater (VISTA)");
#endif

	if (needToClose)
	{
		Close();
	}
	
	if (ERROR_SUCCESS != ret)
	{
		throw new IOException("RegDeleteTree returned " + Int32::ToString(ret));
	}
}

void WinRegKey::Set(int i)
{
	bool needToClose = !m_isOpen;
	if (!m_isOpen)
	{
		Open(true);
	}
	
	int ret = RegSetValueEx(m_hkey, NULL, 0, REG_DWORD, (BYTE*)&i, sizeof(int));
	
	if (needToClose)
	{
		Close();
	}
	
	if (ERROR_SUCCESS != ret)
	{
		throw new IOException("RegSetValueEx returned " + Int32::ToString(ret));
	}	
}

void WinRegKey::Set(const String& val)
{
	bool needToClose = !m_isOpen;
	if (!m_isOpen)
	{
		Open(true);
	}

	int ret = RegSetValueEx(m_hkey, NULL, 0, REG_SZ, (const BYTE*)val.GetChars(), val.Length());
	
	if (needToClose)
	{
		Close();
	}
	
	if (ERROR_SUCCESS != ret)
	{
		throw new IOException("RegSetValueEx returned " + Int32::ToString(ret));
	}	
}

void WinRegKey::SetSubKey(const String&key, const String& val)
{
	bool needToClose = !m_isOpen;
	if (!m_isOpen)
	{
		Open(true);
	}

	int ret = RegSetValueEx(m_hkey, key.GetChars(), 0, REG_SZ, (const BYTE*)val.GetChars(), val.Length());
	
	if (needToClose)
	{
		Close();
	}
	
	if (ERROR_SUCCESS != ret)
	{
		throw new IOException("RegSetValueEx returned " + Int32::ToString(ret));
	}	
}

List<WinRegKey> WinRegKey::ListSubKeys()
{
	bool needToClose = !m_isOpen;
	if (!m_isOpen)
	{
		Open(false);
	}

	List<WinRegKey> list;
	char keyName[128];
	DWORD keySize = sizeof(keyName);
	int pos = 0;
	int ret;
			
	while (ERROR_SUCCESS == (ret = RegEnumKeyEx(m_hkey, pos++, (LPSTR)keyName, &keySize, 0, 0, 0, 0)))
	{
		list.Add(WinRegKey(m_path + "\\" + keyName, m_hive));
		keySize = sizeof(keyName);
	}
	
	if (needToClose)
	{
		Close();
	}
	
	if (ret != ERROR_NO_MORE_ITEMS)
	{
		throw new IOException("RegEnumKeyEx returned " + Int32::ToString(ret));
	}
	
	return list;
}

List<String> WinRegKey::ListSubValueNames()
{
	bool needToClose = !m_isOpen;
	if (!m_isOpen)
	{
		Open(false);
	}

	List<String> list;
	char keyName[128];
	DWORD keySize = sizeof(keyName);
	int pos = 0;
	int ret;
		
	keyName[0] = '\0';
	while (ERROR_SUCCESS == (ret = RegEnumValue(m_hkey, pos++, (LPSTR)keyName, &keySize, 0, 0, NULL, NULL)))
	{
		list.Add(keyName);
		keySize = sizeof(keyName);
	}
	
	if (needToClose)
	{
		Close();
	}
	
	if (ret != ERROR_NO_MORE_ITEMS)
	{
		throw new IOException("RegEnumValue returned " + Int32::ToString(ret));
	}
	
	return list;
}

String WinRegKey::ValueString(const String& name)
{
	bool needToClose = !m_isOpen;
	if (!m_isOpen)
	{
		Open(false);
	}
	
	DWORD dataType;
	char data[256];
	DWORD dataSize = sizeof(data);
	int ret = RegQueryValueEx(m_hkey, name.GetChars(), 0, &dataType, (LPBYTE)&data[0], &dataSize);
	
	if (needToClose)
	{
		Close();
	}
	
	if (ERROR_SUCCESS != ret)
	{
		throw new IOException("RegQueryValueEx returned " + Int32::ToString(ret));
	}
	
	if (REG_DWORD == dataType)
	{
		return Int32::ToString(*(int *)data);
	}
	return String(data, 0, dataSize-1);
}

int WinRegKey::ValueInt(const String& name)
{
	bool needToClose = !m_isOpen;
	if (!m_isOpen)
	{
		Open(false);
	}
	
	DWORD dataType;
	char data[8];
	DWORD dataSize = sizeof(data);
	int ret = RegQueryValueEx(m_hkey, name.GetChars(), 0, &dataType, (LPBYTE)&data[0], &dataSize);
	
	if (needToClose)
	{
		Close();
	}
	
	if (ERROR_SUCCESS != ret)
	{
		throw new IOException("RegQueryValueEx returned " + Int32::ToString(ret));
	}
	
	if (REG_DWORD != dataType)
	{
		if (Int32::IsInt(data, dataSize))
		{
			return Int32::Parse(data, dataSize-1);
		}
		throw new ConfigurationException(name + " is not an integer value");
	}
	return *(int *)data;
}

WinRegKey WinRegKey::Create(KeyHive hive, const String& pathAndKey)
{
	HKEY appsettingkey;
	int ret = RegCreateKeyEx
	(
		(HKEY)hive, 
		pathAndKey.GetChars(), 
		0, 
		NULL, 
		REG_OPTION_NON_VOLATILE, 
		KEY_SET_VALUE,
		NULL, 
		&appsettingkey, 
		NULL
	);
	if (ERROR_SUCCESS != ret)
	{
		throw new IOException("RegCreateKeyEx returned " + Int32::ToString(ret));
	}
	
	RegCloseKey(appsettingkey);
	
	return WinRegKey(pathAndKey, hive);
}

bool WinRegKey::KeyExists(KeyHive hive, const String& pathAndKey)
{
	int flags = KEY_READ;
	HKEY hkey = 0;
	
	int ret = RegOpenKeyEx
	(
		(HKEY)hive, 
		pathAndKey.GetChars(),
		0, 
		flags, 
		&hkey
	);
	RegCloseKey(hkey);	
	
	return ERROR_SUCCESS == ret;
}

#ifdef DEBUG
void WinRegKey::ValidateMem() const
{
	m_path.ValidateMem();
}

void WinRegKey::CheckMem() const
{
	m_path.CheckMem();
}
#endif
