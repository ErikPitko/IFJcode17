CC=gcc
CFLAGS=-O0 -Wall -Wextra -g

all: IFJcode17

OBJS=error.o ExpressionParser.o garbage.o scanner.o stack.o parser.o symtable.o InstructionList.o address_code.o

run: all
	./IFJcode17 ${ARGS}

IFJcode17: IFJcode17.o $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@

zip: Makefile $(all)
	zip xsajdi00.zip *.c *.h Makefile

tar: Makefile $(all)
	tar -cvzf xsajdi00.tgz *.c *.h Makefile

clean:
	rm -f *.o IFJcode17 *.tgz *.zip

##############################################################

address_code.o: address_code.c address_code.h symtable.h garbage.h \
 error.h scanner.h
error.o: error.c error.h garbage.h
ExpressionParser.o: ExpressionParser.c ExpressionParser.h stack.h \
 scanner.h garbage.h error.h header.h symtable.h InstructionList.h
garbage.o: garbage.c garbage.h error.h
IFJcode17.o: IFJcode17.c error.h garbage.h ExpressionParser.h stack.h \
 scanner.h header.h symtable.h InstructionList.h parser.h address_code.h
InstructionList.o: InstructionList.c InstructionList.h header.h scanner.h \
 garbage.h error.h symtable.h
parser.o: parser.c parser.h scanner.h garbage.h error.h \
 ExpressionParser.h stack.h header.h symtable.h InstructionList.h \
 address_code.h
scanner.o: scanner.c scanner.h garbage.h error.h
stack.o: stack.c stack.h scanner.h garbage.h error.h header.h symtable.h
symtable.o: symtable.c symtable.h garbage.h error.h
