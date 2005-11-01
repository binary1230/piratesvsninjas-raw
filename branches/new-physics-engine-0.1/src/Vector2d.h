#ifndef VECTOR2D_H
#define VECTOR2D_H

#include <stdio.h>
#include <math.h>
#include <vector>

using namespace std;

class Vector2D;
class Force;

#include "globals.h"
#include "force.h"

class Vector2D {
	public:
		float x,y;

		void Clear();

		int GetX();
		int GetY();

		//! Add two vectors
		inline Vector2D Vector2D::operator+(Vector2D v) const {
			Vector2D Vector2D(x + v.x, y + v.y);
		}

		Vector2D();
		Vector2D(float _x, float _y);
		virtual ~Vector2D();
};

#endif // VECTOR2D_H
