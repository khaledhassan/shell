FLAGS=-Wall -Wextra

all: shell

shell: shell.o
	$(CC) $(CFLAGS) shell.o -o shell

shell.o: shell.c
	$(CC) $(CFLAGS) shell.c -c -o shell.o

clean:
	rm -rf *.o shell

