CC := gcc -std=c99 -I ./ 
L1 := ./Labwork1/
L2 := ./Labwork2/

all: Starter Labwork1 Labwork2

Labwork1: $(L1)unicastServer $(L1)unicastClient \
		$(L1)multicastServer $(L1)multicastClient \
		$(L1)broadcastServer $(L1)broadcastClient

Labwork1: $(L2)Server $(L2)Client

%Server: %Server.o Nets.o   
	$(CC) $< Nets.o -o $@

%Client: %Client.o Nets.o
	$(CC) $< Nets.o -o $@

Starter: Starter.o Nets.o
	$(CC) Starter.o Nets.o -o Starter

%.o: %.c
	$(CC) -c -g $< -o $@

Nets.o: Nets.c Nets.h
	gcc  -c -g $< -o $@

clean:
	rm -rf *.o
