app = app.out
OBJS = hashmap.o main.o
CFLAGS = -std=c11 -Wall -Wextra -Werror -Wpedantic -Wnull-dereference -ggdb3
TEST = test-suite

.PHONY: test run clean

$(app): $(OBJS)
	gcc $(CFLAGS) -o $@ $^

main.o: main.c
	gcc $(CFLAGS) -c -o $@ $?

hashmap.o: hashmap.c
	gcc $(CFLAGS) -c -o $@ $?

tests/test.o: tests/test.c
	gcc -std=c11 -ggdb3 -c -o $@ $?

lib/gest.o: lib/gest.c
	gcc -std=c11 -ggdb3 -c -o $@ $? 2>/dev/null

test: hashmap.o tests/test.o lib/gest.o
	gcc -std=c11 -ggdb3 -o $(TEST) $^
	@printf "\n---\n\n"
	valgrind -q --leak-check=full --track-origins=yes -s ./$(TEST)

run:
	make && ./$(app)

clean:
	-rm -r *.o */*.o $(app) $(TEST)
