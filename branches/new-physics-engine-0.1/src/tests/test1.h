#ifndef Test_Vector2D_H 
#define Test_Vector2D_H 

#include <cppunit/extensions/HelperMacros.h>
#include <stdio.h>
#include "vector2D.h"

class Test_Vector2D : public CppUnit::TestFixture {
				CPPUNIT_TEST_SUITE( Test_Vector2D ); 
				CPPUNIT_TEST( testGetSet ); 
				CPPUNIT_TEST( testOperatorEquals ); 
				CPPUNIT_TEST_SUITE_END(); 
				
				float *testing_set;
				int count;
				
			public: 
				void setUp(); 
				void tearDown(); 
				
				void testAccessors(); 
				void testOperatorEquals(); 
				void testGetSet(); 
				
}; 

#endif // Test_Vector2D_H 
