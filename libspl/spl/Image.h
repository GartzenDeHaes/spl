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
#ifndef _image_h
#define _image_h

#include <spl/types.h>
#include <spl/Debug.h>
#include <spl/Memory.h>

namespace spl
{
/** 
 * @defgroup types Types
 * @ingroup core
 * @{
 */

/** @brief Simplistic image data, currently only works with BMP.
 *
 */
class Image : public IMemoryValidate
{
private:
	// forbid copy constructor
	inline Image(const Image& i) {}
	inline void operator =(const Image& i) {}

protected:
	byte *m_pixels;
	int m_w, m_h;
    int m_depth, m_bpp;
	unsigned int m_rmask, m_gmask, m_bmask, m_amask;
    unsigned int m_red[256], m_green[256], m_blue[256], m_alpha[256];

public:
	Image();
	virtual ~Image();

	byte *Pixels() { return m_pixels; }
	int Depth() { return m_depth; }
	int Bbp() { return m_bpp; }
	int Width() { return m_w; }
	int Height() { return m_h; }
	unsigned int RMask() { return m_rmask; }
	unsigned int GMask() { return m_gmask; }
	unsigned int BMask() { return m_bmask; }
	unsigned int AMask() { return m_amask; }
	unsigned int *PRed() { return m_red; }
	unsigned int *PGreen() { return m_green; }
	unsigned int *PBlue() { return m_blue; }
	unsigned int *PAlpha() { return m_alpha; }

	/** Load a BMP file */
	static Image *GetImage( const char *filename );

#ifdef DEBUG
	virtual void ValidateMem () const;
	virtual void CheckMem () const;
#endif
};

/** @} */
}
#endif
