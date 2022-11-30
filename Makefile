app = app.out
OBJS = hashmap.o main.o
CFLAGS = -std=c11 -Wall -ggdb3
# CFLAGS = -O2

$(app): $(OBJS)
	gcc $(CFLAGS) -o $@ $(OBJS)

main.o: main.c
	gcc -c -o $@ $?

hashmap.o: hashmap.c
	gcc -c -o $@ $?

clean:
	rm -r *.o $(app)

run:
	make && ./$(app)
