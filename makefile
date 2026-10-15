CXX = g++
CXXFLAGS = -g -Wall -pthread -O2

TARGET = mt-collatz

all: $(TARGET)

$(TARGET): mt-collatz.o
	$(CXX) $(CXXFLAGS) mt-collatz.o -o $(TARGET)

mt-collatz.o: mt-collatz.cpp
	$(CXX) $(CXXFLAGS) -c mt-collatz.cpp -o mt-collatz.o

clean:
	rm -f $(TARGET) mt-collatz.o 
