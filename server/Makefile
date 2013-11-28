CC = gcc -g -Wall -std=c99

objs = ../base/punch.o ../base/binary_tree.o server.o 

server:$(objs)
	$(CC) -o p2p_server $(objs) 

.PHONY:clean
clean:
	rm p2p_server $(objs)
