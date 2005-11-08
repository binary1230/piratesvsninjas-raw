# Rules for the test code (use `make check` to execute)

# all test programs to run
TESTS = vector2D
check_PROGRAMS = $(TESTS)

# vector2D unit tests
vector2D_SOURCES = 	runner.cpp ../vector2D.h \
										test_vector2D.cpp test_vector2D.h
vector2D_CXXFLAGS = $(CPPUNIT_CFLAGS) -I ../ -Wall
vector2D_LDFLAGS = $(CPPUNIT_LIBS)
vector2D_LDADD = ../vector2D.o
