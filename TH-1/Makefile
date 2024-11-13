all: array-summary

array-summary: array_summary.o
	g++ -o array-summary array_summary.o

array_summary.o: array_summary.cpp
	g++ -c array_summary.cpp

clean:
	rm -f array-summary array_summary.o
