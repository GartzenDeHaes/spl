/**  @file
  *  This code was mostly ported from mono.
  */
//
// System.Web.HttpUtility
//
// Authors:
//   Patrik Torstensson (Patrik.Torstensson@labs2.com)
//   Wictor Wilén (decode/encode functions) (wictor@ibizkit.se)
//   Tim Coleman (tim@timcoleman.com)
//   Gonzalo Paniagua Javier (gonzalo@ximian.com)
//
// Copyright (C) 2005 Novell, Inc (http://www.novell.com)
//
// Permission is hereby granted, free of charge, to any person obtaining
// a copy of this software and associated documentation files (the
// "Software"), to deal in the Software without restriction, including
// without limitation the rights to use, copy, modify, merge, publish,
// distribute, sublicense, and/or sell copies of the Software, and to
// permit persons to whom the Software is furnished to do so, subject to
// the following conditions:
//
// The above copyright notice and this permission notice shall be
// included in all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
// MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
// NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
// LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
// OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
// WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
//
#include <ctype.h>
#include <spl/Exception.h>
#include <spl/Int32.h>
#include <spl/text/StringBuffer.h>
#include <spl/web/HttpUtility.h>

using namespace spl;

bool HttpUtility::HtmlAttributeEncodeRequired (const char *cp, int len)
{
	for (int i = 0; i < len; i++)
	{
		if ( cp[i] == '&' || cp[i] == '"' || cp[i] == '<')
		{
			return true;
		}
	}
	return false;
}

StringPtr HttpUtility::HtmlAttributeEncode (const char *cp, int len)
{
	int i;

	if (NULL == cp)
	{
		return StringPtr(new String());
	}


	if (!HtmlAttributeEncodeRequired(cp, len))
	{
		return StringPtr(new String(cp));
	}

	StringBuffer output;

	for (i = 0; i < len; i++)
	{
		switch ( cp[i] )
		{
		case '&' :
			output.Append ("&amp;");
			break;
		case '"' :
			output.Append ("&quot;");
			break;
		case '<':
			output.Append ("&lt;");
			break;
		default:
			output.Append ( cp[i] );
			break;
		}
	}

	return output.ToString();
}

static int _GetInt (char c)
{
	if (c >= '0' && c <= '9')
		return c - '0';

	if (c >= 'a' && c <= 'f')
		return c - 'a' + 10;

	if (c >= 'A' && c <= 'F')
		return c - 'A' + 10;

	return -1;
}

static int _GetChar (const char *bytes, int offset, int length)
{
	int value = 0;
	int end = length + offset;
	for (int i = offset; i < end; i++)
	{
		int current = _GetInt (bytes[i]);
		if (current == -1)
		{
			return -1;
		}
		value = (value << 4) + current;
	}

	return value;
}

StringPtr HttpUtility::UrlDecode (const char *cp, int len)
{
	if (NULL == cp)
	{
		return StringPtr(new String());
	}

	if ( IndexOfCh(cp, '%') < 0 && IndexOfCh(cp, '+') < 0 )
	{
		return StringPtr(new String(cp));
	}

	StringBuffer output;
	StringBuffer bytes;
	int xchar;

	for (int i = 0; i < len; i++)
	{
		if (cp[i] == '%' && i + 2 < len && cp[i + 1] != '%')
		{
			if (cp[i + 1] == 'u' && i + 5 < len)
			{
				if (bytes.Length() > 0)
				{
					output.Append (bytes.GetChars());
					bytes.SetLength(0);
				}

				xchar = _GetChar (cp, i + 2, 4);
				if (xchar != -1)
				{
					output.Append ((char) xchar);
					i += 5;
				}
				else
				{
					output.Append ('%');
				}
			}
			else if ((xchar = _GetChar(cp, i + 1, 2)) != -1)
			{
				bytes.Append ( (char)xchar );
				i += 2;
			}
			else
			{
				output.Append ('%');
			}
			continue;
		}

		if (bytes.Length() > 0)
		{
			output.Append (bytes.GetChars());
			bytes.SetLength(0);
		}

		if (cp[i] == '+')
		{
			output.Append (' ');
		}
		else
		{
			output.Append (cp[i]);
		}
	}

	if (bytes.Length() > 0)
	{
		output.Append (bytes.GetChars());
	}

	return output.ToString();
}

static char _urlHexChars[] = {'0','1','2','3','4','5','6','7','8','9','a','b','c','d','e','f'};

bool HttpUtility::UrlEncodeRequired (const char *cp, int len)
{
	for (int i = 0; i < len; i++)
	{
		char c = cp[i];
		if ((c < '0') || (c < 'A' && c > '9') || (c > 'Z' && c < 'a') || (c > 'z'))
		{
			if (NotEncoded (c))
			{
				continue;
			}
			return true;
		}
	}
	return false;
}

StringPtr HttpUtility::UrlEncode (const char *cp, int len)
{
	if (cp == NULL)
	{
		return StringPtr(new String());
	}

	if (cp[0] == '\0')
	{
		return StringPtr(new String());
	}

	int i;
	bool isUnicode = false;


	if (!UrlEncodeRequired(cp, len))
	{
		return StringPtr(new String(cp));
	}

	StringBuffer result;

	for ( i = 0; i < len; i++ )
	{
		char c = cp[i];

#if CHAR_MAX > 255
		if (c > 255)
		{
			int idx;
			int i = (int) c;

			result.Append('%');
			result.Append ('u');
			idx = i >> 12;
			result.Append (_urlHexChars [idx]);
			idx = (i >> 8) & 0x0F;
			result.Append (_urlHexChars [idx]);
			idx = (i >> 4) & 0x0F;
			result.Append (_urlHexChars [idx]);
			idx = i & 0x0F;
			result.Append (_urlHexChars [idx]);
			continue;
		}
#endif
		if (c > ' ' && NotEncoded (c))
		{
			result.Append(c);
			continue;
		}
		if (c==' ')
		{
			result.Append('+');
			continue;
		}
		if ( (c < '0') ||
			(c < 'A' && c > '9') ||
			(c > 'Z' && c < 'a') ||
			(c > 'z'))
		{
#if CHAR_MAX > 127
			if (isUnicode && c > 127)
			{
				result.Append ('%');
				result.Append ('u');
				result.Append ('0');
				result.Append ('0');
			}
			else
#endif
			{
				result.Append('%');
			}
			int idx = ((int) c) >> 4;
			result.Append (_urlHexChars [idx]);
			idx = ((int) c) & 0x0F;
			result.Append (_urlHexChars [idx]);
		}
		else
		{
			result.Append (c);
		}
	}

	return result.ToString();
}

StringPtr HttpUtility::HtmlDecode (const char *cp, int len)
{
	if (cp == NULL)
	{
		throw new InvalidArgumentException("HtmlDecode: argument was null");
	}

	if (IndexOfCh(cp, '&') < 0)
	{
		return StringPtr(new String(cp));
	}

	StringBuffer entity;
	StringBuffer output;

	// 0 -> nothing,
	// 1 -> right after '&'
	// 2 -> between '&' and ';' but no '#'
	// 3 -> '#' found after '&' and getting numbers
	int state = 0;
	int number = 0;
	bool have_trailing_digits = false;

	for (int i = 0; i < len; i++)
	{
		char c = cp[i];
		if (state == 0)
		{
			if (c == '&')
			{
				entity.Append (c);
				state = 1;
			}
			else
			{
				output.Append (c);
			}
			continue;
		}

		if (c == '&')
		{
			state = 1;
			if (have_trailing_digits)
			{
				entity.Append(Int32::ToString(number));
				have_trailing_digits = false;
			}

			output.Append(entity.GetChars());
			entity.SetLength(0);
			entity.Append ('&');
			continue;
		}

		if (state == 1)
		{
			if (c == ';')
			{
				state = 0;
				output.Append(entity.GetChars());
				output.Append(c);
				entity.SetLength(0);
			}
			else
			{
				number = 0;
				if (c != '#')
				{
					state = 2;
				}
				else
				{
					state = 3;
				}
				entity.Append (c);
			}
		}
		else if (state == 2)
		{
			entity.Append (c);
			if (c == ';')
			{
				// This only checks the most common entities, since I don't want a static hashtable in the library.
				if (entity.Equals("&nbsp;"))
				{
					output.Append(' ');
				}
				else if (entity.Equals("&amp;"))
				{
					output.Append('&');
				}
				else if (entity.Equals("&lt;"))
				{
					output.Append('<');
				}
				else if (entity.Equals("&gt;"))
				{
					output.Append('>');
				}
				else if (entity.Equals("&quot;"))
				{
					output.Append('"');
				}
				else
				{
					output.Append(entity.GetChars());
				}
				state = 0;
				entity.SetLength(0);;
			}
		}
		else if (state == 3)
		{
			if (c == ';')
			{
				if (number > 65535)
				{
					output.Append("&#");
					output.Append(Int32::ToString(number));
					output.Append(";");
				}
				else
				{
					output.Append ((char) number);
				}
				state = 0;
				entity.SetLength(0);
				have_trailing_digits = false;
			}
			else if (isdigit(c))
			{
				number = number * 10 + ((int) c - '0');
				have_trailing_digits = true;
			}
			else
			{
				state = 2;
				if (have_trailing_digits)
				{
					entity.Append (Int32::ToString(number));
					have_trailing_digits = false;
				}
				entity.Append(c);
			}
		}
	}

	if (entity.Length() > 0)
	{
		output.Append(entity.GetChars());
	}
	else if (have_trailing_digits)
	{
		output.Append (Int32::ToString(number));
	}
	return output.ToString ();
}

bool HttpUtility::HtmlEncodeRequired (const char *cp, int len)
{
	for (int i = 0; i < len; i++)
	{
		char c = cp[i];
		if (c == '&' || c == '"' || c == '<' || c == '>' || c > 159)
		{
			return true;
		}
	}
	return false;
}

StringPtr HttpUtility::HtmlEncode (const char *cp, int len)
{
	if (cp == NULL)
	{
		return StringPtr(new String());
	}

	int i;


	if (!HtmlEncodeRequired(cp, len))
	{
		return StringPtr(new String(cp));
	}

	StringBuffer output;

	for (i = 0; i < len; i++)
	{
		switch (cp[i])
		{
		case '&':
			output.Append ("&amp;");
			break;
		case '>' :
			output.Append ("&gt;");
			break;
		case '<':
			output.Append ("&lt;");
			break;
		case '"':
			output.Append ("&quot;");
			break;
		default:
			// MS starts encoding with &# from 160 and stops at 255.
			// We don't do that. One reason is the 65308/65310 unicode
			// characters that look like '<' and '>'.
#if CHAR_MAX > 127
			if (cp[i] > 159)
			{
				output.Append ("&#");
				output.Append (Int32::ToString((int)cp[i]));
				output.Append (";");
			}
			else
#endif
			{
				output.Append (cp[i]);
			}
			break;
		}
	}
	return output.ToString();
}
