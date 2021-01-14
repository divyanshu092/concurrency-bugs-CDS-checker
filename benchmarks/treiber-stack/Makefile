include ../benchmarks.mk

main: my_stack.o main.c
	$(CC) -o $@ $^ $(CFLAGS) $(LDFLAGS)

%.o: %.c
	$(CC) -c -o $@ $^ $(CFLAGS)

clean:
	rm -f *.o
