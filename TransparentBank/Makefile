all: bank-init bank-deinit client server client-online

bank-init: bank-init.cpp bank.cpp bank.hpp
	g++ bank-init.cpp bank.cpp -o bank-init -pthread

bank-deinit: bank-deinit.cpp bank.cpp bank.hpp
	g++ bank-deinit.cpp bank.cpp -o bank-deinit -pthread

client: client.cpp bank.cpp bank.hpp
	g++ client.cpp bank.cpp -o client -pthread

server: server.cpp bank.cpp bank.hpp
	g++ server.cpp bank.cpp -o server -pthread

client-online: client-online.cpp
	g++ client-online.cpp lib/libcolorprint.a -o client-online \
	    -Iinclude -pthread

clean:
	rm -f bank-init bank-deinit client server client-online
