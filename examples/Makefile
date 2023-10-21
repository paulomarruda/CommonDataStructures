CC := gcc
CFLAGS := -Wall -Wextra -g -std=c99 -O3
INCLUDE := ../include/
SRC := $(wildcard ../src/*.c)
TARGETS := $(wildcard *.c)
OBJECTS := $(SRC: %.c=%.o)
EXECUTABLES := $(TARGETS: %.c=%.out)

sources: $(SRC)
	$(CC) $(CFLAGS) -I $(INCLUDE) -g -c $@ -o $^
examples: $(TARGET) $(OBJECTS)
	$(CC) $(CFLAGS) -I $(INCLUDE) $^ -o $@
clean: 
	rm -rf *.o *.out
