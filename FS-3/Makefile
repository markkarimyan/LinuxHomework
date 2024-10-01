CXX = g++


TARGET = hacker-rm


all: $(TARGET)

$(TARGET): hacker-rm.o
	$(CXX) -o $(TARGET) hacker-rm.o

hacker-rm.o: hacker-rm.cpp
	$(CXX) -c hacker-rm.cpp


clean:
	rm -f *.o $(TARGET)
