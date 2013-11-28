CC = gcc -g -Wall -std=c99

objs = punch.o binary_tree.o 

server:$(objs)
	$(CC) -o server $(objs) 

.PHONY:clean
clean:
	rm server $(objs)
