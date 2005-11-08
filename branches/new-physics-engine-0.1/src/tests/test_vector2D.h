#ifndef Test_Vector2D_H 
#define Test_Vector2D_H 

#include <cppunit/extensions/HelperMacros.h>
#include <stdio.h>
#include "vector2D.h"

class Test_Vector2D : public CppUnit::TestFixture {
				CPPUNIT_TEST_SUITE( Test_Vector2D ); 
				CPPUNIT_TEST( testGetSet ); 
				CPPUNIT_TEST( testClear );
				CPPUNIT_TEST( testOperatorEquals ); 
				CPPUNIT_TEST( testOperatorEqualsFuzzy ); 
				CPPUNIT_TEST( testOperatorPlus ); 
				CPPUNIT_TEST( testOperatorMinus ); 
				CPPUNIT_TEST( testOperatorMultiply ); 
				CPPUNIT_TEST( testOperatorDivide ); 
				CPPUNIT_TEST( testDotProduct );
				CPPUNIT_TEST_SUITE_END(); 
				
				float *testing_set;
				int count;
				
			public: 
				void setUp(); 
				void tearDown(); 
				
				void testGetSet(); 
				void testClear();
				void testOperatorEquals(); 
				void testOperatorEqualsFuzzy();
				void testOperatorPlus();
				void testOperatorMinus();
				void testOperatorMultiply();
				void testOperatorDivide();
				void testDotProduct();
}; 

#endif // Test_Vector2D_H 
