CC = gcc
CFLAGS = -Wall -g -Werror

.PHONY: all
all: SystemTables

SystemTables: SystemTables.o
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $<

.PHONY: clean
clean:
	rm -f *.o
	rm -f SystemTables*

.PHONY: help
help: Makefile
	@sed -n 's/^##//p' $<
