CC := gcc -std=c99 -I ../

all: unicastServer unicastClient multicastServer multicastClient Starter broadcastServer broadcastClient

%Server: %Server.o ../Nets.o   
	$(CC) $< ../Nets.o -o $@

%Client: %Client.o ../Nets.o
	$(CC) $< ../Nets.o -o $@

Starter: Starter.o ../Nets.o
	$(CC) Starter.o ../Nets.o -o Starter

%.o: %.c
	$(CC) -c -g $< -o $@

../Nets.o: ../Nets.c ../Nets.h
	gcc  -c -g $< -o $@

clean:
	rm -f *.o
	rm -f ../*.o
