CXX = g++

TARGET = read-file

all: $(TARGET)

$(TARGET): read-file.o
	$(CXX) -o $(TARGET) read-file.o

read-file.o: read-file.cpp
	$(CXX) -c read-file.cpp


clean:
	rm -f *.o $(TARGET)
