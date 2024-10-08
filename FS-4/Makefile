TARGET = redirect-cin

all: $(TARGET)

$(TARGET): redirect-cin.o
	g++ -o $(TARGET) redirect-cin.o

redirect-cin.o: redirect-cin.cpp
	g++ -c redirect-cin.cpp

clean:
	rm -f *.o $(TARGET)
