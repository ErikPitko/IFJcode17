CC=gcc
CFLAGS=-O0 -Wall -Wextra -g

all: IFJcode17

OBJS=error.o ExpressionParser.o garbage.o InstructionList.o scanner.o stack.o

IFJcode17: IFJcode17.o $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@

zip: Makefile $(all)
	zip xsajdi00.zip *.c *.h Makefile

tar: Makefile $(all)
	tar -cvzf xsajdi00.tgz *.c *.h Makefile

clean:
	rm -f *.o IFJcode17 *.tgz *.zip

##############################################################

error.o: error.c error.h garbage.h
ExpressionParser.o: ExpressionParser.c ExpressionParser.h stack.h \
 scanner.h garbage.h InstructionList.h
garbage.o: garbage.c garbage.h
IFJcode17.o: IFJcode17.c error.h garbage.h
InstructionList.o: InstructionList.c InstructionList.h garbage.h \
 scanner.h error.h
scanner.o: scanner.c scanner.h
stack.o: stack.c stack.h scanner.h garbage.h ExpressionParser.h
