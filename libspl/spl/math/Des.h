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
#ifndef _des_h
#define _des_h

#include <spl/types.h>
#include <spl/Debug.h>
#include <spl/collection/Array.h>
#include <spl/RefCountPtr.h>
#include <spl/String.h>

namespace spl
{
/** 
 * @defgroup math Math
 * @{
 */

/**
 * @brief Encryption/Decryption context of DES
 */
typedef struct _des_ctx
{
    uint32 encrypt_subkeys[32];
    uint32 decrypt_subkeys[32];
} des_ctx[1];

#include <spl/String.h>

/** @brief Based on code written by Michael Roth <mroth@nessie.de>, September 1998.
 *	See code file for orginal comments.
 */
class DES
{
private:
	// forbid copy constructor
	inline DES(const DES& pmp) {}
	inline void operator =(const DES& pmp) {}

protected:
	des_ctx m_ctx;

public:
	DES(const String& password);
	void EncryptBinary(const Array<byte>& data, Array<byte>& output);
	void DecryptBinary(Array<byte>& data);
	RefCountPtr<Array<byte> > DecryptString(const String& data);
	RefCountPtr<String> EncryptString(const Array<byte>& data);

	static RefCountPtr<Array<byte> > DecryptString(const String& password, const String& data);
	static RefCountPtr<String> EncryptString(const String& password, const Array<byte>& data);
	static void EncryptBinary(const String& password, const Array<byte>& data, Array<byte>& output);
	static void DecryptBinary(const String& password, Array<byte>& data );
};

/** @} */
}
#endif


