CXX := g++
TARGET := demo-application

all: libparallel_scheduler.so $(TARGET)

libparallel_scheduler.so: parallel_scheduler.cpp parallel_scheduler.h
	$(CXX) -fPIC -shared -o $@ parallel_scheduler.cpp

$(TARGET): demo_application.cpp libparallel_scheduler.so
	$(CXX) -o $@ demo_application.cpp -L. -lparallel_scheduler -Wl,-rpath,.

clean:
	rm -f *.o *.so $(TARGET)
