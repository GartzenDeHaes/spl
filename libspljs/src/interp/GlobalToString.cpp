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
#include <spl/interp/jsfunctions/GlobalToString.h>
#include <spl/interp/JsString.h>

namespace spl
{
	GlobalToString::GlobalToString()
	{
		m_prog->NoteArgument();
	}

	GlobalToString::~GlobalToString()
	{
	}

	bool GlobalToString::IsNative() const
	{
		return true;
	}

	VariantPtr GlobalToString::Call(JsMethod *isthis, Vector<VariantPtr>& args)
	{
		if (0 == args.Count())
		{
			return VariantPtr(new Variant());
		}
		return VariantPtr(new Variant(IJsObjectPtr(new JsString(*args.ElementAtRef(0)->ToString()))));
	}

	VariantPtr JsGlobalParseFloat(JsMethod *isthis, Vector<VariantPtr>& args)
	{
		double d;
		if (args.ElementAtRef(0)->ToFloat64(d))
		{
			return VariantPtr(new Variant(d));
		}

		return VariantPtr(new Variant());
	}

	VariantPtr JsGlobalParseInt(JsMethod *isthis, Vector<VariantPtr>& args)
	{
		int32 d;
		if (args.ElementAtRef(0)->ToInt32(d))
		{
			return VariantPtr(new Variant(d));
		}

		return VariantPtr(new Variant());
	}
}
