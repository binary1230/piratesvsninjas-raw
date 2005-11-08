#include "test_vector2D.h"

// Registers the fixture into the 'registry'
CPPUNIT_TEST_SUITE_REGISTRATION( Test_Vector2D );

// test Clear()
void Test_Vector2D::testClear() {
	Vector2D v;
	v.SetX(3.0f);
	v.SetY(3.0f);
	v.Clear();
	CPPUNIT_ASSERT(v.GetX() == 0.0f && v.GetY() == 0.0f);
}

void Test_Vector2D::testOperatorPlus() {
	Vector2D v1, v2, v_result, v_expected;
	
	v1.SetX(1.0f);	v1.SetY(2.0f);
	v2.SetX(1.0f);	v2.SetY(2.0f);
	v_expected.SetX(2.0f);	v_expected.SetY(4.0f);

	v_result = v1 + v2;

	CPPUNIT_ASSERT(v_result == v_expected);
	CPPUNIT_ASSERT(v_result.GetX() == 2.0f && v_result.GetY() == 4.0f);
}

void Test_Vector2D::testOperatorMinus() {
	Vector2D v1, v2, v_result, v_expected;
	
	v1.SetX(3.0f);	v1.SetY(4.0f);
	v2.SetX(1.0f);	v2.SetY(-4.0f);
	v_expected.SetX(2.0f);	v_expected.SetY(8.0f);

	v_result = v1 - v2;

	CPPUNIT_ASSERT(v_result == v_expected);
	CPPUNIT_ASSERT(v_result.GetX() == 2.0f && v_result.GetY() == 8.0f);
}

// test strict equality for the same numbers
void Test_Vector2D::testOperatorEquals() {
	Vector2D v1, v2;
	int i;

	for (i = 0; i < count; i++) {
			v1.SetX(testing_set[i]);	
			v1.SetY(testing_set[i]);
			
			v2.SetX(testing_set[i]);	
			v2.SetY(testing_set[i]);

			CPPUNIT_ASSERT(v1 == v2);
			CPPUNIT_ASSERT(v2 == v1);
	}
}

// test fuzzy equality
void Test_Vector2D::testOperatorEqualsFuzzy() {
	Vector2D v1, v2;
	int i;

	for (i = 0; i < count; i++) {
		v1.SetX(testing_set[i]);	
		v1.SetY(testing_set[i]);
		
		v2.SetX(testing_set[i] + 0.00000001f); 
		v2.SetY(testing_set[i] + 0.00000001f);
		
		CPPUNIT_ASSERT(v1 == v2);
		CPPUNIT_ASSERT(v2 == v1);
	}
}

// test basic methods with different values
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

void Test_Vector2D::setUp() {
float set[] = {	0.0f, -1.0f, 1.0f, 5.0f, -5.0f, 0.00001f, 
				-0.00001f, 100000.0f, -100000.0f };

testing_set = set;
count = sizeof(set) / sizeof(float);
}

void Test_Vector2D::tearDown() {
}
