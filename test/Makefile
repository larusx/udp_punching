CC = gcc -g -Wall -std=c99

#objs = ../base/binary_tree.o test_binary_tree.o
objs = ../base/punch.o test_udp.o

server:$(objs)
#	$(CC) -o test_binary_tree $(objs) 
	$(CC) -o test_udp $(objs) 

.PHONY:clean
clean:
#	rm test_binary_tree $(objs)
	rm test_udp $(objs)
