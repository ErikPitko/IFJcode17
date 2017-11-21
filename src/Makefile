CC=gcc
CFLAGS=-O0 -Wall -Wextra -g

all: IFJcode17

OBJS=error.o ExpressionParser.o garbage.o scanner.o stack.o parser.o InstructionList.o

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
 scanner.h garbage.h header.h error.h InstructionList.h
garbage.o: garbage.c garbage.h
IFJcode17.o: IFJcode17.c error.h garbage.h ExpressionParser.h stack.h \
 scanner.h header.h InstructionList.h parser.h
InstructionList.o: InstructionList.c InstructionList.h header.h scanner.h \
 garbage.h error.h
parser.o: parser.c parser.h scanner.h garbage.h ExpressionParser.h \
 stack.h header.h error.h InstructionList.h
scanner.o: scanner.c scanner.h garbage.h
stack.o: stack.c stack.h scanner.h garbage.h header.h
