CC = gcc
Makefile: TCPServer.o  
	gcc -o server TCPServer.o  
TCPServer.o: TCPServer.c dslkd.h
	gcc -c TCPServer.c  