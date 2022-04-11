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
#ifndef _point4_h
#define _point4_h

#include <spl/types.h>
#include <spl/Debug.h>
#include <spl/math/Math.h>
#include <spl/Memory.h>

namespace spl
{
	/** 
	 * @defgroup math Math
	 * @{
	 */

	template<typename T, typename FLOATTYPE> class Vector3;
	template<typename T, typename FLOATTYPE> class Vector4;

	/** @brief Point in R3 */
	template<typename T, typename FLOATTYPE>
	class Point3 : public IMemoryValidate
	{
	protected:
		T m_x, m_y, m_z;

	public:
		Point3()
		: m_x(0), m_y(0), m_z(0)
		{
		}

		Point3(T x)
		: m_x(x), m_y(0), m_z(0)
		{
		}

		Point3(T x, T y)
		: m_x(x), m_y(y), m_z(0)
		{
		}

		Point3(T x, T y, T z)
		: m_x(x), m_y(y), m_z(z)
		{
		}

		Point3(const Point3<T, FLOATTYPE>& pt)
		: m_x(pt.m_x), m_y(pt.m_y), m_z(pt.m_z)
		{
		}

		inline Point3<T, FLOATTYPE>& operator=(const Point3<T, FLOATTYPE>& pt)
		{
			m_x = pt.m_x;
			m_y = pt.m_y;
			m_z = pt.m_z;

			return *this;
		}

		inline T X() { return m_x; }
		inline T Y() { return m_y; }
		inline T Z() { return m_z; }

		// Point and Vector Operations (always valid) 
		Vector3<T,FLOATTYPE> operator -(Point3<T,FLOATTYPE>& pt)       // Vector difference
		{
			return Vector3<T,FLOATTYPE>(m_x - pt.m_x, m_y - pt.m_y, m_z - pt.m_z);
		}

		Point3<T,FLOATTYPE> operator +(Vector3<T,FLOATTYPE>& v)      // +translate
		{
			return Point3<T,FLOATTYPE>(m_x + pt.m_x, m_y + pt.m_y, m_z + pt.m_z);
		}

		Point3<T,FLOATTYPE> operator -(Vector3<T,FLOATTYPE>& v)      // -translate
		{
			return Point3<T,FLOATTYPE>(m_x - v.m_x, m_y - v.m_y, m_z - v.m_z);
		}

		Point3<T,FLOATTYPE>& operator +=(Vector3<T,FLOATTYPE>& v)     // inc translate
		{
			m_x += v.m_x;
			m_y += v.m_y;
			m_z += v.m_z;
			return *this;
		}

		Point3<T,FLOATTYPE>& operator -=(Vector3<T,FLOATTYPE>& v)     // dec translate
		{
			m_x -= v.m_x;
			m_y -= v.m_y;
			m_z -= v.m_z;
			return *this;
		}

		inline bool IsEqual(Point3<T,FLOATTYPE> *pt)
		{
			return m_x == pt->m_x && m_y == pt->m_y && m_z == pt->m_z;
		}

		bool operator ==(Point3<T,FLOATTYPE>& pt)
		{
			return IsEqual(pt)
		}

		bool operator !=(Point3<T,FLOATTYPE>& pt)
		{
			return !IsEqual(pt);
		}

		///@brief Euclidean distance
		static FLOATTYPE Distance( Point3<T,FLOATTYPE> *p, Point3<T,FLOATTYPE> *q ) 
		{
			FLOATTYPE dx = p->m_x - q->m_x;
			FLOATTYPE dy = p->m_y - q->m_y;
			FLOATTYPE dz = p->m_z - q->m_z;
			return FastMath::Sqrt(dx*dx + dy*dy + dz*dz);
		}

		///@brief Sidedness of a Point wrt a directed line P1->P2 - makes sense in 2D only
		FLOATTYPE IsLeft( Point3<T,FLOATTYPE> *p1, Point3<T,FLOATTYPE> *p2) 
		{
			return ((p1->m_x - m_x) * (p2->m_y - m_y) - (p2->m_x - m_x) * (p1->m_y - m_y));
		}

		virtual void ValidateMem() const
		{
		}
		
		virtual void CheckMem() const
		{
		}
	};

	/** @brief Point in R4 */
	template<typename T, typename FLOATTYPE>
	class Point4 : public IMemoryValidate
	{
	protected:
		T m_x, m_y, m_z, m_w;

	public:
		Point4()
		: m_x(0), m_y(0), m_z(0), m_w(0)
		{
		}

		Point4(T x)
		: m_x(x), m_y(0), m_z(0), m_w(0)
		{
		}

		Point4(T x, T y)
		: m_x(x), m_y(y), m_z(0), m_w(0)
		{
		}

		Point4(T x, T y, T z)
		: m_x(x), m_y(y), m_z(z), m_w(0)
		{
		}

		Point4(T x, T y, T z, T w)
		: m_x(x), m_y(y), m_z(z), m_w(w)
		{
		}

		Point4(const Point4<T, FLOATTYPE>& pt)
		: m_x(pt.m_x), m_y(pt.m_y), m_z(pt.m_z), m_w(pt.m_w)
		{
		}

		inline Point4<T, FLOATTYPE>& operator =(const Point4<T, FLOATTYPE>& pt)
		{
			m_x = pt.m_x;
			m_y = pt.m_y;
			m_z = pt.m_z;
			m_w = pt.m_w;

			return *this;
		}

		inline T X() { return m_x; }
		inline T Y() { return m_y; }
		inline T Z() { return m_z; }
		inline T W() { return m_w; }

		// Point and Vector Operations (always valid) 
		Vector4<T,FLOATTYPE> operator -(Point4<T,FLOATTYPE>& pt)       // Vector difference
		{
			return Vector4<T,FLOATTYPE>(m_x - pt.m_x, m_y - pt.m_y, m_z - pt.m_z, m_w - pt.m_w);
		}

		Point4<T,FLOATTYPE> operator +(Vector4<T,FLOATTYPE>& v)      // +translate
		{
			return Point4<T,FLOATTYPE>(m_x + pt.m_x, m_y + pt.m_y, m_z + pt.m_z, m_w + pt.m_w);
		}

		Point4<T,FLOATTYPE> operator -(Vector4<T,FLOATTYPE>& v)      // -translate
		{
			return Point4<T,FLOATTYPE>(m_x - v.m_x, m_y - v.m_y, m_z - v.m_z, m_w - v.m_w);
		}

		Point4<T,FLOATTYPE>& operator +=(Vector4<T,FLOATTYPE>& v)     // inc translate
		{
			m_x += v.m_x;
			m_y += v.m_y;
			m_z += v.m_z;
			m_w += v.m_w;
			return *this;
		}

		Point4<T,FLOATTYPE>& operator -=(Vector4<T,FLOATTYPE>& v)     // dec translate
		{
			m_x -= v.m_x;
			m_y -= v.m_y;
			m_z -= v.m_z;
			m_w -= v.m_w;
			return *this;
		}

		inline bool IsEqual(Point4<T,FLOATTYPE> *pt)
		{
			return m_x == pt->m_x && m_y == pt->m_y && m_z == pt->m_z && m_w == pt->m_w;
		}

		bool operator ==(Point4<T,FLOATTYPE>& pt)
		{
			return IsEqual(pt)
		}

		bool operator !=(Point4<T,FLOATTYPE>& pt)
		{
			return !IsEqual(pt);
		}

		///@brief Euclidean distance
		static FLOATTYPE Distance( Point4<T,FLOATTYPE> *p, Point4<T,FLOATTYPE> *q ) 
		{
			FLOATTYPE dx = p->m_x - q->m_x;
			FLOATTYPE dy = p->m_y - q->m_y;
			FLOATTYPE dz = p->m_z - q->m_z;
			return FastMath::Sqrt(dx*dx + dy*dy + dz*dz);
		}

		///@brief Sidedness of a Point wrt a directed line P1->P2 - makes sense in 2D only
		FLOATTYPE IsLeft( Point4<T,FLOATTYPE> *p1, Point4<T,FLOATTYPE> *p2) 
		{
			return ((p1->m_x - m_x) * (p2->m_y - m_y) - (p2->m_x - m_x) * (p1->m_y - m_y));
		}

		virtual void ValidateMem() const
		{
		}
		
		virtual void CheckMem() const
		{
		}
	};

	#include <spl/math/Vector.h>

	/** @} */
}
#endif
