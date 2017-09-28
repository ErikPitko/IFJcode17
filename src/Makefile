CC=gcc
CFLAGS=-O0 -Wall

all: IFJcode17

IFJcode17: IFJcode17.o error.o
	$(CC) $(CFLAGS) $^ -o $@

zip: Makefile $(all)
	zip xsajdi00.zip *.c *.h Makefile

tar: Makefile $(all)
	tar -cvzf xsajdi00.tgz *.c *.h Makefile

clean:
	rm -f *.o IFJcode17 *.tgz *.zip

##############################################################

error.o: error.c error.h
IFJcode17.o: IFJcode17.c
