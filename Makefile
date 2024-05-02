CC = gcc
CFLAGS = -Wall -std=c99 -pedantic
DATA = data
DATA_OBJS = data.o 
PROGS = $(DATA)

all : $(PROGS)

$(DATA) : $(DATA_OBJS)
	$(CC) $(CFLAGS) -o $(DATA) $(DATA_OBJS)

data.o : data.c data.h
	$(CC) $(CFLAGS) -c data.c


clean :
	rm *.o $(PROGS) core
