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
#include <stdio.h>
#include <stdlib.h>

#include <spl/types.h>

#ifdef HAVE_STRING_H
#include <string.h>
#endif

#include <spl/Image.h>

using namespace spl;

static unsigned int u32fread(FILE *);
static unsigned int u16fread(FILE *);
static unsigned int u8fread(FILE *);

static bool IsBMP(char const *name)
{
	int len = (int)strlen(name);
	return name[len-1] == 'p' && name[len-2] == 'm' && name[len-3] == 'b' && name[len-4] == '.';
}

Image *Image::GetImage(const char *name)
{
    Image *im = new Image();
	if ( ! IsBMP(name) )
	{
		delete im;
		return NULL;
	}
    /* Try to load a BMP file */
    unsigned int i, colors, offset, tmp, planes;
    FILE *fp;

    fp = fopen(name, "rb");
    if(!fp)
    {
        delete im;
        return NULL;
    }

    if(u16fread(fp) != 0x4d42)
    {
        fclose(fp);
        delete im;
        return NULL;
    }

    u32fread(fp); /* size */
    u16fread(fp); /* reserved 1 */
    u16fread(fp); /* reserved 2 */

    offset = u32fread(fp);

    tmp = u32fread(fp); /* header size */
    if(tmp == 40)
    {
        im->m_w = u32fread(fp);
        im->m_h = u32fread(fp);
        planes = u16fread(fp);
        im->m_bpp = u16fread(fp);

        tmp = u32fread(fp); /* compression */
        if(tmp != 0)
        {
            fclose(fp);
            delete im;
            return NULL;
        }

        u32fread(fp); /* sizeimage */
        u32fread(fp); /* xpelspermeter */
        u32fread(fp); /* ypelspermeter */
        u32fread(fp); /* biclrused */
        u32fread(fp); /* biclrimportantn */

        colors = (offset - 54) / 4;
        for(i = 0; i < colors && i < 256; i++)
        {
            im->m_blue[i] = u8fread(fp) * 16;
            im->m_green[i] = u8fread(fp) * 16;
            im->m_red[i] = u8fread(fp) * 16;
            im->m_alpha[i] = 0;
            u8fread(fp);
        }
    }
    else if(tmp == 12)
    {
        im->m_w = u32fread(fp);
        im->m_h = u32fread(fp);
        planes = u16fread(fp);
        im->m_bpp = u16fread(fp);

        colors = (offset - 26) / 3;
        for(i = 0; i < colors && i < 256; i++)
        {
            im->m_blue[i] = u8fread(fp);
            im->m_green[i] = u8fread(fp);
            im->m_red[i] = u8fread(fp);
            im->m_alpha[i] = 0;
        }
    }
    else
    {
        fclose(fp);
        delete im;
        return NULL;
    }

    /* Fill the rest of the palette */
    for(i = colors; i < 256; i++)
        im->m_blue[i] = im->m_green[i] = im->m_red[i] = im->m_alpha[i] = 0;

    im->m_depth = (im->m_bpp + 7) / 8;

    /* Sanity check */
    if(!im->m_w || im->m_w > 0x10000 || !im->m_h || im->m_h > 0x10000 || planes != 1)
    {
        fclose(fp);
        delete im;
        return NULL;
    }

    /* Allocate the pixel buffer */
    im->m_pixels = (byte *)malloc(im->m_w * im->m_h * im->m_depth);
    if(!im->m_pixels)
    {
        fclose(fp);
        delete im;
        return NULL;
    }

    memset(im->m_pixels, 0, im->m_w * im->m_h * im->m_depth);

    /* Read the bitmap data */
    for(i = im->m_h; i--; )
    {
		int j;
        unsigned int k, bits = 0;

        switch(im->m_bpp)
        {
            case 1:
                for(j = 0; j < im->m_w; j++)
                {
                    k = j % 32;
                    if(k == 0)
                        bits = u32fread(fp);
                    im->m_pixels[im->m_w * i * im->m_depth + j] =
                        (bits >> ((k & ~0xf) + 0xf - (k & 0xf))) & 0x1;
                }
                break;
            case 4:
                for(j = 0; j < im->m_w; j++)
                {
                    k = j % 8;
                    if(k == 0)
                        bits = u32fread(fp);
                    im->m_pixels[im->m_w * i * im->m_depth + j] =
                        (bits >> (4 * ((k & ~0x1) + 0x1 - (k & 0x1)))) & 0xf;
                }
                break;
            default:
                /* Works for 8bpp, but also for 16, 24 etc. */
                fread(im->m_pixels + im->m_w * i * im->m_depth, im->m_w * im->m_depth, 1, fp);
                /* Pad reads to 4 bytes */
                tmp = (im->m_w * im->m_depth) % 4;
                tmp = (4 - tmp) % 4;
                while(tmp--)
                    u8fread(fp);
                break;
        }
    }

    switch(im->m_depth)
    {
    case 3:
        im->m_rmask = 0xff0000;
        im->m_gmask = 0x00ff00;
        im->m_bmask = 0x0000ff;
        im->m_amask = 0x000000;
        break;
    case 2: /* XXX: those are the 16 bits values */
        im->m_rmask = 0x7c00;
        im->m_gmask = 0x03e0;
        im->m_bmask = 0x001f;
        im->m_amask = 0x0000;
        break;
    case 1:
    default:
        im->m_bpp = 8;
        im->m_rmask = im->m_gmask = im->m_bmask = im->m_amask = 0;
        break;
    }
    fclose(fp);

    return im;
}

Image::Image()
: m_pixels(NULL)
{
}

Image::~Image()
{
	if ( NULL != m_pixels )
	{
		free(m_pixels);
	}
}

#ifdef DEBUG
void Image::ValidateMem () const
{
	ASSERT_PTR(m_pixels);
}

void Image::CheckMem () const
{
	DEBUG_NOTE_MEM(m_pixels);
}
#endif


#if !defined(USE_IMLIB2)
static unsigned int u32fread(FILE *fp)
{
    unsigned char buffer[4];
    fread(buffer, 4, 1, fp);
    return ((unsigned int)buffer[3] << 24) | ((unsigned int)buffer[2] << 16)
             | ((unsigned int)buffer[1] << 8) | ((unsigned int)buffer[0]);
}

static unsigned int u16fread(FILE *fp)
{
    unsigned char buffer[2];
    fread(buffer, 2, 1, fp);
    return ((unsigned int)buffer[1] << 8) | ((unsigned int)buffer[0]);
}

static unsigned int u8fread(FILE *fp)
{
    unsigned char buffer;
    fread(&buffer, 1, 1, fp);
    return (unsigned int)buffer;
}
#endif

