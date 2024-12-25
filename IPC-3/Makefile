CC = g++
TARGET = shared-arr

$(TARGET): shared-arr.o main.o
	$(CC) shared-arr.o main.o -o $(TARGET)

shared-arr.o: shared-arr.cpp
	$(CC) -c shared-arr.cpp

main.o: main.cpp
	$(CC) -c main.cpp

clean:
	rm -f $(TARGET) *.o
