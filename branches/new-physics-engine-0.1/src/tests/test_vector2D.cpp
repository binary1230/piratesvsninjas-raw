#include "test1.h"

// Registers the fixture into the 'registry'
CPPUNIT_TEST_SUITE_REGISTRATION( Test_Vector2D );

void Test_Vector2D::setUp() {
	float set[] = {	0.0f, -1.0f, 1.0f, 5.0f, -5.0f, 0.00001f, 
					-0.00001f, 100000.0f, -100000.0f };

	testing_set = set;
	count = sizeof(set) / sizeof(float);
}

void Test_Vector2D::testOperatorEquals() {
	Vector2D v1, v2;
	int i;

	// test strict equality for the same numbers
	for (i = 0; i < count; i++) {
			v1.SetX(testing_set[i]);	
			v1.SetY(testing_set[i]);
			
			v2.SetX(testing_set[i]);	
			v2.SetY(testing_set[i]);

			CPPUNIT_ASSERT(v1 == v2);
			CPPUNIT_ASSERT(v2 == v1);
	}

	// test fuzzy equality
	for (i = 0; i < count; i++) {
		v1.SetX(testing_set[i]);	
		v1.SetY(testing_set[i]);
		
		v2.SetX(testing_set[i] + 0.00000001f); 
		v2.SetY(testing_set[i] + 0.00000001f);
		
		CPPUNIT_ASSERT(v1 == v2);
		CPPUNIT_ASSERT(v2 == v1);
	}
}

void Test_Vector2D::testGetSet() {
	Vector2D v;
	int i;
	float x, y;
	
	for (i = 0; i < count; i++) {
		x = testing_set[i];
		v.SetX(x);
		CPPUNIT_ASSERT_EQUAL(x, v.GetX());
		
		y = testing_set[i];
		v.SetY(y);
		CPPUNIT_ASSERT_EQUAL(y, v.GetY());
	}
}

void Test_Vector2D::tearDown() {
}
