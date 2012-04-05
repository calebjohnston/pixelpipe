#ifndef __CG_MAT4_H
#define __CG_MAT4_H

#include "cg/vecmath/mat3.hpp"
#include "cg/vecmath/vec4.hpp"

#define SWAP(a, b, t)   {t = a; a = b; b = t;}

namespace cg {
namespace vecmath {

template<class T> 
class Mat4
{
private:
    TVec4<T> row[4];

public:
    // Standard constructors
    //
    Mat4() { *this = 0.0; }
    template<class U> Mat4(const TVec4<U>& r0,const TVec4<U>& r1,const TVec4<U>& r2,const TVec4<U>& r3)
    {
		row[0]=r0;
		row[1]=r1;
		row[2]=r2;
		row[3]=r3;
	}
    template<class U> Mat4(const Mat3<U>& basis, const TVec3<U>& trans, T scale)
    {
		row[0][0] = basis[0][0];	row[0][1] = basis[0][1];	row[0][2] = basis[0][2];	row[0][3] = trans.x;
		row[1][0] = basis[1][0];	row[1][1] = basis[1][1];	row[1][2] = basis[1][2];	row[1][3] = trans.y;
		row[2][0] = basis[2][0];	row[2][1] = basis[2][1];	row[2][2] = basis[2][2];	row[2][3] = trans.z;
		row[0][0] *= scale;		row[1][1] *= scale;		row[2][2] *= scale;
		row[3][3] = 1.0;
	}
    template<class U> Mat4(const Mat4<U>& m) { *this = m; }
	// template<class U> Mat4(const T* arr)	// doesn't work in clang++? why?
	Mat4(const T* arr)
	{
		row[0] = TVec4<T>(arr[0], arr[4], arr[8], arr[12]);
		row[1] = TVec4<T>(arr[1], arr[5], arr[9], arr[13]);
		row[2] = TVec4<T>(arr[2], arr[6], arr[10], arr[14]);
		row[3] = TVec4<T>(arr[3], arr[7], arr[11], arr[15]);
	}

    // Descriptive interface
    //
    typedef T value_type;
    typedef TVec4<T> vector_type;
    typedef Mat4<T> inverse_type;
    static int dim() { return 4; }

    // Access methods
    //
    T& operator()(int i, int j)       { return row[i][j]; }
    T  operator()(int i, int j) const { return row[i][j]; }
    TVec4<T>&       operator[](int i)       { return row[i]; }
    const TVec4<T>& operator[](int i) const { return row[i]; }
    inline TVec4<T> getColumn(int i) const { return TVec4<T>(row[0][i], row[1][i], row[2][i], row[3][i]); }
	inline TVec4<T> getRow(int i) const { return TVec4<T>(row[i]); }

    operator       T*()       { return row[0]; }
    operator const T*()       { return row[0]; }
    operator const T*() const { return row[0]; }

    // Assignment methods
    //
    inline Mat4<T>& operator=(const Mat4<T>& m);
    inline Mat4<T>& operator=(T s);

    inline Mat4<T>& operator+=(const Mat4<T>& m);
    inline Mat4<T>& operator-=(const Mat4<T>& m);
    inline Mat4<T>& operator*=(T s);
    inline Mat4<T>& operator/=(T s);

    static Mat4<T> outer_product(const TVec4<T>& u, const TVec4<T>& v)
	{
	    Mat4<T> A;

	    for(int i=0; i<dim(); i++){
			for(int j=0; j<dim(); j++){
		    	A(i, j) = u[i]*v[j];
			}
		}

	    return A;
	}
	
    static Mat4<T> outer_product(const TVec4<T>& v)
	{
	    Mat4<T> A;

	    A(0,0) = v.x*v.x;	A(0,1) = v.x*v.y;	A(0,2) = v.x*v.z;	A(0,3) = v.x*v.w;
	    A(1,0) = A(0,1);	A(1,1) = v.y*v.y;	A(1,2) = v.y*v.z;	A(1,3) = v.y*v.w;
	    A(2,0) = A(0,2);	A(2,1) = A(1,2);	A(2,2) = v.z*v.z;	A(2,3) = v.z*v.w;
		A(3,0) = A(0,3);	A(3,1) = A(1,3);	A(3,2) = A(2,3);	A(3,3) = v.w*v.w;

	    return A;
	}
	
	inline void setColumn(int i, const TVec4<T>& v)
	{
		row[0][i] = v.x; row[1][i] = v.y; row[2][i] = v.z; row[3][i] = v.w;
	}
	
	inline void setRow(int i, const TVec4<T>& v)
	{	
		row[i] = v;
	}
	
    Mat4<T>& diagonal(T d)
	{
	    *this = 0.0;
	    row[0][0] = row[1][1] = row[2][2] = row[3][3] = d;
	    return *this;
	}
	
    Mat4<T>& identity()
	{
		return diagonal(1.0);
	}

    static Mat4<T> I()
	{
		return Mat4<T>(TVec4<T>(1,0,0,0), TVec4<T>(0,1,0,0), TVec4<T>(0,0,1,0), TVec4<T>(0,0,0,1));
	}
	
	inline T getDeterminant()
	{
		return det(*this);
	}

	inline T getTrace()
	{
		return trace(*this);
	}

	inline Mat4<T> getTranspose()
	{
		return transpose(*this);
	}

	Mat4<T> getAdjoint()
	{
		return adjoint(*this);
	}

	inline void toArray(T* arr) const
	{
		// T arr[16] = {	row[0][0],	row[1][0],	row[2][0],	row[3][0],	
		// 				row[0][1],	row[1][1],	row[2][1],	row[3][1],	
		// 				row[0][2],	row[1][2],	row[2][2],	row[3][2],	
		// 				row[0][3],	row[1][3],	row[2][3],	row[3][3]
		// 			};
		// return arr;
		arr[0] = row[0][0];
		arr[1] = row[1][0];
		arr[2] = row[2][0];
		arr[3] = row[3][0];
		arr[4] = row[0][1];
		arr[5] = row[1][1];
		arr[6] = row[2][1];
		arr[7] = row[3][1];	
		arr[8] = row[0][2];
		arr[9] = row[1][2];
		arr[10] = row[2][2];
		arr[11] = row[3][2];	
		arr[12] = row[0][3];
		arr[13] = row[1][3];
		arr[14] = row[2][3];
		arr[15] = row[3][3];
	}
};

////////////////////////////////////////////////////////////////////////
//
// Method definitions
//

template<class T> inline Mat4<T>& Mat4<T>::operator=(const Mat4<T>& m)
	{
	    row[0] = m[0]; row[1] = m[1]; row[2] = m[2]; row[3] = m[3];
	    return *this;
	}

template<class T> inline Mat4<T>& Mat4<T>::operator=(T s)
	{
	    row[0]=s;  row[1]=s;  row[2]=s;  row[3]=s;
	    return *this;
	}

template<class T> inline Mat4<T>& Mat4<T>::operator+=(const Mat4<T>& m)
	{
	    row[0] += m[0]; row[1] += m[1]; row[2] += m[2]; row[3] += m[3];
	    return *this;
	}

template<class T> inline Mat4<T>& Mat4<T>::operator-=(const Mat4<T>& m)
	{
	    row[0] -= m[0]; row[1] -= m[1]; row[2] -= m[2]; row[3] -= m[3];
	    return *this;
	}

template<class T> inline Mat4<T>& Mat4<T>::operator*=(T s)
	{
	    row[0] *= s; row[1] *= s; row[2] *= s; row[3] *= s;
	    return *this;
	}

template<class T> inline Mat4<T>& Mat4<T>::operator/=(T s)
	{
	    row[0] /= s; row[1] /= s; row[2] /= s; row[3] /= s;
	    return *this;
	}

////////////////////////////////////////////////////////////////////////
//
// Operator definitions
//

template<class T> inline Mat4<T> operator+(const Mat4<T>& n, const Mat4<T>& m)
	{ return Mat4<T>(n[0]+m[0], n[1]+m[1], n[2]+m[2], n[3]+m[3]); }

template<class T> inline Mat4<T> operator-(const Mat4<T>& n, const Mat4<T>& m)
	{ return Mat4<T>(n[0]-m[0], n[1]-m[1], n[2]-m[2], n[3]-m[3]); }

template<class T> inline Mat4<T> operator-(const Mat4<T>& n)
	{ return Mat4<T>(-n[0], -n[1], -n[2], -n[3]); }

template<class T> inline Mat4<T> operator*(T s, const Mat4<T>& m)
	{ return Mat4<T>(m[0]*s, m[1]*s, m[2]*s, m[3]*s); }
	
template<class T> inline Mat4<T> operator*(const Mat4<T>& m, T s)
	{ return s*m; }

template<class T> inline Mat4<T> operator/(const Mat4<T>& m, T s)
	{ return Mat4<T>(m[0]/s, m[1]/s, m[2]/s, m[3]/s); }

template<class T> inline TVec4<T> operator*(const Mat4<T>& m, const TVec4<T>& v)
	{ return TVec4<T>(m[0]*v, m[1]*v, m[2]*v, m[3]*v); }

template<class T> Mat4<T> operator*(const Mat4<T>& n, const Mat4<T>& m)
	{
	    Mat4<T> A;
	    int i,j;

	    for(i=0;i<4;i++)
		for(j=0;j<4;j++)
		    A(i,j) = n[i]*m.getColumn(j);

	    return A;
	}

//
// Transform a homogeneous 3-vector and reproject into normal 3-space
//
template<class T> inline TVec3<T> operator*(const Mat4<T>& m, const TVec3<T>& v)
	{
	    TVec4<T> u=TVec4<T>(v,1);
	    T w=m[3]*u;

	    if(w==0.0)  return Vec3(m[0]*u, m[1]*u, m[2]*u);
	    else        return Vec3(m[0]*u/w, m[1]*u/w, m[2]*u/w);
	}

template<class T> inline std::ostream &operator<<(std::ostream &out, const Mat4<T>& M)
	{
		return out << std::endl << M.getRow(0) << std::endl << M.getRow(1) << std::endl << M.getRow(2) << std::endl << M.getRow(3);
	}

template<class T> inline std::istream &operator>>(std::istream &in, Mat4<T>& M)
	{
		return in >> M[0] >> M[1] >> M[2] >> M[3];
	}

////////////////////////////////////////////////////////////////////////
//
// Transformations
//

template<class T> Mat4<T> translationMatrix(const TVec3<T>& delta)
	{
	    return Mat4<T>(TVec4<T>(1, 0, 0, delta[0]),
			TVec4<T>(0, 1, 0, delta[1]),
			TVec4<T>(0, 0, 1, delta[2]),
			TVec4<T>(0, 0, 0, 1));
	}

template<class T> Mat4<T> scalingMatrix(const TVec3<T>& scale)
	{
	    return Mat4<T>(	TVec4<T>(scale[0],	0,    		0,			0),
						TVec4<T>(0,    		scale[1],	0,			0),
						TVec4<T>(0,    		0,			scale[2],	0),
						TVec4<T>(0,    		0,			0,			1));
	}

template<class T> Mat4<T> rotationMatrix(T theta, const TVec3<T>& axis, bool degrees=false)
	{
		if(degrees) theta = theta * M_PI / 180.0;
		
	    T c=cos(theta), s=sin(theta),
		xx=axis[0]*axis[0],  yy=axis[1]*axis[1],  zz=axis[2]*axis[2],
		xy=axis[0]*axis[1],  yz=axis[1]*axis[2],  xz=axis[0]*axis[2];

	    T xs=axis[0]*s, ys=axis[1]*s, zs=axis[2]*s;

	    Mat4<T> M;
	    M(0,0)=xx*(1-c)+c;  M(0,1)=xy*(1-c)-zs;  M(0,2)=xz*(1-c)+ys;  M(0,3) = 0;
	    M(1,0)=xy*(1-c)+zs;  M(1,1)=yy*(1-c)+c;  M(1,2)=yz*(1-c)-xs;  M(1,3)=0;
	    M(2,0)=xz*(1-c)-ys;  M(2,1)=yz*(1-c)+xs;  M(2,2)=zz*(1-c)+c;  M(2,3)=0;
	    M(3,0)=0;  M(3,1)=0;  M(3,2)=0;  M(3,3)=1;

	    return M;
	}

template<class T> Mat4<T> perspectiveMatrix(T fovy, T aspect, T zmin=0.0, T zmax=0.0)
	{
	    T A, B;
	    Mat4<T> M;

	    if( zmax==0.0 ){
			A = B = 1.0;
	    }
	    else{
			A = (zmax+zmin)/(zmin-zmax);
			B = (2*zmax*zmin)/(zmin-zmax);
	    }

	    T f = 1.0/tan(fovy*M_PI/180.0/2.0);
	    M(0,0) = f/aspect;
	    M(1,1) = f;
	    M(2,2) = A;
	    M(2,3) = B;
	    M(3,2) = -1;
	    M(3,3) = 0;

	    return M;
	}

template<class T> Mat4<T> lookAtMatrix(const TVec3<T>& from, const TVec3<T>& at, const TVec3<T>& v_up)
	{
	    TVec3<T> up = v_up;
		unitize(up);
	    TVec3<T> f = at - from;
		unitize(f);

	    TVec3<T> s=f^up;
	    TVec3<T> u=s^f;

	    // NOTE: These steps are left out of the GL man page!!
	    unitize(s);
	    unitize(u);

	    Mat4<T> M(TVec4<T>(s, 0), TVec4<T>(u, 0), TVec4<T>(-f, 0), TVec4<T>(0, 0, 0, 1));

	    return M * translationMatrix(-from);
	}

template<class T> Mat4<T> viewportMatrix(T w, T h)
	{
	    return scalingMatrix(TVec3<T>(w/2.0, -h/2.0, 1)) * translationMatrix(TVec3<T>(1, -1, 0));
	}
	
	
/**
 * Sets this matrix to be a camera to frame matrix.
 * 
 * @param u The u vector of the frame.
 * @param v The v vector of the frame.
 * @param w The w vector of the frame.
 * @param p The origin of the frame.
 */
template<class T> Mat4<T> cameraToFrame(const TVec3<T>& u, const TVec3<T>& v, const TVec3<T>& w, const TVec3<T>& p)
	{
		Mat4<T> M;
		M.identity();
		
		M(0,0) = u.x;
		M(0,1) = u.y;
		M(0,2) = u.z;
		M(1,0) = v.x;
		M(1,1) = v.y;
		M(1,2) = v.z;
		M(2,0) = w.x;
		M(2,1) = w.y;
		M(2,2) = w.z;
		TVec4<T> temp(p.x, p.y, p.z, 1.0);
		temp = M * temp;
		M(0,3) = -temp.x;
		M(1,3) = -temp.y;
		M(2,3) = -temp.z;
		
		return M;
	}

/**
 * Sets this matrix to be a frame to camera matrix.
 * 
 * @param u The u vector of the frame.
 * @param v The v vector of the frame.
 * @param w The w vector of the frame.
 * @param p The origin of the frame.
 */
template<class T> Mat4<T> frameToCamera(const TVec3<T>& u, const TVec3<T>& v, const TVec3<T>& w, const TVec3<T>& p)
	{
		Mat4<T> M;
		M.identity();
		
		M(0,0) = u.x;
		M(0,1) = v.x;
		M(0,2) = w.x;
		M(0,3) = p.x;
		M(1,0) = u.y;
		M(1,1) = v.y;
		M(1,2) = w.y;
		M(1,3) = p.y;
		M(2,0) = u.z;
		M(2,1) = v.z;
		M(2,2) = w.z;
		M(2,3) = p.z;
		
		return M;
	}

////////////////////////////////////////////////////////////////////////
//
// Misc. function definitions
//

template<class T> inline T det(const Mat4<T>& m)
	{
		return m[0] * cross(m[1], m[2], m[3]);
	}
	
template<class T> inline T trace(const Mat4<T>& m)
	{
		return m(0,0)+m(1,1)+m(2,2)+m(3,3);
	}

template<class T> inline Mat4<T> transpose(const Mat4<T>& m)
	{
		return Mat4<T>(m.getColumn(0), m.getColumn(1), m.getColumn(2), m.getColumn(3));
	}

template<class T> Mat4<T> adjoint(const Mat4<T>& m)
	{
	    Mat4<T> A;

	    A[0] = cross( m[1], m[2], m[3]);
	    A[1] = cross(-m[0], m[2], m[3]);
	    A[2] = cross( m[0], m[1], m[3]);
	    A[3] = cross(-m[0], m[1], m[2]);

	    return A;
	}


// Matrix inversion code for 4x4 matrices using Gaussian elimination
// with partial pivoting.  This is a specialized version of a
// procedure originally due to Paul Heckbert <ph@cs.cmu.edu>.
//
// Returns determinant of A, and B=inverse(A)
// If matrix A is singular, returns 0 and leaves trash in B.
//
	
template<class T> T invert(Mat4<T>& m_inv, const Mat4<T>& m)
	{
	    Mat4<T> A = m;
	    int i, j, k;
	    T max, t, det, pivot;

	    /*---------- forward elimination ----------*/

	    for (i=0; i<4; i++) {               /* put identity matrix in B */
	        for (j=0; j<4; j++) {
	            m_inv(i, j) = (T)(i==j);
			}	
		}

	    det = 1.0;
	    for (i=0; i<4; i++) {               /* eliminate in column i, below diagonal */
	        max = -1.;
	        for (k=i; k<4; k++) {           /* find pivot for column i */
	            if (fabs(A(k, i)) > max) {
	                max = fabs(A(k, i));
	                j = k;
	            }
			}
			
	        if (max<=0.) return 0.0;        /* if no nonzero pivot, PUNT */
	
	        if (j!=i) {                     /* swap rows i and j */
	            for (k=i; k<4; k++)
	                SWAP(A(i, k), A(j, k), t);
	            for (k=0; k<4; k++)
	                SWAP(m_inv(i, k), m_inv(j, k), t);
	            det = -det;
	        }
	
	        pivot = A(i, i);
	        det *= pivot;
	
	        for (k=i+1; k<4; k++) {         /* only do elems to right of pivot */
	            A(i, k) /= pivot;
			}
			
	        for (k=0; k<4; k++) {
	            m_inv(i, k) /= pivot;
			}
	        /* we know that A(i, i) will be set to 1, so don't bother to do it */

	        for (j=i+1; j<4; j++) {         /* eliminate in rows below i */
	            t = A(j, i);                /* we're gonna zero this guy */
	            for (k=i+1; k<4; k++) {     /* subtract scaled row i from row j */
	                A(j, k) -= A(i, k)*t;   /* (ignore k<=i, we know they're 0) */
				}
	            for (k=0; k<4; k++) {
	                m_inv(j, k) -= m_inv(i, k)*t;
				}
	        }
	    }

	    /*---------- backward elimination ----------*/

	    for (i=4-1; i>0; i--) {             /* eliminate in column i, above diagonal */
	        for (j=0; j<i; j++) {           /* eliminate in rows above i */
	            t = A(j, i);                /* we're gonna zero this guy */
	            for (k=0; k<4; k++) {       /* subtract scaled row i from row j */
	                m_inv(j, k) -= m_inv(i, k)*t;
				}
	        }
	    }

	    return det;
	}
	
template<class T> T invert_cramer(Mat4<T>& m_inv, const Mat4<T>& m)
	{
	    Mat4<T> A = adjoint(m);
	    T d = A[0] * m[0];

	    if( d==0.0 )
		return 0.0;

	    m_inv = transpose(A) / d;
	    return d;
	}

// template<class T> extern bool eigen(const Mat4<T>& m, TVec4<T>& eig_vals, TVec4<T> eig_vecs[4]);


typedef Mat4<double> Matrix4;
typedef Mat4<float>  Matrix4f;

} // namespace vecmath
} // namespace cg

#undef SWAP

#endif	// __CG_MAT4_H
