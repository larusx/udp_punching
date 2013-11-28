CC = gcc -g -Wall -std=c99

objs = ../base/binary_tree.o test_binary_tree.o

server:$(objs)
	$(CC) -o test_binary_tree $(objs) 

.PHONY:clean
clean:
	rm test_binary_tree $(objs)
