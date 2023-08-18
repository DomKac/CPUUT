CC=clang
CFLAGS=-Weverything -Wpedantic -Wno-declaration-after-statement

TESTFILES=src/queue.c test/queue_test.c test/test.c
APPFILES=src/queue.c app/main.c

TESTOBJFILES=src/queue.o test/queue_test.o test/test.o
APPOBJFILES=src/queue.o app/main.o

all: test app

test: $(TESTOBJFILES) 
	$(CC) $(TESTOBJFILES) -o test.out -l m

app: $(APPOBJFILES) 
	$(CC) $(APPOBJFILES) -o main.out

%.o: %.c queue.h
	$(CC) $(CFLAGS) -c $< -o $@
 
clean:
	rm -rf */*.o *.o *.out
