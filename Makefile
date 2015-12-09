 
Client: Client.o socket.o Blockable.o
	g++ -o Client Client.o socket.o Blockable.o -pthread -l rt

Client.o : Client.cpp socket.h
	g++ -c Client.cpp -std=c++11

Server : Server.o thread.o socket.o socketserver.o Blockable.o deck.o card.o hand.o
	g++ -o Server Server.o thread.o socket.o socketserver.o Blockable.o deck.o card.o hand.o -pthread -l rt

Blockable.o : Blockable.cpp Blockable.h
	g++ -c Blockable.cpp -std=c++11
	
Server.o : Server.cpp socket.h socketserver.h thread.h
	g++ -c Server.cpp -std=c++11

thread.o : thread.cpp thread.h 
	g++ -c thread.cpp -std=c++11

socket.o : socket.cpp socket.h
	g++ -c socket.cpp -std=c++11

socketserver.o : socketserver.cpp socket.h socketserver.h
	g++ -c socketserver.cpp
	
deck.o : deck.cpp deck.h
	g++ -c deck.cpp -std=c++11
	
card.o : card.cpp card.h
	g++ -c card.cpp -std=c++11
	
hand.o : hand.cpp hand.h
	g++ -c hand.cpp -std=c++11