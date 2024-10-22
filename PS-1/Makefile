all: do-command

do-command: do-command.o
	g++ -o do-command do-command.o

do-command.o: do-command.cpp
	g++ -c do-command.cpp

clean:
	rm -f do-command do-command.o