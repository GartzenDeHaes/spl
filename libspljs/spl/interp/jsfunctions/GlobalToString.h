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
#ifndef _globaltostring_h
#define _globaltostring_h

#include <spl/interp/JsMethod.h>

namespace spl
{
	class GlobalToString : public JsMethod
	{
	public:
		GlobalToString();
		virtual ~GlobalToString();
		
		virtual bool IsNative() const;
		virtual VariantPtr Call(JsMethod *isthis, Vector<VariantPtr>& args);
	};

	extern VariantPtr JsGlobalParseFloat(JsMethod *isthis, Vector<VariantPtr>& args);
	extern VariantPtr JsGlobalParseInt(JsMethod *isthis, Vector<VariantPtr>& args);
}
#endif