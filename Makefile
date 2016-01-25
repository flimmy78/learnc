CC		=gcc
TARGET	=pthproduce
$(TARGET): pthproduce.c
	gcc -o ./bin/pthproduce pthproduce.c -g -Wall -pthread
clean:
	rm -vf ./bin/* *.o *~
