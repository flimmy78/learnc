CC			=arm-none-linux-gnueabi-gcc
COPY		=cp
CFLAGS		=-g -Wall
LIBS		=-lpthread
TARGET		=pthproduce sempro
BINPATH		=$(PWD)/bin
NFSROOT		=/nfsroot

all: pthproduce sempro

pthproduce: pthproduce.c
	$(CC) -o $(BINPATH)/pthproduce pthproduce.c $(CFLAGS) $(LIBS)
sempro: sempro.c
	$(CC) -o $(BINPATH)/sempro sempro.c $(CFLAGS) $(LIBS)
com: com.c
	$(CC) -o $(BINPATH)/com com.c $(CFLAGS) $(LIBS)
	$(COPY) $(BINPATH)/com $(NFSROOT)
clean:
	rm -vf ./bin/* *.o *~
