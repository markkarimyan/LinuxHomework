all: copy

copy: copy.o
	g++ -o copy copy.o

copy.o: copy.cpp
	g++ -c copy.cpp

clean:
	rm -f *.o copy
