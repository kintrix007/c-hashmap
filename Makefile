app = app.out
OBJS = hashmap.o main.o

$(app): $(OBJS)
	gcc -O2 -o $@ $(OBJS)

main.o: main.c
	gcc -c -o $@ $?

hashmap.o: hashmap.c
	gcc -c -o $@ $?

clean:
	rm -r *.o $(app)

run:
	make && ./$(app)
