# Rules for the test code (use `make check` to execute)
TESTS = Test1
check_PROGRAMS = $(TESTS)

headers = \
test1.h

Test1_SOURCES = test1.cpp main.cpp ../src/vector2D.o $(headers)
Test1_CXXFLAGS = $(CPPUNIT_CFLAGS)
Test1_LDFLAGS = $(CPPUNIT_LIBS)
