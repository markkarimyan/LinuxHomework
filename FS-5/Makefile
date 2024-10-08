TARGET = manual-append

all: $(TARGET)

$(TARGET): manual-append.o
	g++ -o $(TARGET) manual-append.o

manual-append.o: manual-append.cpp
	g++ -c manual-append.cpp

clean:
	rm -f *.o $(TARGET)
