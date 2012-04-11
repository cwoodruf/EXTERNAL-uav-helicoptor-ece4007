CC=gcc
CFLAGS=-lssl -lcrypto -lcurses -lpthread   -g
main: menu.o  svr.o cli.o cont.o
	$(CC) -o menu menu.c $(CFLAGS)
	$(CC) -o cli cli.c $(CFLAGS)
	$(CC) -o svr svr.c $(CFLAGS)
	$(CC) -o cont cont.c $(CFLAGS)
	rm -f *.o
cli: cli.o 
	$(CC) -o cli cli.c $(CFLAGS)
svr: svr.o 
	$(CC) -o svr svr.c $(CFLAGS)
cont: cont.o 
	$(CC) -o cont cont.c $(CFLAGS)		

clean:
	rm -f *.o menu cli svr cont
