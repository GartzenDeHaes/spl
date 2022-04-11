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
#ifndef _matrix4_h
#define _matrix4_h

#include <spl/types.h>
#include <spl/Debug.h>
#include <spl/Memory.h>
#include <spl/math/Vector.h>

namespace spl
{
	/** 
	 * @defgroup math Math
	 * @{
	 */

	/** @brief 3X3 matrix; loops are unrolled for performance. */
	template<typename T, typename FLOATTYPE>
	class Matrix3 : public IMemoryValidate
	{
	protected:
		T m00, m01, m02;
		T m10, m11, m12;
		T m20, m21, m22;

	public:
		friend template Vector3<typename T, typename FLOATTYPE>;

		Matrix3()
		{
			SetIdentity();
		}

		Matrix3(const Matrix3& m)
		:	m00(m.m00), m01(m.m01), m02(m.m02),
			m10(m.m10), m11(m.m11), m12(m.m12),
			m20(m.m20), m21(m.m21), m22(m.m22)
		{
		}

		Matrix3(Vector3<T,FLOATTYPE> *right, Vector3<T,FLOATTYPE> * up, Vector3<T,FLOATTYPE> * forward)
		{
			m00 = right->X();
			m10 = right->Y();
			m20 = right->Z();
			m01 = up->X();
			m11 = up->Y();
			m21 = up->Z();
			m02 = forward->X();
			m12 = forward->Y();
			m22 = forward->Z();
		}

		///@brief Matrix for scaling
		static Matrix3<T,FLOATTYPE> ScaleMatrix(FLOATTYPE dx, FLOATTYPE dy, FLOATTYPE dz)
		{
			Matrix3<T,FLOATTYPE> m;
			m.m00 = dx;
			m.m11 = dy;
			m.m22 = dz;
			return m;
		}

		///@brief matrix for scaling
		static Matrix3<T,FLOATTYPE> ScaleMatrix(FLOATTYPE d)
		{
			return ScaleMatrix(d, d, d);
		}

		void Shift(FLOATTYPE dx, FLOATTYPE dy, FLOATTYPE dz)
		{
			Matrix3<T,FLOATTYPE> m = ShiftMatrix(dx, dy, dz);
			Transform( &m );
		}

		void Scale(FLOATTYPE dx, FLOATTYPE dy, FLOATTYPE dz)
		{
			Matrix3<T,FLOATTYPE> m = ScaleMatrix(dx, dy, dz);
			Transform( &m );
		}

		void Scale(FLOATTYPE d)
		{
			Matrix4<T,FLOATTYPE> m = ScaleMatrix(d);
			Transform( &m );
		}

		void Rotate(FLOATTYPE dx, FLOATTYPE dy, FLOATTYPE dz)
		{
			Matrix3<T,FLOATTYPE> m = RotateMatrix(dx, dy, dz);
			Transform( &m );
		}

		void ScaleSelf(FLOATTYPE dx, FLOATTYPE dy, FLOATTYPE dz)
		{
			Matrix3<T,FLOATTYPE> m = ScaleMatrix(dx, dy, dz);
			PreTransform( &m );
		}

		void ScaleSelf(FLOATTYPE d)
		{
			Matrix3<T,FLOATTYPE> m = ScaleMatrix(d);
			PreTransform( &m );
		}

		void RotateSelf(FLOATTYPE dx, FLOATTYPE dy, FLOATTYPE dz)
		{
			Matrix3<T,FLOATTYPE> m = RotateMatrix(dx, dy, dz);
			PreTransform( &m );
		}

		///@brief matrix for rotation
		static Matrix3<T,FLOATTYPE> RotateMatrix(FLOATTYPE dx, FLOATTYPE dy, FLOATTYPE dz)
		{
			Matrix3<T,FLOATTYPE> out;
			FLOATTYPE SIN;
			FLOATTYPE COS;

			if (dx != 0)
			{
				Matrix3<T,FLOATTYPE> m;
				SIN = FastMath::Sin(dx);
				COS = FastMath::Cos(dx);
				m.m11 = (T)COS;
				m.m12 = (T)SIN;
				m.m21 = (T)-SIN;
				m.m22 = (T)COS;
				out.Transform(&m);
			}
			if (dy != 0)
			{
				Matrix3<T,FLOATTYPE> m;
				SIN = FastMath::Sin(dy);
				COS = FastMath::Cos(dy);
				m.m00 = (T)COS;
				m.m02 = (T)SIN;
				m.m20 = (T)-SIN;
				m.m22 = (T)COS;
				out.Transform(&m);
			}
			if (dz != 0)
			{
				Matrix3<T,FLOATTYPE> m;
				SIN = FastMath::Sin(dz);
				COS = FastMath::Cos(dz);
				m.m00 = (T)COS;
				m.m01 = (T)SIN;
				m.m10 = (T)-SIN;
				m.m11 = (T)COS;
				out.Transform(&m);
			}
			return out;
		}

		///@brief transforms this matrix by matrix n from left (this=n x this)
		void Transform( Matrix3<T,FLOATTYPE> *n )
		{
			Matrix3<T,FLOATTYPE> m = *this;

			m00 = n->m00*m.m00 + n->m01*m.m10 + n->m02*m.m20;
			m01 = n->m00*m.m01 + n->m01*m.m11 + n->m02*m.m21;
			m02 = n->m00*m.m02 + n->m01*m.m12 + n->m02*m.m22;
			m10 = n->m10*m.m00 + n->m11*m.m10 + n->m12*m.m20;
			m11 = n->m10*m.m01 + n->m11*m.m11 + n->m12*m.m21;
			m12 = n->m10*m.m02 + n->m11*m.m12 + n->m12*m.m22;
			m20 = n->m20*m.m00 + n->m21*m.m10 + n->m22*m.m20;
			m21 = n->m20*m.m01 + n->m21*m.m11 + n->m22*m.m21;
			m22 = n->m20*m.m02 + n->m21*m.m12 + n->m22*m.m22;
		}

		///@brief Transforms this matrix by matrix n from right (this=this x n)
		void PreTransform( Matrix3<T,FLOATTYPE> *n )
		{
			Matrix3<T,FLOATTYPE> m = *this;

			m00 = m.m00*n->m00 + m.m01*n->m10 + m.m02*n->m20;
			m01 = m.m00*n->m01 + m.m01*n->m11 + m.m02*n->m21;
			m02 = m.m00*n->m02 + m.m01*n->m12 + m.m02*n->m22;
			m10 = m.m10*n->m00 + m.m11*n->m10 + m.m12*n->m20;
			m11 = m.m10*n->m01 + m.m11*n->m11 + m.m12*n->m21;
			m12 = m.m10*n->m02 + m.m11*n->m12 + m.m12*n->m22;
			m20 = m.m20*n->m00 + m.m21*n->m10 + m.m22*n->m20;
			m21 = m.m20*n->m01 + m.m21*n->m11 + m.m22*n->m21;
			m22 = m.m20*n->m02 + m.m21*n->m12 + m.m22*n->m22;
		}

		///@brief Returns m1 x m2
		static Matrix3<T,FLOATTYPE> Multiply(Matrix3<T,FLOATTYPE> *m1, Matrix3<T,FLOATTYPE> *m2)
		{
			Matrix3<T,FLOATTYPE> m;

			m.m00 = m1->m00*m2->m00 + m1->m01*m2->m10 + m1->m02*m2->m20;
			m.m01 = m1->m00*m2->m01 + m1->m01*m2->m11 + m1->m02*m2->m21;
			m.m02 = m1->m00*m2->m02 + m1->m01*m2->m12 + m1->m02*m2->m22;
			m.m10 = m1->m10*m2->m00 + m1->m11*m2->m10 + m1->m12*m2->m20;
			m.m11 = m1->m10*m2->m01 + m1->m11*m2->m11 + m1->m12*m2->m21;
			m.m12 = m1->m10*m2->m02 + m1->m11*m2->m12 + m1->m12*m2->m22;
			m.m20 = m1->m20*m2->m00 + m1->m21*m2->m10 + m1->m22*m2->m20;
			m.m21 = m1->m20*m2->m01 + m1->m21*m2->m11 + m1->m22*m2->m21;
			m.m22 = m1->m20*m2->m02 + m1->m21*m2->m12 + m1->m22*m2->m22;
			return m;
		}

		void MultEqual(Matrix3<T,FLOATTYPE> *m2)
		{
			T m00, m01, m02;
			T m10, m11, m12;
			T m20, m21, m22;

			mm00 = m00*m2->m00 + m01*m2->m10 + m02*m2->m20;
			mm01 = m00*m2->m01 + m01*m2->m11 + m02*m2->m21;
			mm02 = m00*m2->m02 + m01*m2->m12 + m02*m2->m22;
			mm10 = m10*m2->m00 + m11*m2->m10 + m12*m2->m20;
			mm11 = m10*m2->m01 + m11*m2->m11 + m12*m2->m21;
			mm12 = m10*m2->m02 + m11*m2->m12 + m12*m2->m22;
			mm20 = m20*m2->m00 + m21*m2->m10 + m22*m2->m20;
			mm21 = m20*m2->m01 + m21*m2->m11 + m22*m2->m21;
			mm22 = m20*m2->m02 + m21*m2->m12 + m22*m2->m22;

			m00 = mm00; 
			m01 = mm01; 
			m02 = mm02; 
			m10 = mm10; 
			m11 = mm11; 
			m12 = mm12; 
			m20 = mm20; 
			m21 = mm21; 
			m22 = mm22; 
		}

		///@brief Returns the inverse of this matrix
		Matrix3<T,FLOATTYPE> Inverse()
		{
			Matrix3<T,FLOATTYPE> m;

			FLOATTYPE q1 = m12;  FLOATTYPE q6 = m10*m01;  FLOATTYPE q7 = m10*m21;  FLOATTYPE q8 = m02;
			FLOATTYPE q13 = m20*m01;  FLOATTYPE q14 = m20*m11;  FLOATTYPE q21 = m02*m21;  
			FLOATTYPE q25 = m01*m12;  FLOATTYPE q27 = m02*m11;  
			FLOATTYPE q29 = m10*m22;  FLOATTYPE q31 = m20*m12;  
			FLOATTYPE q35 = m00*m22;  FLOATTYPE q37 = m20*m02;  
			FLOATTYPE q41 = m00*m12;  FLOATTYPE q43 = m10*m02;  
			FLOATTYPE q45 = m00*m11;  FLOATTYPE q48 = m00*m21;
			FLOATTYPE q49 = q45*m22-q48*q1-q6*m22+q7*q8;
			FLOATTYPE q50 = q13*q1-q14*q8;
			FLOATTYPE q51 = 1/(q49+q50);

			m.m00 = (T)((m11*m22-m11-m21*m12+m21+m12-m22)*q51);
			m.m01 = (T)(-(m01*m22-m01-q21)*q51);
			m.m02 = (T)((q25-q27)*q51);
			m.m10 = (T)(-(q29-q31)*q51);
			m.m11 = (T)((q35-q37)*q51);
			m.m12 = (T)(-(q41-q43)*q51);
			m.m20 = (T)((q7-q14)*q51);
			m.m21 = (T)(-(q48-q13)*q51);
			m.m22 = (T)((q45-q6)*q51);

			return m;
		}

		///@brief Resets the matrix
		void SetIdentity()
		{
			m00=1; m01=0; m02=0;
			m10=0; m11=1; m12=0;
			m20=0; m21=0; m22=1;
		}
		
		Matrix3& operator =(const Matrix3& m)
		{
			m00 = m.m00; m01 = m.m01; m02 = m.m02;
			m10 = m.m10; m11 = m.m11; m12 = m.m12;
			m20 = m.m20; m21 = m.m21; m22 = m.m22;
			return *this;
		}

		virtual void ValidateMem() const
		{
		}

		virtual void CheckMem() const
		{
		}
	};

	/** @brief 4X4 matrix; loops are unrolled for performance. */
	template<typename T, typename FLOATTYPE>
	class Matrix4 : public IMemoryValidate
	{
	protected:
		T m00, m01, m02, m03;
		T m10, m11, m12, m13;
		T m20, m21, m22, m23;
		T m30, m31, m32, m33;

	public:
		friend template Vector4<typename T, typename FLOATTYPE>;

		Matrix4()
		{
			SetIdentity();
		}

		Matrix4(const Matrix4& m)
		: m00(m.m00), m01(m.m01), m02(m.m02), m03(m.m03),
		  m10(m.m10), m11(m.m11), m12(m.m12), m13(m.m13),
		  m20(m.m20), m21(m.m21), m22(m.m22), m23(m.m23),
		  m30(m.m30), m31(m.m31), m32(m.m32), m33(m.m33)
		{
		}

		Matrix4(Vector4<T,FLOATTYPE> *right, Vector4<T,FLOATTYPE> * up, Vector4<T,FLOATTYPE> * forward)
		: m03(0), m13(0), m23(0), m33(1)
		{
			m00 = right->X();
			m10 = right->Y();
			m20 = right->Z();
			m30 = right->W();
			m01 = up->X();
			m11 = up->Y();
			m21 = up->Z();
			m31 = up->W();
			m02 = forward->X();
			m12 = forward->Y();
			m22 = forward->Z();
			m32 = forward->W();
		}

		///@brief Matrix for shifting
		static Matrix4<T,FLOATTYPE> ShiftMatrix(FLOATTYPE dx, FLOATTYPE dy, FLOATTYPE dz)
		{
			Matrix4<T,FLOATTYPE> m;
			m.m03=dx;
			m.m13=dy;
			m.m23=dz;
			return m;
		}

		///@brief Matrix for scaling
		static Matrix4<T,FLOATTYPE> ScaleMatrix(FLOATTYPE dx, FLOATTYPE dy, FLOATTYPE dz)
		{
			Matrix4<T,FLOATTYPE> m;
			m.m00=dx;
			m.m11=dy;
			m.m22=dz;
			return m;
		}

		///@brief matrix for scaling
		static Matrix4<T,FLOATTYPE> ScaleMatrix(FLOATTYPE d)
		{
			return ScaleMatrix(d, d, d);
		}

		void Shift(FLOATTYPE dx, FLOATTYPE dy, FLOATTYPE dz)
		{
			Matrix4<T,FLOATTYPE> m = ShiftMatrix(dx, dy, dz);
			Transform( &m );
		}

		void Scale(FLOATTYPE dx, FLOATTYPE dy, FLOATTYPE dz)
		{
			Matrix4<T,FLOATTYPE> m = ScaleMatrix(dx, dy, dz);
			Transform( &m );
		}

		void Scale(FLOATTYPE d)
		{
			Matrix4<T,FLOATTYPE> m = ScaleMatrix(d);
			Transform( &m );
		}

		void Rotate(FLOATTYPE dx, FLOATTYPE dy, FLOATTYPE dz)
		{
			Matrix4<T,FLOATTYPE> m = RotateMatrix(dx, dy, dz);
			Transform( &m );
		}

		void ScaleSelf(FLOATTYPE dx, FLOATTYPE dy, FLOATTYPE dz)
		{
			Matrix4<T,FLOATTYPE> m = ScaleMatrix(dx, dy, dz);
			PreTransform( &m );
		}

		void ScaleSelf(FLOATTYPE d)
		{
			Matrix4<T,FLOATTYPE> m = ScaleMatrix(d);
			PreTransform( &m );
		}

		void RotateSelf(FLOATTYPE dx, FLOATTYPE dy, FLOATTYPE dz)
		{
			Matrix4<T,FLOATTYPE> m = RotateMatrix(dx, dy, dz);
			PreTransform( &m );
		}

		///@brief matrix for rotation
		static Matrix4<T,FLOATTYPE> RotateMatrix(FLOATTYPE dx, FLOATTYPE dy, FLOATTYPE dz)
		{
			Matrix4<T,FLOATTYPE> out;
			FLOATTYPE SIN;
			FLOATTYPE COS;

			if (dx != 0)
			{
				Matrix4<T,FLOATTYPE> m;
				SIN = FastMath::Sin(dx);
				COS = FastMath::Cos(dx);
				m.m11 = COS;
				m.m12 = SIN;
				m.m21 = -SIN;
				m.m22 = COS;
				out.Transform(&m);
			}
			if (dy != 0)
			{
				Matrix4<T,FLOATTYPE> m;
				SIN = FastMath::Sin(dy);
				COS = FastMath::Cos(dy);
				m.m00 = COS;
				m.m02 = SIN;
				m.m20 = -SIN;
				m.m22 = COS;
				out.Transform(&m);
			}
			if (dz != 0)
			{
				Matrix4<T,FLOATTYPE> m;
				SIN = FastMath::Sin(dz);
				COS = FastMath::Cos(dz);
				m.m00 = COS;
				m.m01 = SIN;
				m.m10 = -SIN;
				m.m11 = COS;
				out.Transform(&m);
			}
			return out;
		}

		///@brief transforms this matrix by matrix n from left (this=n x this)
		void Transform( Matrix4<T,FLOATTYPE> *n )
		{
			Matrix4<T,FLOATTYPE> m = *this;

			m00 = n->m00*m.m00 + n->m01*m.m10 + n->m02*m.m20;
			m01 = n->m00*m.m01 + n->m01*m.m11 + n->m02*m.m21;
			m02 = n->m00*m.m02 + n->m01*m.m12 + n->m02*m.m22;
			m03 = n->m00*m.m03 + n->m01*m.m13 + n->m02*m.m23 + n->m03;
			m10 = n->m10*m.m00 + n->m11*m.m10 + n->m12*m.m20;
			m11 = n->m10*m.m01 + n->m11*m.m11 + n->m12*m.m21;
			m12 = n->m10*m.m02 + n->m11*m.m12 + n->m12*m.m22;
			m13 = n->m10*m.m03 + n->m11*m.m13 + n->m12*m.m23 + n->m13;
			m20 = n->m20*m.m00 + n->m21*m.m10 + n->m22*m.m20;
			m21 = n->m20*m.m01 + n->m21*m.m11 + n->m22*m.m21;
			m22 = n->m20*m.m02 + n->m21*m.m12 + n->m22*m.m22;
			m23 = n->m20*m.m03 + n->m21*m.m13 + n->m22*m.m23 + n->m23;
		}

		///@brief Transforms this matrix by matrix n from right (this=this x n)
		void PreTransform( Matrix4<T,FLOATTYPE> *n )
		{
			Matrix4<T,FLOATTYPE> m = *this;

			m00 = m.m00*n->m00 + m.m01*n->m10 + m.m02*n->m20;
			m01 = m.m00*n->m01 + m.m01*n->m11 + m.m02*n->m21;
			m02 = m.m00*n->m02 + m.m01*n->m12 + m.m02*n->m22;
			m03 = m.m00*n->m03 + m.m01*n->m13 + m.m02*n->m23 + m.m03;
			m10 = m.m10*n->m00 + m.m11*n->m10 + m.m12*n->m20;
			m11 = m.m10*n->m01 + m.m11*n->m11 + m.m12*n->m21;
			m12 = m.m10*n->m02 + m.m11*n->m12 + m.m12*n->m22;
			m13 = m.m10*n->m03 + m.m11*n->m13 + m.m12*n->m23 + m.m13;
			m20 = m.m20*n->m00 + m.m21*n->m10 + m.m22*n->m20;
			m21 = m.m20*n->m01 + m.m21*n->m11 + m.m22*n->m21;
			m22 = m.m20*n->m02 + m.m21*n->m12 + m.m22*n->m22;
			m23 = m.m20*n->m03 + m.m21*n->m13 + m.m22*n->m23 + m.m23;
		}

		///@brief Returns m1 x m2
		static Matrix4<T,FLOATTYPE> Multiply(Matrix4<T,FLOATTYPE> *m1, Matrix4<T,FLOATTYPE> *m2)
		{
			Matrix4<T,FLOATTYPE> m;

			m.m00 = m1->m00*m2->m00 + m1->m01*m2->m10 + m1->m02*m2->m20 + m1->m03*m2->m30;
			m.m01 = m1->m00*m2->m01 + m1->m01*m2->m11 + m1->m02*m2->m21 + m1->m03*m2->m31;
			m.m02 = m1->m00*m2->m02 + m1->m01*m2->m12 + m1->m02*m2->m22 + m1->m03*m2->m32;
			m.m03 = m1->m00*m2->m03 + m1->m01*m2->m13 + m1->m02*m2->m23 + m1->m03*m2->m33;
			m.m10 = m1->m10*m2->m00 + m1->m11*m2->m10 + m1->m12*m2->m20 + m1->m13*m2->m30;
			m.m11 = m1->m10*m2->m01 + m1->m11*m2->m11 + m1->m12*m2->m21 + m1->m13*m2->m31;
			m.m12 = m1->m10*m2->m02 + m1->m11*m2->m12 + m1->m12*m2->m22 + m1->m13*m2->m32;
			m.m13 = m1->m10*m2->m03 + m1->m11*m2->m13 + m1->m12*m2->m23 + m1->m13*m2->m33;
			m.m20 = m1->m20*m2->m00 + m1->m21*m2->m10 + m1->m22*m2->m20 + m1->m23*m2->m30;
			m.m21 = m1->m20*m2->m01 + m1->m21*m2->m11 + m1->m22*m2->m21 + m1->m23*m2->m31;
			m.m22 = m1->m20*m2->m02 + m1->m21*m2->m12 + m1->m22*m2->m22 + m1->m23*m2->m32;
			m.m23 = m1->m20*m2->m03 + m1->m21*m2->m13 + m1->m22*m2->m23 + m1->m23*m2->m33;
			m.m30 = m1->m30*m2->m00 + m1->m31*m2->m10 + m1->m32*m2->m20 + m1->m33*m2->m30;
			m.m31 = m1->m30*m2->m01 + m1->m31*m2->m11 + m1->m32*m2->m21 + m1->m33*m2->m31;
			m.m32 = m1->m30*m2->m02 + m1->m31*m2->m12 + m1->m32*m2->m22 + m1->m33*m2->m32;
			m.m33 = m1->m30*m2->m03 + m1->m31*m2->m13 + m1->m32*m2->m23 + m1->m33*m2->m33;
			return m;
		}

		void MultEqual( Matrix4<T,FLOATTYPE> *m2 )
		{
			T mm00, mm01, mm02, mm03;
			T mm10, mm11, mm12, mm13;
			T mm20, mm21, mm22, mm23;
			T mm30, mm31, mm32, mm33;

			mm00 = m00*m2->m00 + m01*m2->m10 + m02*m2->m20 + m03*m2->m30;
			mm01 = m00*m2->m01 + m01*m2->m11 + m02*m2->m21 + m03*m2->m31;
			mm02 = m00*m2->m02 + m01*m2->m12 + m02*m2->m22 + m03*m2->m32;
			mm03 = m00*m2->m03 + m01*m2->m13 + m02*m2->m23 + m03*m2->m33;
			mm10 = m10*m2->m00 + m11*m2->m10 + m12*m2->m20 + m13*m2->m30;
			mm11 = m10*m2->m01 + m11*m2->m11 + m12*m2->m21 + m13*m2->m31;
			mm12 = m10*m2->m02 + m11*m2->m12 + m12*m2->m22 + m13*m2->m32;
			mm13 = m10*m2->m03 + m11*m2->m13 + m12*m2->m23 + m13*m2->m33;
			mm20 = m20*m2->m00 + m21*m2->m10 + m22*m2->m20 + m23*m2->m30;
			mm21 = m20*m2->m01 + m21*m2->m11 + m22*m2->m21 + m23*m2->m31;
			mm22 = m20*m2->m02 + m21*m2->m12 + m22*m2->m22 + m23*m2->m32;
			mm23 = m20*m2->m03 + m21*m2->m13 + m22*m2->m23 + m23*m2->m33;
			mm30 = m30*m2->m00 + m31*m2->m10 + m32*m2->m20 + m33*m2->m30;
			mm31 = m30*m2->m01 + m31*m2->m11 + m32*m2->m21 + m33*m2->m31;
			mm32 = m30*m2->m02 + m31*m2->m12 + m32*m2->m22 + m33*m2->m32;
			mm33 = m30*m2->m03 + m31*m2->m13 + m32*m2->m23 + m33*m2->m33;

			m00 = mm00; 
			m01 = mm01; 
			m02 = mm02; 
			m03 = mm03;
			m10 = mm10; 
			m11 = mm11; 
			m12 = mm12; 
			m13 = mm13;
			m20 = mm20; 
			m21 = mm21; 
			m22 = mm22; 
			m23 = mm23;
			m30 = mm30; 
			m31 = mm31; 
			m32 = mm32; 
			m33 = mm33;
		}

		///@brief Returns the inverse of this matrix
		Matrix4<T,FLOATTYPE> Inverse()
		{
			Matrix4<T,FLOATTYPE> m;

			FLOATTYPE q1 = m12;  FLOATTYPE q6 = m10*m01;  FLOATTYPE q7 = m10*m21;  FLOATTYPE q8 = m02;
			FLOATTYPE q13 = m20*m01;  FLOATTYPE q14 = m20*m11;  FLOATTYPE q21 = m02*m21;  FLOATTYPE q22 = m03*m21;
			FLOATTYPE q25 = m01*m12;  FLOATTYPE q26 = m01*m13;  FLOATTYPE q27 = m02*m11;  FLOATTYPE q28 = m03*m11;
			FLOATTYPE q29 = m10*m22;  FLOATTYPE q30 = m10*m23;  FLOATTYPE q31 = m20*m12;  FLOATTYPE q32 = m20*m13;
			FLOATTYPE q35 = m00*m22;  FLOATTYPE q36 = m00*m23;  FLOATTYPE q37 = m20*m02;  FLOATTYPE q38 = m20*m03;
			FLOATTYPE q41 = m00*m12;  FLOATTYPE q42 = m00*m13;  FLOATTYPE q43 = m10*m02;  FLOATTYPE q44 = m10*m03;
			FLOATTYPE q45 = m00*m11;  FLOATTYPE q48 = m00*m21;
			FLOATTYPE q49 = q45*m22-q48*q1-q6*m22+q7*q8;
			FLOATTYPE q50 = q13*q1-q14*q8;
			FLOATTYPE q51 = 1/(q49+q50);

			m.m00 = (T)((m11*m22*m33-m11*m23*m32-m21*m12*m33+m21*m13*m32+m31*m12*m23-m31*m13*m22)*q51);
			m.m01 = (T)(-(m01*m22*m33-m01*m23*m32-q21*m33+q22*m32)*q51);
			m.m02 = (T)((q25*m33-q26*m32-q27*m33+q28*m32)*q51);
			m.m03 = (T)(-(q25*m23-q26*m22-q27*m23+q28*m22+q21*m13-q22*m12)*q51);
			m.m10 = (T)(-(q29*m33-q30*m32-q31*m33+q32*m32)*q51);
			m.m11 = (T)((q35*m33-q36*m32-q37*m33+q38*m32)*q51);
			m.m12 = (T)(-(q41*m33-q42*m32-q43*m33+q44*m32)*q51);
			m.m13 = (T)((q41*m23-q42*m22-q43*m23+q44*m22+q37*m13-q38*m12)*q51);
			m.m20 = (T)((q7*m33-q30*m31-q14*m33+q32*m31)*q51);
			m.m21 = (T)(-(q48*m33-q36*m31-q13*m33+q38*m31)*q51);
			m.m22 = (T)((q45*m33-q42*m31-q6*m33+q44*m31)*q51);
			m.m23 = (T)(-(q45*m23-q42*m21-q6*m23+q44*m21+q13*m13-q38*m11)*q51);

			return m;
		}

		///@brief Resets the matrix
		void SetIdentity()
		{
			m00=1; m01=0; m02=0; m03=0;
			m10=0; m11=1; m12=0; m13=0;
			m20=0; m21=0; m22=1; m23=0;
			m30=0; m31=0; m32=0; m33=1;
		}

		Matrix4& operator =(const Matrix4& m)
		{
			m00 = m.m00; m01 = m.m01; m02 = m.m02; m03 = m.m03;
			m10 = m.m10; m11 = m.m11; m12 = m.m12; m13 = m.m13;
			m20 = m.m20; m21 = m.m21; m22 = m.m22; m23 = m.m23;
			m30 = m.m30; m31 = m.m31; m32 = m.m32; m33 = m.m33;
		}

		virtual void ValidateMem() const
		{
		}

		virtual void CheckMem() const
		{
		}
	};

	/** @} */
}
#endif
