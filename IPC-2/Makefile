CXX = g++
TARGET = prime-calc
OBJS = main.o

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(OBJS) -o $(TARGET)

main.o: main.cpp
	$(CXX) -c main.cpp

clean:
	rm -f $(TARGET) $(OBJS)
