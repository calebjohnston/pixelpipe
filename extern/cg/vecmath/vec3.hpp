#ifndef __GFX_VEC3_INCLUDED  
#define __GFX_VEC3_INCLUDED

/************************************************************************

  3D Vector class

  $Id: vec3.h 427 2004-09-27 04:45:31Z garland $

 ************************************************************************/

#include "cg/vecmath/vec2.hpp"

namespace cg {
namespace vecmath {

template<class T>
class TVec3 {
public:
	union {
		struct {
			T x, y, z;
		};
		T elt[3];
	};
	//T elt[3];

    // Standard constructors
    //
    TVec3(T s=0) { *this = s; }
    TVec3(T x, T y, T z) { elt[0]=x; elt[1]=y; elt[2]=z; }

    // Copy constructors & assignment operators
    template<class U> TVec3(const TVec3<U>& v) { *this = v; }
    TVec3(const T *v) { elt[0]=v[0]; elt[1]=v[1]; elt[2]=v[2]; }

    template<class U> TVec3& operator=(const TVec3<U>& v)
	{ elt[0]=v[0];  elt[1]=v[1];  elt[2]=v[2];  return *this; }
    TVec3& operator=(T s) { elt[0]=elt[1]=elt[2]=s; return *this; }

    // Descriptive interface
    //
    typedef T value_type;
    static int dim() { return 3; }


    // Access methods
    //
    operator       T*()       { return elt; }
    operator const T*() const { return elt; }

#ifndef HAVE_CASTING_LIMITS
    T& operator[](int i)       { return elt[i]; }
    T  operator[](int i) const { return elt[i]; }
    operator const T*()       { return elt; }
#endif


    // Assignment and in-place arithmetic methods
    //
    inline TVec3& operator+=(const TVec3& v);
    inline TVec3& operator-=(const TVec3& v);
    inline TVec3& operator*=(T s);
    inline TVec3& operator/=(T s);
	inline T lengthSquared() { return elt[0]*elt[0] + elt[1]*elt[1] + elt[2]*elt[2]; }
	inline T length() { return sqrt(lengthSquared()); }
	inline void set(T _x, T _y, T _z) { elt[0] = _x, elt[1] = _y; elt[2] = _z; }
};

////////////////////////////////////////////////////////////////////////
//
// Method definitions
//

template<class T> inline TVec3<T>& TVec3<T>::operator+=(const TVec3<T>& v)
	{ elt[0] += v[0];   elt[1] += v[1];   elt[2] += v[2];  return *this; }

template<class T> inline TVec3<T>& TVec3<T>::operator-=(const TVec3<T>& v)
	{ elt[0] -= v[0];   elt[1] -= v[1];   elt[2] -= v[2];  return *this; }

template<class T> inline TVec3<T>& TVec3<T>::operator*=(T s)
	{ elt[0] *= s;   elt[1] *= s;   elt[2] *= s;  return *this; }

template<class T> inline TVec3<T>& TVec3<T>::operator/=(T s)
	{ elt[0] /= s;   elt[1] /= s;   elt[2] /= s;  return *this; }


////////////////////////////////////////////////////////////////////////
//
// Operator definitions
//

template<class T>
inline TVec3<T> operator+(const TVec3<T> &u, const TVec3<T>& v)
	{ return TVec3<T>(u[0]+v[0], u[1]+v[1], u[2]+v[2]); }

template<class T>
inline TVec3<T> operator-(const TVec3<T> &u, const TVec3<T>& v)
	{ return TVec3<T>(u[0]-v[0], u[1]-v[1], u[2]-v[2]); }

template<class T> inline TVec3<T> operator-(const TVec3<T> &v)
	{ return TVec3<T>(-v[0], -v[1], -v[2]); }

#if _MSC_VER>=1200
// Normally, we use the <class T, class N> construct below to allow the scalar
// argument to be different than the template type.  This, for example, allows
// the user to write things like v/2.  Unfortunately, Microsoft VC6.0 (aka
// v1200) gets confused by this.  We used to include explicit versions for the
// case of int's, but this was causing silent (and incorrect) coercion of
// floats to ints.
//
  template<class T> inline TVec3<T> operator*(T s, const TVec3<T> &v)
	{ return TVec3<T>(v[0]*s, v[1]*s, v[2]*s); }
  template<class T> inline TVec3<T> operator*(const TVec3<T> &v, T s)
	{ return s*v; }

  template<class T> inline TVec3<T> operator/(const TVec3<T> &v, T s)
	{ return TVec3<T>(v[0]/s, v[1]/s, v[2]/s); }
#else
  template<class T, class N> inline TVec3<T> operator*(N s, const TVec3<T> &v)
	{ return TVec3<T>(v[0]*s, v[1]*s, v[2]*s); }
  template<class T, class N> inline TVec3<T> operator*(const TVec3<T> &v, N s)
	{ return s*v; }

  template<class T, class N> inline TVec3<T> operator/(const TVec3<T> &v, N s)
	{ return TVec3<T>(v[0]/s, v[1]/s, v[2]/s); }
#endif

template<class T> inline T operator*(const TVec3<T> &u, const TVec3<T>& v)
	{ return u[0]*v[0] + u[1]*v[1] + u[2]*v[2]; }

template<class T> inline TVec3<T> cross(const TVec3<T>& u, const TVec3<T>& v)
{
    return TVec3<T>( u[1]*v[2] - v[1]*u[2],
		-u[0]*v[2] + v[0]*u[2],
		 u[0]*v[1] - v[0]*u[1] );
}

template<class T>
inline TVec3<T> operator^(const TVec3<T>& u, const TVec3<T>& v)
	{ return cross(u, v); }


template<class T>
inline std::ostream &operator<<(std::ostream &out, const TVec3<T>& v)
	{ return out << "[" << v[0] << ", " << v[1] << ", " << v[2] << "]"; }

template<class T>
inline std::istream &operator>>(std::istream &in, TVec3<T>& v)
	{ return in >> v[0] >> v[1] >> v[2]; }

////////////////////////////////////////////////////////////////////////
//
// Misc. function definitions
//

template<class T> inline T norm2(const TVec3<T>& v)	{ return v*v; }
template<class T> inline T norm(const TVec3<T>& v)	{ return sqrt(norm2(v)); }
template<class T> inline T dot(const TVec3<T>&v1, const TVec3<T>&v2) { return v1[0]*v2[0] + v1[1]*v2[1] + v1[2]*v2[2]; }

template<class T> inline void normalize(TVec3<T>& v)
{
    T l = norm2(v);
    if( l!=1.0 && l!=0.0 )  v /= sqrt(l);
}

template<class T> inline TVec2<T> proj(const TVec3<T>& v)
{
    TVec2<T> u(v[0], v[1]);
    if( v[2]!=1.0 && v[2]!=0.0 )
	u /= v[2];
    return u;
}

typedef TVec3<double> Vector3;
typedef TVec3<float>  Vector3f;

typedef TVec3<double> Color3;
typedef TVec3<float>  Color3f;

typedef TVec3<double> Point3;
typedef TVec3<float>  Point3f;

} // namespace vecmath 
} // namespace cg

// __GFX_VEC3_INCLUDED
#endif
