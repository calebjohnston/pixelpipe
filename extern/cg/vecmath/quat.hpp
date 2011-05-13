#ifndef __GFX_QUAT_INCLUDED // -*- C++ -*-
#define __GFX_QUAT_INCLUDED
#if !defined(__GNUC__)
#  pragma once
#endif

/************************************************************************

  TQuaternion class
  
  $Id: quat.h 440 2005-02-23 05:14:13Z garland $

 ************************************************************************/

#include "cg/vecmath/mat4.hpp"

namespace cg {
namespace vecmath {

template<class T>
class TQuat
{
private:
    TVec3<T> v;		// Vector component
    T s;			// Scalar component

public:
    TQuat() { v=0.0; s=1.0; }
    TQuat(T x, T y, T z, T w) { v[0]=x;v[1]=y;v[2]=z; s=w; }
    template<class U> TQuat(const TVec3<U>& a, T b) { v=a; s=b; }
    template<class U> TQuat(const TQuat<U>& q) { *this=q; }

    // Access methods
    const TVec3<T>& vector() const { return v; }
    TVec3<T>&       vector()       { return v; }
    T      scalar() const { return s; }
    T&     scalar()       { return s; }

    // Assignment and in-place arithmetic methods
    TQuat<T>& operator=(const TQuat<T>& q);
    TQuat<T>& operator+=(const TQuat<T>& q);
    TQuat<T>& operator-=(const TQuat<T>& q);
    TQuat<T>& operator=(T d);
    TQuat<T>& operator*=(T d);
    TQuat<T>& operator/=(T d);

    // Construction of standard quaternions
    static TQuat<T> ident();
};

////////////////////////////////////////////////////////////////////////
//
// Implementation of TQuat methods
//

template<class T> inline TQuat<T>& TQuat<T>::operator=(const TQuat<T>& q) { v=q.v; s=q.s; return *this; }
template<class T> inline TQuat<T>& TQuat<T>::operator+=(const TQuat<T>& q) { v+=q.v; s+=q.s; return *this; }
template<class T> inline TQuat<T>& TQuat<T>::operator-=(const TQuat<T>& q) { v-=q.v; s-=q.s; return *this; }
template<class T> inline TQuat<T>& TQuat<T>::operator=(T d)  { v=d;  s=d;  return *this; }
template<class T> inline TQuat<T>& TQuat<T>::operator*=(T d) { v*=d; s*=d; return *this; }
template<class T> inline TQuat<T>& TQuat<T>::operator/=(T d) { v/=d; s/=d; return *this; }
template<class T> inline TQuat<T> TQuat<T>::ident() { return TQuat<T>(0, 0, 0, 1); }

////////////////////////////////////////////////////////////////////////
//
// Standard arithmetic operators on quaternions
//

template<class T> inline TQuat<T> operator+(const TQuat<T>& q, const TQuat<T>& r)
	{ return TQuat<T>(q.vector()+r.vector(), q.scalar()+r.scalar()); }

template<class T> inline TQuat<T> operator*(const TQuat<T>& q, const TQuat<T>& r)
{
    return TQuat<T>(cross(q.vector(),r.vector()) +
		r.scalar()*q.vector() +
		q.scalar()*r.vector(),
		q.scalar()*r.scalar() - q.vector()*r.vector());
}

template<class T> inline TQuat<T> operator*(const TQuat<T>& q, T s)
	{ return TQuat<T>(q.vector()*s, q.scalar()*s); }
template<class T> inline TQuat<T> operator*(T s, const TQuat<T>& q)
	{ return TQuat<T>(q.vector()*s, q.scalar()*s); }

template<class T> inline TQuat<T> operator/(const TQuat<T>& q, T s)
	{ return TQuat<T>(q.vector()/s, q.scalar()/s); }

template<class T> inline std::ostream &operator<<(std::ostream &out, const TQuat<T>& q)
	{ return out << q.vector() << " " << q.scalar(); }

template<class T> inline std::istream &operator>>(std::istream &in, TQuat<T>& q)
	{ return in >> q.vector() >> q.scalar(); }


////////////////////////////////////////////////////////////////////////
//
// Standard functions on quaternions
//

template<class T> inline T norm(const TQuat<T>& q)
	{ return q.scalar()*q.scalar() + q.vector()*q.vector(); }

template<class T> inline TQuat<T> conjugate(const TQuat<T>& q) { return TQuat<T>(-q.vector(), q.scalar()); }
template<class T> inline TQuat<T> inverse(const TQuat<T>& q) { return conjugate(q)/norm(q); }
template<class T> inline TQuat<T>& unitize(TQuat<T>& q)  { q /= sqrt(norm(q)); return q; }

template<class T> TQuat<T> exp(const TQuat<T>& q)
	{
	    T theta = norm(q.vector());
	    T c = cos(theta);

	    if( theta > FEQ_EPS )
	    {
		T s = sin(theta) / theta;
		return TQuat<T>( s*q.vector(), c);
	    }
	    else
		return TQuat<T>(q.vector(), c);
	}
	
template<class T> TQuat<T> log(const TQuat<T>& q)
	{
	    T scale = norm(q.vector());
	    T theta = atan2(scale, q.scalar());

	    if( scale > 0.0 )  scale=theta/scale;

	    return TQuat<T>(scale*q.vector(), 0.0);
	}
	
template<class T> TQuat<T> axis_to_quat(const TVec3<T>& a, T phi)
	{
	    TVec3<T> u = a;
	    unitize(u);

	    T s = sin(phi/2.0);
	    return TQuat<T>(u[0]*s, u[1]*s, u[2]*s, cos(phi/2.0));
	}
	
template<class T> Mat4<T> quat_to_matrix(const TQuat<T>& q)
	{
	    Mat4<T> M;

	    const T x = q.vector()[0];
	    const T y = q.vector()[1];
	    const T z = q.vector()[2];
	    const T w = q.scalar();
	    const T s = 2/norm(q);

	    M(0,0)=1-s*(y*y+z*z); M(0,1)=s*(x*y-w*z);   M(0,2)=s*(x*z+w*y);   M(0,3)=0;
	    M(1,0)=s*(x*y+w*z);   M(1,1)=1-s*(x*x+z*z); M(1,2)=s*(y*z-w*x);   M(1,3)=0;
	    M(2,0)=s*(x*z-w*y);   M(2,1)=s*(y*z+w*x);   M(2,2)=1-s*(x*x+y*y); M(2,3)=0;
	    M(3,0)=0;             M(3,1)=0;             M(3,2)=0;             M(3,3)=1;

	    return M;
	}
	
template<class T> Mat4<T> unit_quat_to_matrix(const TQuat<T>& q)
	{
	    Mat4<T> M;

	    const T x = q.vector()[0];
	    const T y = q.vector()[1];
	    const T z = q.vector()[2];
	    const T w = q.scalar();

	    M(0,0)=1-2*(y*y+z*z); M(0,1)=2*(x*y-w*z);   M(0,2)=2*(x*z+w*y);   M(0,3)=0;
	    M(1,0)=2*(x*y+w*z);   M(1,1)=1-2*(x*x+z*z); M(1,2)=2*(y*z-w*x);   M(1,3)=0;
	    M(2,0)=2*(x*z-w*y);   M(2,1)=2*(y*z+w*x);   M(2,2)=1-2*(x*x+y*y); M(2,3)=0;
	    M(3,0)=0;             M(3,1)=0;             M(3,2)=0;             M(3,3)=1;

	    return M;
	}
	
template<class T> TQuat<T> slerp(const TQuat<T>& from, const TQuat<T>& to, T t)
	{
	    const TVec3<T>& v_from = from.vector();
	    const TVec3<T>& v_to = to.vector();
	    const T s_from = from.scalar();
	    const T s_to = to.scalar();

	    T cosine = v_from*v_to + s_from*s_to;

	    if( (1+cosine) < FEQ_EPS )
	    {
		// The quaternions are (nearly) diametrically opposed.  We
		// treat this specially (based on suggestion in Watt & Watt).
		//
		T A = sin( (1-t)*M_PI/2.0 );
		T B = sin( t*M_PI/2.0 );

		return TQuat<T>( A*v_from[0] + B*(-v_from[1]),
			     A*v_from[1] + B*(v_from[0]),
			     A*v_from[2] + B*(-s_from),
			     A*s_from    + B*(v_from[2]) );
	    }

	    T A, B;
	    if( (1-cosine) < FEQ_EPS )
	    {
		// The quaternions are very close.  Approximate with normal
		// linear interpolation.  This is cheaper and avoids division
		// by very small numbers.
		//
		A = 1.0 - t;
		B = t;
	    }
	    else
	    {
		// This is the normal case.  Perform SLERP.
		//
		T theta = acos(cosine);
		T sine = sqrt(1 - cosine*cosine);

		A = sin( (1-t)*theta ) / sine;
		B = sin( t*theta ) / sine;

	    }

	    return TQuat<T>( A*v_from + B*v_to,  A*s_from + B*s_to);
	}


typedef TQuat<double> Quaternion;
typedef TQuat<float>  Quaternionf;

} // namespace vecmath
} // namespace cg

// __GFX_QUAT_INCLUDED
#endif
