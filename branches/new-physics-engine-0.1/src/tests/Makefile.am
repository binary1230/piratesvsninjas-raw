# Rules for the test code (use `make check` to execute)
TESTS = Test1
check_PROGRAMS = $(TESTS)

Test1_SOURCES = test1.cpp main.cpp ../vector2D.h
Test1_CXXFLAGS = $(CPPUNIT_CFLAGS) -I ../ -Wall
Test1_LDFLAGS = $(CPPUNIT_LIBS)
Test1_LDADD = ../vector2D.o
