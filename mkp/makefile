CXX = g++
CXXFLAGS = -Wall -std=c++11
LIBS = -I/usr/include/eigen3

all: main test_mkp test_utils

main: main.o mkp.o utils.o
	$(CXX) $(CXXFLAGS) -o main main.o mkp.o utils.o $(LIBS)

test_mkp: test_mkp.o mkp.o utils.o
	$(CXX) $(CXXFLAGS) -o test_mkp test_mkp.o mkp.o utils.o $(LIBS)

test_utils: test_utils.o utils.o mkp.o
	$(CXX) $(CXXFLAGS) -o test_utils test_utils.o utils.o mkp.o $(LIBS)

main.o: main.cpp mkp.hpp utils.hpp
	$(CXX) $(CXXFLAGS) -c main.cpp $(LIBS)

mkp.o: mkp.cpp mkp.hpp
	$(CXX) $(CXXFLAGS) -c mkp.cpp $(LIBS)

utils.o: utils.cpp utils.hpp
	$(CXX) $(CXXFLAGS) -c utils.cpp $(LIBS)

test_mkp.o: test_mkp.cpp mkp.hpp utils.hpp
	$(CXX) $(CXXFLAGS) -c test_mkp.cpp $(LIBS)

test_utils.o: test_utils.cpp utils.hpp mkp.hpp
	$(CXX) $(CXXFLAGS) -c test_utils.cpp $(LIBS)

clean:
	rm -f *.o main test_mkp test_utils
