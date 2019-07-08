#pragma once

//#include "kuConstants.h"

class int2 {
public:
    int x, y;
    
    static const int DIM = 2;
	
    int2();
    int2( int scalar );
    int2( int _x, int _y );
	
    int * getPtr() {
        return (int*)&x;
	}
    const int * getPtr() const {
        return (const int*)&x;
	}
	
    int& operator[]( int n ){
		return getPtr()[n];
	}
	
    int operator[]( int n ) const {
		return getPtr()[n];
	}
	
	
    void set( int _scalar );
    void set( int _x, int _y );
    void set( const int2& vec );
	
    bool operator==( const int2& vec ) const;
    bool operator!=( const int2& vec ) const;
    bool match( const int2& vec ) const;

        // Overloading for any type to any type
    //
    int2  operator+( const int2& vec ) const;
    int2& operator+=( const int2& vec );
    int2  operator-( const int2& vec ) const;
    int2& operator-=( const int2& vec );
    int2  operator*( const int2& vec ) const;
    int2& operator*=( const int2& vec );
    int2  operator/( const int2& vec ) const;
    int2& operator/=( const int2& vec );
	
	
    //operator overloading for int
    //
    //    void 	  operator=( const int f);
    int2  operator+( const int f ) const;
    int2& operator+=( const int f );
    int2  operator-( const int f ) const;
    int2& operator-=( const int f );
    int2  operator-() const;
    int2  operator*( const int f ) const;
    int2& operator*=( const int f );
    int2  operator/( const int f ) const;
    int2& operator/=( const int f );
	
    friend ostream& operator<<(ostream& os, const int2& vec);
    friend istream& operator>>(istream& is, const int2& vec);
	
    // Distance between two points.
    //
    float distance( const int2& pnt) const;
    int squareDistance( const int2& pnt ) const;
	
	
    float length() const;
    int lengthSquared() const;
//	OF_DEPRECATED_MSG("Use int2::lengthSquared() instead.", int squareLength() const);
	
	
    float angle( const int2& vec ) const;
    float angleRad( const int2& vec ) const;
	
    int dot( const int2& vec ) const;
	
	
	

};


// Non-Member operators
//
int2 operator+( int f, const int2& vec );
int2 operator-( int f, const int2& vec );
int2 operator*( int f, const int2& vec );
int2 operator/( int f, const int2& vec );

/////////////////
// Implementation
/////////////////

inline int2::int2(): x(0), y(0) {};
inline int2::int2( int _scalar ): x(_scalar), y(_scalar) {};
inline int2::int2( int _x, int _y ):x(_x), y(_y) {}

// Getters and Setters.
//
//
inline void int2::set( int _scalar ) {
	x = _scalar;
	y = _scalar;
}

inline void int2::set( int _x, int _y ) {
	x = _x;
	y = _y;
}

inline void int2::set( const int2& vec ) {
	x = vec.x;
	y = vec.y;
}


// Check similarity/equality.
//
//
inline bool int2::operator==( const int2& vec ) const {
	return (x == vec.x) && (y == vec.y);
}

inline bool int2::operator!=( const int2& vec ) const {
	return (x != vec.x) || (y != vec.y);
}

inline bool int2::match( const int2& vec ) const {
    return ( x == vec.x	&& y == vec.y );
}

// Overloading for any type to any type
//
//

inline int2 int2::operator+( const int2& vec ) const {
    return int2( x+vec.x, y+vec.y);
}

inline int2& int2::operator+=( const int2& vec ) {
	x += vec.x;
	y += vec.y;
	return *this;
}

inline int2 int2::operator-( const int2& vec ) const {
    return int2(x-vec.x, y-vec.y);
}

inline int2& int2::operator-=( const int2& vec ) {
	x -= vec.x;
	y -= vec.y;
	return *this;
}

inline int2 int2::operator*( const int2& vec ) const {
    return int2(x*vec.x, y*vec.y);
}

inline int2& int2::operator*=( const int2& vec ) {
	x*=vec.x;
	y*=vec.y;
	return *this;
}

inline int2 int2::operator/( const int2& vec ) const {
    return int2( vec.x!=0 ? x/vec.x : x , vec.y!=0 ? y/vec.y : y);
}

inline int2& int2::operator/=( const int2& vec ) {
	vec.x!=0 ? x/=vec.x : x;
	vec.y!=0 ? y/=vec.y : y;
	return *this;
}

inline ostream& operator<<(ostream& os, const int2& vec) {
	os << vec.x << ", " << vec.y;
	return os;
}

inline istream& operator>>(istream& is, int2& vec) {
	is >> vec.x;
	is.ignore(2);
	is >> vec.y;
	return is;
}

//operator overloading for int
//
//
//inline void int2::operator=( const int f){
//	x = f;
//	y = f;
//}

inline int2 int2::operator+( const int f ) const {
    return int2( x+f, y+f);
}

inline int2& int2::operator+=( const int f ) {
	x += f;
	y += f;
	return *this;
}

inline int2 int2::operator-( const int f ) const {
    return int2( x-f, y-f);
}

inline int2& int2::operator-=( const int f ) {
	x -= f;
	y -= f;
	return *this;
}

inline int2 int2::operator-() const {
    return int2(-x, -y);
}

inline int2 int2::operator*( const int f ) const {
    return int2(x*f, y*f);
}

inline int2& int2::operator*=( const int f ) {
	x*=f;
	y*=f;
	return *this;
}

inline int2 int2::operator/( const int f ) const {
    if(f == 0) return int2(x, y);
	
    return int2(x/f, y/f);
}

inline int2& int2::operator/=( const int f ) {
	if(f == 0) return *this;
	
	x/=f;
	y/=f;
	return *this;
}

// Distance between two points.
//
//
inline float int2::distance( const int2& pnt) const {
    int vx = x-pnt.x;
    int vy = y-pnt.y;
    return (float)sqrt(float(vx*vx + vy*vy));
}


inline int int2::squareDistance( const int2& pnt ) const {
    int vx = x-pnt.x;
    int vy = y-pnt.y;
	return vx*vx + vy*vy;
}



// Length
//
//
inline float int2::length() const {
    return (float)sqrt( float(x*x + y*y) );
}

inline int int2::lengthSquared() const {
    return (int)(x*x + y*y);
}

//inline int int2::squareLength() const {
//	return lengthSquared();
//}



/**
 * Angle (deg) between two vectors.
 * This is a signed relative angle between -180 and 180.
 */
inline float int2::angle( const int2& vec ) const {
    return (float)(atan2( (float)(x*vec.y-y*vec.x), (float)(x*vec.x + y*vec.y) )*RAD_TO_DEG);
}

/**
 * Angle (deg) between two vectors.
 * This is a signed relative angle between -180 and 180.
 */
inline float int2::angleRad( const int2& vec ) const {
	return atan2( (float)(x*vec.y-y*vec.x), (float)(x*vec.x + y*vec.y) );
}


/**
 * Dot Product.
 */
inline int int2::dot( const int2& vec ) const {
	return x*vec.x + y*vec.y;
}


// Non-Member operators
//
//
inline int2 operator+( int f, const int2& vec ) {
    return int2( f+vec.x, f+vec.y);
}

inline int2 operator-( int f, const int2& vec ) {
    return int2( f-vec.x, f-vec.y);
}

inline int2 operator*( int f, const int2& vec ) {
    return int2( f*vec.x, f*vec.y);
}

inline int2 operator/( int f, const int2& vec ) {
    return int2( f/vec.x, f/vec.y);
}


