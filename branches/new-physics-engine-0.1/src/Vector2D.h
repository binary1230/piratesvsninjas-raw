#ifndef VECTOR2D_H
#define VECTOR2D_H

#include <stdio.h>
#include <math.h>
#include <vector>

using namespace std;

class Vector2D;
class Force;

//! A fudge value.  You can't compare two floats
//! directly, we just want to know if they are close.
//! This defines how close they must be to be considered
//! 'equal'
#define TOLERANCE 0.00001

// #include "globals.h"
// #include "force.h"

class Vector2D {
	public:
		float x,y;

		void Clear();

		int GetX();
		int GetY();
		
		//! Assign a vector's values to this vector
		inline bool Vector2D::operator=(Vector2D v) const {
				x = v.x;
				y = v.y;
		}
	
		//! Add two vectors
		inline Vector2D Vector2D::operator+(Vector2D v) const {
			return Vector2D(x + v.x, y + v.y);
		}

		//! Subtract two vectors
		inline Vector2D Vector2D::operator-(Vector2D v) const {
			return Vector2D(x - v.x, y - v.y);
		}
		
		//! Compare two vectors
		inline bool Vector2D::operator==(Vector2D v) const {
			return (x - v.x < TOLERANCE && x - v.x > -TOLERANCE
							y - v.y < TOLERANCE && y - v.y > -TOLERANCE)
		}
		
		//! Compare two vectors
		inline bool Vector2D::operator!=(Vector2D v) const {
			return (!(this == v));
		}

		//! Scalar product (vector * number)
		inline Vector2D Vector2D::operator*(float n) const {
			return Vector2D(x*n, y*n);
		}
		
		Vector2D();
		Vector2D(float _x, float _y);
		virtual ~Vector2D();
};

#endif // VECTOR2D_H
