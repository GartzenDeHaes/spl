#include <spl/Double.h>
#include <spl/Int64.h>
#include <spl/interp/jsfunctions/JsMathFn.h>

namespace spl
{
	VariantPtr JsGlobalIsFinite(JsMethod *isthis, Vector<VariantPtr>& args)
	{
		double d;
		if (args.ElementAtRef(0)->ToFloat64(d))
		{
			return VariantPtr(new Variant(!Math::IsINF(d)));
		}
		else
		{
			return VariantPtr(new Variant());
		}
	}

	VariantPtr JsGlobalIsNan(JsMethod *isthis, Vector<VariantPtr>& args)
	{
		double d;
		if (args.ElementAtRef(0)->ToFloat64(d))
		{
			return VariantPtr(new Variant(Math::IsNaN(d)));
		}
		else
		{
			return VariantPtr(new Variant());
		}
	}

	VariantPtr JsMathAbs(JsMethod *isthis, Vector<VariantPtr>& args)
	{
		double d;
		if (args.ElementAtRef(0)->ToFloat64(d))
		{
			return VariantPtr(new Variant(Math::Abs(d)));
		}
		else
		{
			return VariantPtr(new Variant());
		}
	}

	VariantPtr JsMathAcos(JsMethod *isthis, Vector<VariantPtr>& args)
	{
		double d;
		if (args.ElementAtRef(0)->ToFloat64(d))
		{
			return VariantPtr(new Variant(Math::Acos(d)));
		}
		else
		{
			return VariantPtr(new Variant());
		}
	}

	VariantPtr JsMathAsin(JsMethod *isthis, Vector<VariantPtr>& args)
	{
		double d;
		if (args.ElementAtRef(0)->ToFloat64(d))
		{
			return VariantPtr(new Variant(Math::Asin(d)));
		}
		else
		{
			return VariantPtr(new Variant());
		}
	}

	VariantPtr JsMathAtan(JsMethod *isthis, Vector<VariantPtr>& args)
	{
		double d;
		if (args.ElementAtRef(0)->ToFloat64(d))
		{
			return VariantPtr(new Variant(Math::Atan(d)));
		}
		else
		{
			return VariantPtr(new Variant());
		}
	}

	VariantPtr JsMathAtan2(JsMethod *isthis, Vector<VariantPtr>& args)
	{
		double x, y;
		if (args.ElementAtRef(0)->ToFloat64(y) && args.ElementAtRef(1)->ToFloat64(x))
		{
			return VariantPtr(new Variant(Math::Atan2(x, y)));
		}
		else
		{
			return VariantPtr(new Variant());
		}
	}

	VariantPtr JsMathCeil(JsMethod *isthis, Vector<VariantPtr>& args)
	{
		double d;
		if (args.ElementAtRef(0)->ToFloat64(d))
		{
			return VariantPtr(new Variant(Math::Ceiling(d)));
		}
		else
		{
			return VariantPtr(new Variant());
		}
	}

	VariantPtr JsMathCos(JsMethod *isthis, Vector<VariantPtr>& args)
	{
		double d;
		if (args.ElementAtRef(0)->ToFloat64(d))
		{
			return VariantPtr(new Variant(Math::Cos(d)));
		}
		else
		{
			return VariantPtr(new Variant());
		}
	}

	VariantPtr JsMathExp(JsMethod *isthis, Vector<VariantPtr>& args)
	{
		double d;
		if (args.ElementAtRef(0)->ToFloat64(d))
		{
			return VariantPtr(new Variant(Math::Exp(d)));
		}
		else
		{
			return VariantPtr(new Variant());
		}
	}

	VariantPtr JsMathFloor(JsMethod *isthis, Vector<VariantPtr>& args)
	{
		double d;
		if (args.ElementAtRef(0)->ToFloat64(d))
		{
			return VariantPtr(new Variant(Math::Floor(d)));
		}
		else
		{
			return VariantPtr(new Variant());
		}
	}

	VariantPtr JsMathLog(JsMethod *isthis, Vector<VariantPtr>& args)
	{
		double d;
		if (args.ElementAtRef(0)->ToFloat64(d))
		{
			return VariantPtr(new Variant(Math::Log(d)));
		}
		else
		{
			return VariantPtr(new Variant());
		}
	}

	VariantPtr JsMathMax(JsMethod *isthis, Vector<VariantPtr>& args)
	{
		double d = (double)Int64::MinValue();
		for ( int x = 0; x < args.Count(); x++ )
		{
			double dd;
			if (args.ElementAtRef(x)->ToFloat64(dd))
			{
				if (dd > d)
				{
					d = dd;
				}
			}
		}

		return VariantPtr(new Variant(d));
	}

	VariantPtr JsMathMin(JsMethod *isthis, Vector<VariantPtr>& args)
	{
		double d = (double)Int64::MaxValue();
		for ( int x = 0; x < args.Count(); x++ )
		{
			double dd;
			if (args.ElementAtRef(x)->ToFloat64(dd))
			{
				if (dd < d)
				{
					d = dd;
				}
			}
		}

		return VariantPtr(new Variant(d));
	}

	VariantPtr JsMathPow(JsMethod *isthis, Vector<VariantPtr>& args)
	{
		double d;
		if (args.ElementAtRef(0)->ToFloat64(d))
		{
			return VariantPtr(new Variant());
		}
		else
		{
			return VariantPtr(new Variant());
		}
	}

	VariantPtr JsMathRandom(JsMethod *isthis, Vector<VariantPtr>& args)
	{
		return VariantPtr(new Variant(Math::Random()));
	}

	VariantPtr JsMathRound(JsMethod *isthis, Vector<VariantPtr>& args)
	{
		double d;
		if (args.ElementAtRef(0)->ToFloat64(d))
		{
			return VariantPtr(new Variant(Math::Round(d)));
		}
		else
		{
			return VariantPtr(new Variant());
		}
	}

	VariantPtr JsMathSin(JsMethod *isthis, Vector<VariantPtr>& args)
	{
		double d;
		if (args.ElementAtRef(0)->ToFloat64(d))
		{
			return VariantPtr(new Variant(Math::Sin(d)));
		}
		else
		{
			return VariantPtr(new Variant());
		}
	}

	VariantPtr JsMathSqrt(JsMethod *isthis, Vector<VariantPtr>& args)
	{
		double d;
		if (args.ElementAtRef(0)->ToFloat64(d))
		{
			return VariantPtr(new Variant(Math::Sqrt(d)));
		}
		else
		{
			return VariantPtr(new Variant());
		}
	}

	VariantPtr JsMathTan(JsMethod *isthis, Vector<VariantPtr>& args)
	{
		double d;
		if (args.ElementAtRef(0)->ToFloat64(d))
		{
			return VariantPtr(new Variant(Math::Tan(d)));
		}
		else
		{
			return VariantPtr(new Variant());
		}
	}
}
