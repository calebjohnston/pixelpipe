#ifndef __CG_MAT3_H
#define __CG_MAT3_H
#if !defined(__GNUC__)
#  pragma once
#endif

/************************************************************************

  3x3 Matrix class

  $Id: mat3.h 427 2004-09-27 04:45:31Z garland $

 ************************************************************************/

#include "cg/vecmath/vec3.hpp"

namespace cg {
namespace vecmath {

template<class T> 
class Mat3
{
private:
    TVec3<T> row[3];

public:
    // Standard constructors
    //
    Mat3() { *this = 0.0; }
    template<class U> Mat3(const TVec3<U>& r0,const TVec3<U>& r1,const TVec3<U>& r2)
    	{ row[0]=r0; row[1]=r1; row[2]=r2; }
    template<class U> Mat3(const Mat3<U>& m) { *this = m; }

    // Descriptive interface
    //
    typedef T value_type;
    typedef TVec3<T> vector_type;
    typedef Mat3<T> inverse_type;
    static int dim() { return 3; }

    // Access methods
    // 
    T& operator()(int i, int j)       { return row[i][j]; }
    T  operator()(int i, int j) const { return row[i][j]; }
    TVec3<T>&       operator[](int i)       { return row[i]; }
    const TVec3<T>& operator[](int i) const { return row[i]; }
    inline TVec3<T> getColumn(int i) const { return TVec3<T>(row[0][i], row[1][i], row[2][i]); }
	inline TVec3<T> getRow(int i) const { return TVec3<T>(row[i]); }
	inline void clear() { row[0] = TVec3<T>(); row[1] = TVec3<T>(); row[2] = TVec3<T>(); }

    operator       T*()       { return row[0]; }
    operator const T*()       { return row[0]; }
    operator const T*() const { return row[0]; }



    // Assignment methods
    //
    inline Mat3<T>& operator=(const Mat3<T>& m);
    inline Mat3<T>& operator=(T s);

    inline Mat3<T>& operator+=(const Mat3<T>& m);
    inline Mat3<T>& operator-=(const Mat3<T>& m);
    inline Mat3<T>& operator*=(T s);
    inline Mat3<T>& operator/=(T s);


    // Construction of standard matrices
    //
    static Mat3<T> I()
	{
		return Mat3<T>(TVec3<T>(1,0,0), TVec3<T>(0,1,0), TVec3<T>(0,0,1));
	}
	
    static Mat3<T> outer_product(const TVec3<T>& u, const TVec3<T>& v)
	{
	    Mat3<T> A;

	    for(int i=0; i<3; i++)
		for(int j=0; j<3; j++)
		    A(i, j) = u[i]*v[j];

	    return A;
	}
	
	
    static Mat3<T> outer_product(const TVec3<T>& v)
	{
	    Mat3<T> A;
	    T x=v[0], y=v[1], z=v[2];

	    A(0,0) = x*x;  A(0,1) = x*y;  A(0,2) = x*z;
	    A(1,0)=A(0,1); A(1,1) = y*y;  A(1,2) = y*z;
	    A(2,0)=A(0,2); A(2,1)=A(1,2); A(2,2) = z*z;

	    return A;
	}
	
	inline void setColumn(int i, const TVec3<T>& v)
	{
		row[0][i] = v.x;
		row[1][i] = v.y;
		row[2][i] = v.z;
	}
	
	inline void setRow(int i, const TVec3<T>& v)
	{
		row[i] = v;
	}

    Mat3<T> &diag(T d)
	{
	    *this = 0.0;
	    row[0][0] = row[1][1] = row[2][2] = d;
	    return *this;
	}
	
    Mat3<T> &ident()
	{
		return diag(1.0);
	}


};

////////////////////////////////////////////////////////////////////////
//
// Methods definitions
//

template<class T> inline Mat3<T>& Mat3<T>::operator=(const Mat3<T>& m)
	{ row[0] = m[0]; row[1] = m[1]; row[2] = m[2];  return *this; }

template<class T> inline Mat3<T>& Mat3<T>::operator=(T s)
	{ row[0]=s;  row[1]=s;  row[2]=s;  return *this; }

template<class T> inline Mat3<T>& Mat3<T>::operator+=(const Mat3<T>& m)
	{ row[0] += m[0]; row[1] += m[1]; row[2] += m[2]; return *this; }

template<class T> inline Mat3<T>& Mat3<T>::operator-=(const Mat3<T>& m)
	{ row[0] -= m[0]; row[1] -= m[1]; row[2] -= m[2]; return *this; }

template<class T> inline Mat3<T>& Mat3<T>::operator*=(T s)
	{ row[0] *= s; row[1] *= s; row[2] *= s;  return *this; }

template<class T> inline Mat3<T>& Mat3<T>::operator/=(T s)
	{ row[0] /= s; row[1] /= s; row[2] /= s;  return *this; }

////////////////////////////////////////////////////////////////////////
//
// Operator definitions
//

template<class T> inline Mat3<T> operator+(const Mat3<T>& n, const Mat3<T>& m)
	{ return Mat3<T>(n[0]+m[0], n[1]+m[1], n[2]+m[2]); }

template<class T> inline Mat3<T> operator-(const Mat3<T>& n, const Mat3<T>& m)
	{ return Mat3<T>(n[0]-m[0], n[1]-m[1], n[2]-m[2]); }

template<class T> inline Mat3<T> operator-(const Mat3<T>& m)
	{ return Mat3<T>(-m[0], -m[1], -m[2]); }

template<class T> inline Mat3<T> operator*(T s, const Mat3<T>& m)
	{ return Mat3<T>(m[0]*s, m[1]*s, m[2]*s); }
template<class T> inline Mat3<T> operator*(const Mat3<T>& m, T s)
	{ return s*m; }

template<class T> inline Mat3<T> operator/(const Mat3<T>& m, T s)
	{ return Mat3<T>(m[0]/s, m[1]/s, m[2]/s); }

template<class T> inline TVec3<T> operator*(const Mat3<T>& m, const TVec3<T>& v)
	{ return TVec3<T>(m[0]*v, m[1]*v, m[2]*v); }

template<class T> Mat3<T> operator*(const Mat3<T>& n, const Mat3<T>& m)
	{
	    Mat3<T> A;

	    for(int i=0;i<3;i++)
		for(int j=0;j<3;j++)
		    A(i,j) = n[i]*m.getColumn(j);

	    return A;
	}

template<class T> inline std::ostream &operator<<(std::ostream &out, const Mat3<T>& M)
	{ return out << std::endl << M[0] << std::endl << M[1] << std::endl << M[2]; }

template<class T> inline std::istream &operator>>(std::istream &in, Mat3<T>& M)
	{ return in >> M[0] >> M[1] >> M[2]; }

////////////////////////////////////////////////////////////////////////
//
// Misc. function definitions
//

template<class T> inline T det(const Mat3<T>& m)
	{
		return m[0] * (m[1] ^ m[2]);
	}

template<class T> inline T trace(const Mat3<T>& m)
	{
		return m(0,0) + m(1,1) + m(2,2);
	}

template<class T> inline Mat3<T> transpose(const Mat3<T>& m)
	{
		return Mat3<T>(m.getColumn(0), m.getColumn(1), m.getColumn(2));
	}
	
template<class T> Mat3<T> adjoint(const Mat3<T>& m)
	{
	    return Mat3<T>(m[1]^m[2],
			m[2]^m[0],
			m[0]^m[1]);
	}

template<class T> T invert(Mat3<T>& m_inv, const Mat3<T>& m)
	{
	    Mat3<T> A = adjoint(m);
	    T d = A[0] * m[0];

	    if( d==0.0 )
		return 0.0;

	    m_inv = transpose(A) / d;
	    return d;
	}

template<class T> inline Mat3<T> row_extend(const TVec3<T>& v) { return Mat3<T>(v, v, v); }

template<class T> Mat3<T> diag(const TVec3<T>& v)
	{
	    return Mat3<T>(TVec3<T>(v[0],0,0),  TVec3<T>(0,v[1],0),  TVec3<T>(0,0,v[2]));
	}

template<class T> extern bool eigen(const Mat3<T>& m, TVec3<T>& eig_vals, TVec3<T> eig_vecs[3]);

typedef Mat3<double> Matrix3;
typedef Mat3<float>  Matrix3f;

} // namespace vecmath
} // namespace cg

#endif	// __CG_MAT3_H
