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
#ifndef _vector4_h
#define _vector4_h

#include <spl/types.h>
#include <spl/Debug.h>
#include <spl/math/Point.h>

namespace spl
{
	/** 
	 * @defgroup math Math
	 * @{
	 */

	template<typename T, typename FLOATTYPE> class Matrix3;
	template<typename T, typename FLOATTYPE> class Matrix4;

	/** @brief Math vector in R3 */
	template <typename T, typename FLOATTYPE>
	class Vector3 : public Point3<T, FLOATTYPE>, public IMemoryValidate
	{
	protected:

	public:
		Vector3()
		: Point3<T,FLOATTYPE>()
		{
		}

		Vector3(T x)
		: Point3<T,FLOATTYPE>(x)
		{
		}

		Vector3(T x, T y)
		: Point3<T,FLOATTYPE>(x, y)
		{
		}

		Vector3(T x, T y, T z)
		: Point3<T,FLOATTYPE>(x, y, z)
		{
		}

		Vector3(const Vector3<T, FLOATTYPE>& v)
		: Point3<T, FLOATTYPE>(v)
		{
		}

		/// @brief Lenght of this vector
		inline FLOATTYPE Length()
		{
			return (FLOATTYPE)FastMath::Sqrt((FLOATTYPE)(m_x*m_x+m_y*m_y+m_z*m_z));
		}

		///@brief Normalizes this vector
		void Normalize()
		{
			FLOATTYPE dist = Length();
			if (dist == 0) 
			{
				return;
			}
			FLOATTYPE invdist = 1 / dist;
			m_x = (T)(m_x * invdist);
			m_y = (T)(m_y * invdist);
			m_z = (T)(m_z * invdist);
		}

		///@brief Reverses this vector
		void Reverse()
		{
			m_x = -m_x;
			m_y = -m_y;
			m_z = -m_z;
		}

		///@brief Returns a modified vector by matrix m
		Vector3<T,FLOATTYPE> Transform(Matrix3<T,FLOATTYPE> *m)
		{
			return Vector3<T,FLOATTYPE>(
				m_x*m->m00 + m_y*m->m01 + m_z*m->m02+ m_w*m->m03,
				m_x*m->m10 + m_y*m->m11 + m_z*m->m12+ m_w*m->m13,
				m_x*m->m20 + m_y*m->m21 + m_z*m->m22+ m_w*m->m23
			);
		}
		
		///@brief Modifies the vector by matrix m
		Vector3<T,FLOATTYPE> Transform(Matrix4<T,FLOATTYPE> *m)
		{
			return Vector3<T,FLOATTYPE>(
				m_x*m->m00 + m_y*m->m01 + m_z*m->m02+ m->m03,
				m_x*m->m10 + m_y*m->m11 + m_z*m->m12+ m->m13,
				m_x*m->m20 + m_y*m->m21 + m_z*m->m22+ m->m23);
		}

		void MultEqual( Matrix3<T,FLOATTYPE> *m )
		{
			FLOATTYPE a, b, c;
				
			a = m_x * m->m00 + m_y * m->m01 + m_z * m->m02;
			b = m_x * m->m10 + m_y * m->m11 + m_z * m->m12;
			c = m_x * m->m20 + m_y * m->m21 + m_z * m->m22;
			m_x = a;
			m_y = b;
			m_z = c;
		}

		void MultEqual( Vector3<T,FLOATTYPE> *v )
		{
			m_x *= v->m_x;
			m_y *= v->m_y;
			m_z *= v->m_z;
		}

		///@breif Returns the cylindric coordinates out of the given cartesian coordinates
		static void GetCylindric(T x, T y, FLOATTYPE *r, FLOATTYPE *theta)
		{
			*r = (FLOATTYPE)FastMath::Sqrt(x*x + y*y);
			*theta = (FLOATTYPE)Math::Atan2(x, y);
		}

		static void GetCartesian(T *x, T *y, FLOATTYPE r, FLOATTYPE theta)
		// Builds the cartesian coordinates out of the given cylindric coordinates
		{
			*x = r * FastMath::Cos(theta);
			*y = r * FastMath::Sin(theta);
		}

		///@brief Returns the normal vector of the plane defined by the two vectors
		static Vector3<T,FLOATTYPE> GetNormal(Vector4<T,FLOATTYPE> *a, Vector4<T,FLOATTYPE> *b)
		{
			Vector3<T,FLOATTYPE> v = VectorProduct(a, b);
			v.Normalize();
			return v;
		}

		///@brief Returns the normal vector of the plane defined by the two vectors
		static Vector3<T,FLOATTYPE> GetNormal(Vector3<T,FLOATTYPE> *a, Vector3<T,FLOATTYPE> *b, Vector3<T,FLOATTYPE> *c)
		{
			Vector3<T,FLOATTYPE> v = VectorProduct(a,b,c);
			v.Normalize();
			return v;
		}

		///@brief Returns a x b
		static Vector3<T,FLOATTYPE> VectorProduct(Vector3<T,FLOATTYPE> *a, Vector3<T,FLOATTYPE> *b)
		{
			return Vector3<T,FLOATTYPE>(a->m_y*b->m_z-b->m_y*a->m_z,a->m_z*b->m_x-b->m_z*a->m_x,a->m_x*b->m_y-b->m_x*a->m_y);
		}

		///@brief Returns (b-a) x (c-a)
		static Vector3<T,FLOATTYPE> VectorProduct(Vector4<T,FLOATTYPE> *a, Vector3<T,FLOATTYPE> *b, Vector3<T,FLOATTYPE> *c)
		{
			Vector4<T,FLOATTYPE> v1 = Sub(b, a);
			Vector4<T,FLOATTYPE> v2 = Sub(c, a);
			return VectorProduct(&v1, &v2);
		}

		inline FLOATTYPE DotProduct(Vector3<T,FLOATTYPE> *b)
		{
			return m_x*b->m_x + m_y*b->m_y + m_z*b->m_z;
		}

		///@brief 3D Exterior Cross Product
		Vector3<T,FLOATTYPE> CrossProduct( Vector3<T,FLOATTYPE> *w ) 
		{
			Vector3<T,FLOATTYPE> v(
				m_y * w.m_z - m_z * w.m_y, 
				m_z * w.m_x - m_x * w.m_z,
				m_x * w.m_y - m_y * w.m_x);
			return v;
		}

		void Translate(FLOATTYPE x, FLOATTYPE y, FLOATTYPE z)
		{
			m_x += x;
			m_y += y;
			m_z += z;
		}

		Vector3<T, FLOATTYPE> ToNormalized()
		{
			Vector3<T, FLOATTYPE> v = *this;
			v.Normalize();
			return v;
		}

		///@brief Returns the angle between 2 vectors, which are also normalized.
		static FLOATTYPE Angle(Vector3<T,FLOATTYPE> *a, Vector3<T,FLOATTYPE> *b)
		{
			a->Normalize();
			b->Normalize();
			return a->m_x*b->m_x + a->m_y*b->m_y + a->m_z*b->m_z;
		}

		///@breif Adds 2 vectors
		static Vector3<T,FLOATTYPE> Add(Vector3<T,FLOATTYPE> *a, Vector3<T,FLOATTYPE> *b)
		{
			return Vector3<T,FLOATTYPE>(a->m_x+b->m_x, a->m_y+b->m_y, a->m_z+b->m_z);
		}

		///@brief Substracts 2 vectors
		static Vector3<T,FLOATTYPE> Sub(Vector3<T,FLOATTYPE> *a, Vector3<T,FLOATTYPE> *b)
		{
			return Vector3<T,FLOATTYPE>(a->m_x-b->m_x, a->m_y-b->m_y, a->m_z-b->m_z);
		}

		static Vector3<T,FLOATTYPE> Scale(FLOATTYPE f, Vector3<T,FLOATTYPE> *a)
		{
			return Vector3<T,FLOATTYPE>(f*a->m_x, f*a->m_y, f*a->m_z);
		}
		
		virtual void ValidateMem () const
		{
		}
		virtual void CheckMem () const
		{
		}
	};

	/** @brief Math vector in R4 */
	template <typename T, typename FLOATTYPE>
	class Vector4 : public Point4<T, FLOATTYPE>, public IMemoryValidate
	{
	protected:

	public:
		Vector4()
		: Point4<T,FLOATTYPE>()
		{
		}

		Vector4(T x)
		: Point4<T,FLOATTYPE>(x)
		{
		}

		Vector4(T x, T y)
		: Point4<T,FLOATTYPE>(x, y)
		{
		}

		Vector4(T x, T y, T z)
		: Point4<T,FLOATTYPE>(x, y, z)
		{
		}

		Vector4(T x, T y, T z, T w)
		: Point4<T,FLOATTYPE>(x, y, z, w)
		{
		}

		Vector4(const Vector4<T, FLOATTYPE>& v)
		: Point4<T, FLOATTYPE>(v)
		{
		}

		inline T X() { return m_x; }
		inline T Y() { return m_y; }
		inline T Z() { return m_z; }
		inline T W() { return m_w; }

		/// @brief Lenght of this vector
		inline FLOATTYPE Length()
		{
			return (FLOATTYPE)FastMath::Sqrt((FLOATTYPE)(m_x*m_x+m_y*m_y+m_z*m_z+m_w*m_w));
		}

		///@brief Normalizes this vector
		void Normalize()
		{
			FLOATTYPE dist = Length();
			if (dist == 0) 
			{
				return;
			}
			FLOATTYPE invdist = 1 / dist;
			m_x = (T)(m_x * invdist);
			m_y = (T)(m_y * invdist);
			m_z = (T)(m_z * invdist);
			m_w = (T)(m_w * invdist);
		}

		///@brief Reverses this vector
		void Reverse()
		{
			m_x = -m_x;
			m_y = -m_y;
			m_z = -m_z;
			m_w = -m_w;
		}


		///@brief Returns a modified vector by matrix m
		Vector4<T,FLOATTYPE> Transform(Matrix4<T,FLOATTYPE> *m)
		{
			return Vector4<T,FLOATTYPE>(
				m_x*m->m00 + m_y*m->m01 + m_z*m->m02+ m_w*m->m03,
				m_x*m->m10 + m_y*m->m11 + m_z*m->m12+ m_w*m->m13,
				m_x*m->m20 + m_y*m->m21 + m_z*m->m22+ m_w*m->m23
			);
		}
		
		///@breif Returns the cylindric coordinates out of the given cartesian coordinates
		static void GetCylindric(T x, T y, FLOATTYPE *r, FLOATTYPE *theta)
		{
			*r = (FLOATTYPE)FastMath::Sqrt(x*x + y*y);
			*theta = (FLOATTYPE)Math::Atan2(x, y);
		}

		static void GetCartesian(T *x, T *y, FLOATTYPE r, FLOATTYPE theta)
		// Builds the cartesian coordinates out of the given cylindric coordinates
		{
			*x = r * FastMath::Cos(theta);
			*y = r * FastMath::Sin(theta);
		}

		///@brief Returns the normal vector of the plane defined by the two vectors
		static Vector4<T,FLOATTYPE> GetNormal(Vector4<T,FLOATTYPE> *a, Vector4<T,FLOATTYPE> *b)
		{
			Vector4<T,FLOATTYPE> v = VectorProduct(a, b);
			v.Normalize();
			return v;
		}

		///@brief Returns the normal vector of the plane defined by the two vectors
		static Vector4<T,FLOATTYPE> GetNormal(Vector4<T,FLOATTYPE> *a, Vector4<T,FLOATTYPE> *b, Vector4<T,FLOATTYPE> *c)
		{
			Vector4<T,FLOATTYPE> v = VectorProduct(a,b,c);
			v.Normalize();
			return v;
		}

		///@brief Returns a x b
		static Vector4<T,FLOATTYPE> VectorProduct(Vector4<T,FLOATTYPE> *a, Vector4<T,FLOATTYPE> *b)
		{
			return Vector4<T,FLOATTYPE>(a->m_y*b->m_z-b->m_y*a->m_z,a->m_z*b->m_x-b->m_z*a->m_x,a->m_x*b->m_y-b->m_x*a->m_y);
		}

		///@brief Returns (b-a) x (c-a)
		static Vector4<T,FLOATTYPE> VectorProduct(Vector4<T,FLOATTYPE> *a, Vector4<T,FLOATTYPE> *b, Vector4<T,FLOATTYPE> *c)
		{
			Vector4<T,FLOATTYPE> v1 = Sub(b, a);
			Vector4<T,FLOATTYPE> v2 = Sub(c, a);
			return VectorProduct(&v1, &v2);
		}

		inline FLOATTYPE DotProduct(Vector4<T,FLOATTYPE> *b)
		{
			return m_x*b->m_x + m_y*b->m_y + m_z*b->m_z + m_w*b->m_w;
		}

		///@brief 3D Exterior Cross Product
		Vector4<T,FLOATTYPE> CrossProduct( Vector4<T,FLOATTYPE> *w ) 
		{
			Vector4<T,FLOATTYPE> v(
				m_y * w.m_z - z * w.m_y, 
				m_z * w.m_x - m_x * w.m_z,
				m_x * w.m_y - m_y * w.m_x);
			return v;
		}

		///@brief Returns the angle between 2 vectors, which are also normalized.
		static FLOATTYPE Angle(Vector4<T,FLOATTYPE> *a, Vector4<T,FLOATTYPE> *b)
		{
			a->Normalize();
			b->Normalize();
			return a->m_x*b->m_x + a->m_y*b->m_y + a->m_z*b->m_z + a->m_w*b->m_w;
		}

		///@breif Adds 2 vectors
		static Vector4<T,FLOATTYPE> Add(Vector4<T,FLOATTYPE> *a, Vector4<T,FLOATTYPE> *b)
		{
			return Vector4<T,FLOATTYPE>(a->m_x+b->m_x, a->m_y+b->m_y, a->m_z+b->m_z, a->m_w+b->m_w);
		}

		///@brief Substracts 2 vectors
		static Vector4<T,FLOATTYPE> Sub(Vector4<T,FLOATTYPE> *a, Vector4<T,FLOATTYPE> *b)
		{
			return Vector4<T,FLOATTYPE>(a->m_x-b->m_x, a->m_y-b->m_y, a->m_z-b->m_z, a->m_w-b->m_w);
		}

		static Vector4<T,FLOATTYPE> Scale(FLOATTYPE f, Vector4<T,FLOATTYPE> *a)
		{
			return Vector4<T,FLOATTYPE>(f*a->m_x, f*a->m_y, f*a->m_z, f*a->m_w);
		}

		Vector4<T, FLOATTYPE> ToNormalized()
		{
			Vector4<T, FLOATTYPE> v = *this;
			v.Normalize();
			return v;
		}

		virtual void ValidateMem () const
		{
		}
		virtual void CheckMem () const
		{
		}
	};

	#include <spl/math/Matrix.h>

	/** @} */
}
#endif
