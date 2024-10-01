CXX = g++


TARGET = simple-copy


all: $(TARGET)

$(TARGET): simple-copy.o
	$(CXX) -o $(TARGET) simple-copy.o

simple-copy.o: simple-copy.cpp
	$(CXX) -c simple-copy.cpp


clean:
	rm -f *.o $(TARGET)
