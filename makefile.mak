#CC : compiler
CC = gcc
#compiler flags
CFLAGS = -Wall -g -Werror

## All: run the prog target
.PHONY: all
all: SystemTables

## prog: link the object files to make the executable
SystemTables: SystemTables.o
	$(CC) $(CFLAGS) -o $@ $^

## %.o compiles C files into object files 
%.o: %.c
	$(CC) $(CFLAGS) -c $<

## clean: remove object files and executable
.PHONY: clean
clean:
	rm -f *.o
	rm -f SystemTables

## help: display this help message
.PHONY: help
help: makefile.mak
	@sed -n 's/^##//p' $<
