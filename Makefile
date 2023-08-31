CC=clang
HEADERS=-Iinc
CFLAGS=-Weverything -Wpedantic -Wno-declaration-after-statement $(HEADERS)

SRCFILES=$(wildcard src/*.c)
TESTFILES=$(wildcard test/*.c)
APPFILES=$(SRCFILES) app/main.c

SRCOBJFILES=$(SRCFILES:.c=.o)
TESTOBJFILES=$(TESTFILES:.c=.o)
APPOBJFILES=$(SRCOBJFILES) app/main.o

all: test app

test: $(TESTOBJFILES) $(SRCOBJFILES)
	$(CC) $(TESTOBJFILES) $(SRCOBJFILES) -o test.out -lm

app: $(APPOBJFILES)
	$(CC) $(APPOBJFILES) -o main.out

src/%.o: src/%.c inc/%.h
	$(CC) $(CFLAGS) -c $< -o $@

test/%.o: test/%.c
	$(CC) $(CFLAGS) -c $< -o $@

app/main.o: app/main.c $(wildcard inc/*.h)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(SRCOBJFILES) $(TESTOBJFILES) app/main.o *.out
