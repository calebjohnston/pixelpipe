#ifndef __CG_MAT2_H // -*- C++ -*-
#define __CG_MAT2_H
#if !defined(__GNUC__)
#  pragma once
#endif

/************************************************************************

  2x2 Matrix class
  
  $Id: mat2.h 427 2004-09-27 04:45:31Z garland $

 ************************************************************************/

#include "cg/vecmath/vec2.hpp"

namespace cg {
namespace vecmath {

/* IN THE PROCESS OF TEMPLATING THE MAT2 CLASS */

template<class T>
class Mat2
{
private:
    TVec2<T> row[2];

public:
    // Standard constructors
    //
    Mat2() { *this = 0.0; }
    Mat2(T a, T b, T c, T d)
	{ row[0][0]=a; row[0][1]=b; row[1][0]=c; row[1][1]=d; }
    template<class U> Mat2(const TVec2<U> &r0,const TVec2<U> &r1) { row[0]=r0; row[1]=r1; }
    template<class U> Mat2(const Mat2<U> &m) { *this = m; }

    // Descriptive interface
    //
    typedef T value_type;
    typedef TVec2<T> vector_type;
    typedef Mat2<T> inverse_type;
    static int dim() { return 2; }

    // Access methods        note: A(i, j) == row i, col j
    //
    T& operator()(int i, int j)       { return row[i][j]; }
    T  operator()(int i, int j) const { return row[i][j]; }
    TVec2<T>&       operator[](int i)       { return row[i]; }
    const TVec2<T>& operator[](int i) const { return row[i]; }
    inline TVec2<T> getColumn(int i) const { return TVec2<T>(row[0][i],row[1][i]); }
	inline TVec2<T> getRow(int i) const { return TVec2<T>(row[i]); }

    operator       T*()       { return row[0]; }
    operator const T*()       { return row[0]; }
    operator const T*() const { return row[0]; }


    // Assignment methods
    //
    inline Mat2<T>& operator=(const Mat2<T>& m);
    inline Mat2<T>& operator=(T s);
    inline Mat2<T>& operator+=(const Mat2<T>& m);
    inline Mat2<T>& operator-=(const Mat2<T>& m);
    inline Mat2<T>& operator*=(T s);
    inline Mat2<T>& operator/=(T s);


    // Construction of standard matrices
    //
	static Mat2<T> I();
	static Mat2<T> outer_product(const TVec2<T> &u, const TVec2<T> &v)
	{
		return Mat2<T>(u[0]*v[0], u[0]*v[1], u[1]*v[0], u[1]*v[1]);
	}
	
	static Mat2<T> outer_product(const TVec2<T> &u)
	{
		return outer_product(u,u);
	}
	
	inline void setColumn(int i, const TVec2<T>& v);
	inline void setRow(int i, const TVec2<T>& v);

	Mat2<T> &diag(T d)
	{
	    row[0][0] = d;   row[0][1] = 0;
	    row[1][0] = 0;   row[1][1] = d;

	    return *this;
	}
	
    Mat2<T> &ident()
	{
		return diag(1.0);
	}
};

////////////////////////////////////////////////////////////////////////
//
// Method definitions
//

template<class T> inline Mat2<T>& Mat2<T>::operator=(const Mat2<T>& m)
	{ row[0]=m[0];  row[1]=m[1];  return *this; }

template<class T> inline Mat2<T>& Mat2<T>::operator=(T s)
	{ row[0]=s;  row[1]=s;  return *this; }

template<class T> inline Mat2<T>& Mat2<T>::operator+=(const Mat2<T>& m)
	{ row[0] += m.row[0]; row[1] += m.row[1];  return *this;}

template<class T> inline Mat2<T>& Mat2<T>::operator-=(const Mat2<T>& m)
	{ row[0] -= m.row[0]; row[1] -= m.row[1];  return *this; }

template<class T> inline Mat2<T>& Mat2<T>::operator*=(T s)
	{ row[0] *= s; row[1] *= s;  return *this; }

template<class T> inline Mat2<T>& Mat2<T>::operator/=(T s)
	{ row[0] /= s; row[1] /= s;  return *this; }

////////////////////////////////////////////////////////////////////////
//
// Operator definitions
//

template<class T> inline Mat2<T> operator+(const Mat2<T> &n, const Mat2<T> &m)
	{ return Mat2<T>(n[0]+m[0], n[1]+m[1]); }

template<class T> inline Mat2<T> operator-(const Mat2<T> &n, const Mat2<T> &m)
	{ return Mat2<T>(n[0]-m[0], n[1]-m[1]); }

template<class T> inline Mat2<T> operator-(const Mat2<T> &m)
	{ return Mat2<T>(-m[0], -m[1]); }

template<class T> inline Mat2<T> operator*(T s, const Mat2<T> &m)
	{ return Mat2<T>(m[0]*s, m[1]*s); }
	
template<class T> inline Mat2<T> operator*(const Mat2<T> &m, T s)
	{ return s*m; }

template<class T> inline Mat2<T> operator/(const Mat2<T> &m, T s)
	{ return Mat2<T>(m[0]/s, m[1]/s); }

template<class T> inline TVec2<T> operator*(const Mat2<T> &m, const TVec2<T> &v)
	{ return TVec2<T>(m[0]*v, m[1]*v); }

template<class T> Mat2<T> operator*(const Mat2<T> &n, const Mat2<T> &m)
	{
	    Mat2<T> A;
	    int i,j;

	    for(i=0;i<2;i++)
		for(j=0;j<2;j++)
		    A(i,j) = n[i]*m.getColumn(j);

	    return A;
	}

template<class T> inline std::ostream &operator<<(std::ostream &out, const Mat2<T>& M)
	{ return out << std::endl << M[0] << std::endl << M[1]; }

template<class T> inline std::istream &operator>>(std::istream &in, Mat2<T>& M)
	{ return in >> M[0] >> M[1]; }

////////////////////////////////////////////////////////////////////////
//
// Misc. function definitions
//

template<class T> inline T det(const Mat2<T> &m)
	{ return m(0,0)*m(1,1) - m(0,1)*m(1,0); }

template<class T> inline T trace(const Mat2<T> &m)
	{ return m(0,0) + m(1,1); }

template<class T> inline Mat2<T> transpose(const Mat2<T> &m)
	{ return Mat2<T>(m.getColumn(0), m.getColumn(1)); }

template<class T> inline Mat2<T> adjoint(const Mat2<T> &m)
	{ return Mat2<T>(perp(m[1]), -perp(m[0])); }

template<class T> extern T invert(Mat2<T> &m_inv, const Mat2<T> &m)
	{
	    T d = det(m);

	    if( d==0.0 )
		return 0.0;

	    m_inv(0, 0) =  m(1,1)/d;
	    m_inv(0, 1) = -m(0,1)/d;
	    m_inv(1, 0) = -m(1,0)/d;
	    m_inv(1, 1) =  m(0,0)/d;

	    return d;
	}
	
template<class T> bool eigenvalues(const Mat2<T>& M, TVec2<T>& evals)
	{
	    T B = -M(0,0) - M(1,1);
	    T C = det(M);

	    T dis = B*B - 4.0*C;
	    if( dis<FEQ_EPS )
		return false;
	    else
	    {
		T s = sqrt(dis);

		evals[0] = 0.5*(-B + s);
		evals[1] = 0.5*(-B - s);
		return true;
	    }
	}
template<class T> bool eigenvectors(const Mat2<T>& M, const TVec2<T>& evals, TVec2<T> evecs[2])
	{
	    evecs[0] = TVec2<T>(-M(0,1), M(0,0)-evals[0]);
	    evecs[1] = TVec2<T>(-M(0,1), M(0,0)-evals[1]);

	    unitize(evecs[0]);
	    unitize(evecs[1]);

	    return true;
	}
	
template<class T> bool eigen(const Mat2<T>& M, TVec2<T>& evals, TVec2<T> evecs[2])
	{
	    bool result = eigenvalues(M, evals);
	    if( result )
		eigenvectors(M, evals, evecs);
	    return result;
	}

template<class T> Mat2<T> Mat2<T>::I() { return Mat2<T>(1,0,  0,1); }

template<class T> inline void Mat2<T>::setColumn(int i, const TVec2<T>& v)
	{ row[0][i] = v.x; row[1][i] = v.y; }

template<class T> inline void Mat2<T>::setRow(int i, const TVec2<T>& v) { row[i] = v; }

typedef Mat2<double> Matrix2;
typedef Mat2<float>  Matrix2f;

} // namespace vecmath
} // namespace cg

#endif	// __CG_MAT2_H
