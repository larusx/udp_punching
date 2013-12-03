CC = gcc -g -Wall -std=c99 

objs = ../base/punch.o client.o 

server:$(objs)
	$(CC) -o p2p_client $(objs) -lpthread

.PHONY:clean
clean:
	rm p2p_client $(objs)
