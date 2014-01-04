CC=gcc
CFLAGS= -g
FINALBIN=NES

all: makestuff

OBJS=$(patsubst %.c,%.o,$(wildcard *.c))

%.o : %.c
	$(CC) $(CFLAGS) -c $<

makestuff: $(OBJS)
	gcc $(CFLAGS) -o $(FINALBIN) $(OBJS)

clean:
	rm *.o
	rm $(FINALBIN)

.PHONY: all
