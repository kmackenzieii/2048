# Makefile
# Make variables
CC=gcc
CFLAGS= -I. -wall
LDFLAGS=-lncurses

# File names
EXEC=2048
SOURCES=$(wildcard *.c)
OBJECTS=$(SOURCES:.c=.0)

# Main target
$(EXEC): $(OBJECTS)
	$(CC) $(OBJECTS) -o $(EXEC) $(LDFLAGS)

%.0: %.c
	$(CC) -c $(C_FLAGS) $< -o $@

clean:
	rm -f $(EXEC) $(OBJECTS)
