CXX = g++
CXXFLAGS = -g -Wall

all: TimeInterval.o interval_tests.o
	$(CXX) $(CXXFLAGS) -o interval_tests TimeInterval.o interval_tests.o

TimeInterval.o: TimeInterval.cpp TimeInterval.h
	$(CXX) $(CXXFLAGS) -c TimeInterval.cpp

interval_tests.o: interval_tests.cpp TimeInterval.h
	$(CXX) $(CXXFLAGS) -c interval_tests.cpp

clean:
	\rm -f interval_tests.o TimeInterval.o interval_tests
