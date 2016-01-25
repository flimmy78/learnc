CC			=gcc
CFLAGS	=-g -Wall
LIBS		=-lpthread
TARGET	=pthproduce sempro
BINPATH	=./bin
all: pthproduce sempro
pthproduce: pthproduce.c
	$(CC) -o $(BINPATH)/pthproduce pthproduce.c $(CFLAGS) $(LIBS)
sempro: sempro.c
	$(CC) -o $(BINPATH)/sempro sempro.c $(CFLAGS) $(LIBS)
clean:
	rm -vf ./bin/* *.o *~
