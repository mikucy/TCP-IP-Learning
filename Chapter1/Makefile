all : server client

server : server.o common.o
	gcc -o server server.o common.o
client : client.o common.o
	gcc -o client client.o common.o

server.o : server.c
	gcc -c server.c
client.o : client.c
	gcc -c client.c
common.o : ../include/common.c
	gcc -c ../include/common.c

.PHONY : clean
clean :
	-rm server.o client.o common.o server client