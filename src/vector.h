#pragma once

#include <sstream>
#include <string>
#include "utils.h"

template < typename T >
struct Vector3
{
	T v [ 3 ];

	Vector3 ()
	{
		for ( unsigned int i = 0; i < NUMELEMS(v); ++i )
			v[i] = 0;
	}

	Vector3 ( const Vector3& other )
	{
		for ( unsigned int i = 0; i < NUMELEMS(v); ++i )
			v[i] = other.v[i];
	}

	Vector3 ( const T& x, const T& y, const T& z )
	{
		v[0] = x;
		v[1] = y;
		v[2] = z;
	}

	const float& x () const { return v[0]; }
	const float& y () const { return v[1]; }
	const float& z () const { return v[2]; }

	float& x () { return v[0]; }
	float& y () { return v[1]; }
	float& z () { return v[2]; }

	operator std::string ()
	{
		std::ostringstream result;
		result << "(" << x() << ", " << y() << ", " << z() << ")";
		return result.str();
	}

	Vector3 operator* ( const T& k ) const
	{
		Vector3 result ( *this );
		for ( unsigned int i = 0; i < NUMELEMS(v); ++i )
			result.v[i] *= k;
		return result;
	}
	Vector3& operator*= ( const T& k )
	{
		for ( unsigned int i = 0; i < NUMELEMS(v); ++i )
			v[i] *= k;
		return *this;
	}

	Vector3 operator/ ( const T& k ) const
	{
		Vector3 result ( *this );
		for ( unsigned int i = 0; i < NUMELEMS(v); ++i )
			result.v[i] /= k;
		return result;
	}
	Vector3& operator/= ( const T& k )
	{
		for ( unsigned int i = 0; i < NUMELEMS(v); ++i )
			v[i] /= k;
		return *this;
	}

	Vector3 operator+ ( const T& k ) const
	{
		Vector3 result ( *this );
		for ( unsigned int i = 0; i < NUMELEMS(v); ++i )
			result.v[i] += k;
		return result;
	}
	Vector3& operator+= ( const T& k )
	{
		for ( unsigned int i = 0; i < NUMELEMS(v); ++i )
			v[i] += k;
		return *this;
	}

	Vector3 operator- ( const T& k ) const
	{
		Vector3 result ( *this );
		for ( unsigned int i = 0; i < NUMELEMS(v); ++i )
			result.v[i] -= k;
		return result;
	}
	Vector3& operator-= ( const T& k )
	{
		for ( unsigned int i = 0; i < NUMELEMS(v); ++i )
			v[i] -= k;
		return *this;
	}
};

template < typename T >
T dot ( const Vector3<T>& u, const Vector3<T>& v )
{
	T sum = 0;
	for ( unsigned int i = 0; i < NUMELEMS(u.v); ++i )
		sum += u.v[i] * v.v[i];
	return sum;
}

template < typename T >
Vector3<T> cross ( const Vector3<T>& u, const Vector3<T>& v )
{
	Vector3<T> result ( u.y() * v.z() - u.z() * v.y(),
		                u.z() * v.x() - u.x() * v.z(),
						u.x() * v.y() - u.y() * v.x() );
	return result;
}

template < typename T >
T length ( const Vector3<T>& v )
{
	return sqrt(dot(v, v));
}

template < typename T >
Vector3<T> normalize ( const Vector3<T>& v )
{
	T length = length(v);
	if ( length != 0 )
		return Vector3(v / length);
	throw "Attempt to normalize null vector";
}

typedef Vector3<float> vec3f;
typedef Vector3<double> vec3d;
